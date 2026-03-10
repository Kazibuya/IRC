# ft_irc

> *"Et si je faisais mon propre serveur ?"*

![C++](https://img.shields.io/badge/language-C++98-blue.svg)
![Norminette](https://img.shields.io/badge/style-Norminette-00BABC.svg)
![42](https://img.shields.io/badge/school-42-black.svg)
![Group](https://img.shields.io/badge/project-group-purple.svg)

---

## 📋 Description

`ft_irc` est un projet de groupe du cursus **École 42** consistant à implémenter un **serveur IRC** conforme au protocole [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459), entièrement en **C++98**.

Le serveur gère simultanément plusieurs clients via `poll()`, l'authentification, la messagerie, les opérations sur les canaux et la gestion des utilisateurs.

```bash
./ircserv <port> <password>
# Exemple : ./ircserv 6667 pass
```

*Projet réalisé par **pmilner-** et **namichel**.*

---

## 📁 Structure du projet

```
IRC/
├── SRCS/
│   └── COMMANDS/
│       └── CommandManager.cpp   # Dispatch + exécution de toutes les commandes
├── INCLUDES/
│   ├── SERVER/
│   │   ├── Server.hpp           # Classe principale : boucle, clients, canaux
│   │   ├── Network.hpp          # Gestion des sockets et poll()
│   │   ├── Client.hpp           # État d'un client connecté (buffers I/O, registration)
│   │   └── Channel.hpp          # Canal IRC (membres, opérateurs, modes, invitations)
│   ├── PARSING/
│   │   └── Token.hpp            # Parsing d'un message IRC brut
│   ├── COMMANDS/
│   │   └── CommandManager.hpp   # commandManager + commandBag + pipeline de validators
│   └── NAMESPACES/
│       ├── Errors.hpp           # Toutes les erreurs IRC (ERR_*)
│       └── Replies.hpp          # Toutes les réponses IRC (RPL_*)
├── BOT/
│   ├── Bot.cpp / Bot.hpp        # Client bot autonome
│   ├── commands.cpp             # !rtd, !russian_roulette, !connect_4
│   ├── client_handler.cpp       # Connexion, registration, I/O réseau du bot
│   ├── Token.cpp / Token.hpp    # Parser IRC côté bot
│   └── Power4.cpp / Power4.hpp  # Moteur Puissance 4 + algorithme minimax
└── Makefile
```

---

## 🏗️ Architecture du serveur

### La boucle principale — `serverLoop()`

Tout part de `serverLoop()` dans `Server.cpp`. À chaque itération, `_network.eventHandler()` appelle `poll()` sur **tous les fd** (serveur + clients) et remplit deux listes : les fd prêts en lecture et en écriture. Le serveur itère ensuite dessus :

```cpp
void Server::serverLoop()
{
    signal(SIGINT, &signal_override); // running = false sur CTRL+C
    while (running)
    {
        std::vector<int> read_events;
        std::vector<int> write_events;

        _network.eventHandler(read_events, write_events); // poll() bloquant

        for (size_t i = 0; i < read_events.size(); i++)
        {
            if (read_events[i] != _network.get_listen())
                handle_client_input(_clients[read_events[i]]); // données client
            else
                accept_new_client(); // nouveau client
        }
        for (size_t i = 0; i < write_events.size(); i++)
            handle_client_output(_clients[write_events[i]]); // vider les out_buffers

        remove_disconnected_clients(); // nettoyage différé
    }
}
```

La déconnexion est **différée** : `prepareDisconnect()` flag le client comme `_connected = false` et retire ses messages des canaux, mais le `delete` effectif n'arrive qu'à `remove_disconnected_clients()` en fin de boucle — évitant d'invalider des itérateurs en cours d'utilisation.

---

### `Network` — La couche socket

`Network` encapsule tout le bas niveau : création du socket avec `SO_REUSEADDR` + `O_NONBLOCK` via `fcntl()`, `bind()`, `listen()`, et un `std::vector<pollfd> _pollfds` qui grandit à chaque nouvelle connexion. `eventHandler()` appelle `poll(-1)` (bloquant indéfiniment) et retourne les deux listes de fd actifs. `send_data()` désactive `POLLOUT` sur le fd dès que le buffer est vidé, évitant des `send()` inutiles à chaque tour de boucle.

### `Client` — L'état d'un client

Chaque client est un objet `Client` dans `std::map<int, Client*> _clients` (indexée par fd socket). Il porte trois flags d'état : `_connected`, `_password` (PASS reçu et correct), `_registered` (PASS + NICK + USER complétés). Le nick par défaut est `"*"` tant que NICK n'est pas reçu — ce qui permet au serveur de formater proprement les erreurs même avant la registration.

Les deux buffers sont au cœur du modèle non-bloquant : `_input_buffer` accumule les `recv()` jusqu'à trouver un `\r\n` complet dans `parse_and_execute()` ; `_output_buffer` stocke tout ce qui doit être envoyé, et `addToClientOutBuffer()` active `POLLOUT` sur le fd dès qu'il reçoit quelque chose.

### `Channel` — Un canal IRC

Les modes du canal sont encodés dans un **`char _modes` utilisé comme bitmask** — chaque bit correspond à un mode dans l'ordre `"itkol"` :

```cpp
void Channel::setMode(char &mode_in, bool state)
{
    char mode_arr[6] = "itkol"; // bit 0=i, 1=t, 2=k, 3=o, 4=l
    for (int i = 0; i < 5; i++)
        if (mode_in == mode_arr[i])
        {
            if (state)  _modes |=  (1 << i); // SET le bit
            else        _modes &= ~(1 << i); // CLEAR le bit
        }
}
```

`getMode('k')` vérifie le bit correspondant, `getModesString()` reconstruit la chaîne de modes lisible (`"itk"`, `"+l"`...). Les membres sont dans `std::map<int, Client*>` (indexée par fd), les opérateurs dans `std::vector<int>` (liste de fd).

### `Token` — Le parser IRC

Le constructeur `Token(std::string raw)` parse une ligne IRC brute en une seule passe :

```
:nick!user@host  CMD  param1  param2  :contenu long ici
 ↓ _sender        ↓ _cmd  ↓ _params  ↓ _content
```

Il trim d'abord le `\r\n`, extrait le préfixe si la ligne commence par `:`, uppercase la commande, puis split les paramètres jusqu'à rencontrer un `:` qui marque le début du contenu libre. La commande est uppercasée à la volée — ce qui rend le serveur insensible à la casse (`join` = `JOIN`).

---

### `Errors` & `Replies` — Les namespaces IRC

Toutes les réponses du serveur passent par deux namespaces distincts, chacun reposant sur un helper interne :

```cpp
// Errors.cpp
static void sendErr(Server &s, Client &c, const std::string &code, const std::string &msg)
{
    std::string full = ":ft_irc " + code + " " + c.getNick() + " " + msg + "\r\n";
    s.addToClientOutBuffer(c, full);
}

// Replies.cpp
static void sendRpl(Server &s, Client &c, const std::string &code, const std::string &msg)
{
    std::string full = ":ft_irc " + code + " " + c.getNick() + " " + msg + "\r\n";
    s.addToClientOutBuffer(c, full);
}
```

Chaque fonction du namespace correspond à un code numérique RFC précis :

| Code | Fonction | Déclencheur |
|:---:|---|---|
| `001` | `rplWelcome` | Registration complète |
| `331/332` | `rplNoTopic` / `rplTopic` | JOIN ou TOPIC query |
| `353/366` | `rplNamReply` / `rplEndOfNames` | JOIN |
| `341` | `rplInviting` | INVITE réussi |
| `324` | `rplChannelModeIs` | MODE query |
| `372/375/376` | `rplMotd*` | Connexion + MOTD |
| `352/315` | `rplWhoReply` / `rplEndOfWho` | WHO |
| `401` | `errNoSuchNick` | PRIVMSG/KICK vers nick inexistant |
| `403` | `errNoSuchChannel` | Commande sur canal inexistant |
| `433` | `errNicknameInUse` | NICK déjà pris |
| `442` | `errNotOnChannel` | Action sur canal dont l'émetteur n'est pas membre |
| `461` | `errNeedMoreParams` | Paramètres manquants |
| `464` | `errPasswdMismatch` | Mauvais mot de passe |
| `471/473/475` | `errChannelIsFull/InviteOnly/BadKey` | JOIN bloqué par mode `l`/`i`/`k` |
| `482` | `errChanOPrivsNeeded` | Action d'opérateur sans être op |

---

### Le `commandBag`

Avant d'exécuter une commande, toutes les données nécessaires sont regroupées dans un `commandBag` :

```cpp
struct commandBag
{
    Token       &t;             // Message parsé
    Client      &sender;        // Client émetteur
    Client      *targetClient;  // Client cible (si applicable)
    Channel     *targetChan;    // Canal cible (si applicable)
    std::string itChan;         // Canal itéré (pour JOIN multi-canaux)
    std::string itNick;         // Nick itéré (pour KICK multi-cibles)
    std::string prefix;         // Préfixe IRC formaté : ":nick!user@host"
    std::string reason;         // Raison (KICK, QUIT...)
    std::string itKey;          // Clé itérée (pour JOIN avec clés)
    bool        isNotice;       // Distingue PRIVMSG de NOTICE
};
```

### Le problème des `if` en cascade

Chaque commande IRC exige plusieurs validations avant d'être exécutée. Sans pipeline, chaque handler ressemble à ça :

```cpp
// ❌ Ingérable dès la 3e validation
void execKick(commandBag &bag)
{
    if (!bag.sender.getIsRegistred()) {
        Errors::errNotRegistered(_srv, bag.sender); return; }
    if (bag.t.getParam(0).empty()) {
        Errors::errNeedMoreParams(_srv, bag.sender, "KICK"); return; }
    if (!getChannel(bag.t.getParam(0))) {
        Errors::errNoSuchChannel(_srv, bag.sender, bag.t.getParam(0)); return; }
    if (!chan->isOperator(&bag.sender)) {
        Errors::errChanOPrivsNeeded(_srv, bag.sender, chan->getName()); return; }
    // ... enfin, la logique métier
}
```

Avec 14 commandes, chacune ayant ses propres règles RFC, ce pattern devient un cauchemar.

### La solution : `runPipeline()`

```cpp
typedef bool (commandManager::*cmdPipeline)(commandBag &bag);

bool commandManager::runPipeline(commandBag &bag, cmdPipeline pipeline[])
{
    for (int i = 0; pipeline[i] != NULL; i++)
        if (!(this->*pipeline[i])(bag))
            return false;
    return true;
}
```

Chaque commande déclare un tableau de pointeurs de méthodes — ses validators — et `runPipeline()` les exécute dans l'ordre. **Dès qu'un validator retourne `false`**, il a déjà envoyé le bon code d'erreur IRC au client, et le pipeline s'arrête. La logique métier n'est atteinte que si tout est valide :

```cpp
void commandManager::execKick(commandBag &bag)
{
    cmdPipeline pipeline[] = {
        &commandManager::_isRegistered,
        &commandManager::_needTwoParam,
        &commandManager::_channelExists,
        &commandManager::_senderIsMember,
        &commandManager::_senderIsOp,
        &commandManager::_targetIsInChannel,
        NULL
    };
    if (!runPipeline(bag, pipeline))
        return;
    // Ici, toutes les validations sont passées.
    // On peut kick.
}
```

Les validators sont des méthodes privées de `commandManager`, chacune responsable d'un seul check et d'une seule erreur RFC. `_isRegistered` envoie `ERR_NOTREGISTERED`, `_senderIsOp` envoie `ERR_CHANOPRIVSNEEDED`, etc. Ils sont **entièrement réutilisables** entre commandes — `_channelExists` est partagé par `JOIN`, `KICK`, `MODE`, `TOPIC`, `INVITE`, `PART`... sans une ligne dupliquée.

---

## ⚙️ Commandes supportées

Chaque commande déclare son propre pipeline de validators. Voici les pipelines réels tels qu'implémentés dans le code :

### Partie obligatoire

**`PASS`**
```
_isAlreadyRegistered → _needOneParam → _passwdMismatch
→ setPassSuccess()
```

**`NICK`**
```
_needPassBeforeNick → _needOneParam → _isNickFormatValid → _isNickAlreadyInUse
→ setNick() + broadcast aux canaux communs (si déjà enregistré)
→ setNick() + _checkRegistration() (si en cours d'enregistrement)
```
> `_checkRegistration()` vérifie si PASS + NICK + USER sont tous reçus — et si oui, envoie les `RPL_WELCOME`, `RPL_YOURHOST`, `RPL_CREATED`, `RPL_MYINFO` et le MOTD automatiquement.

**`USER`**
```
_isAlreadyRegistered → _needThreeParamOrContent
→ setUsername() + setRealname() + _checkRegistration()
```

**`JOIN`**
```
_isRegistered → _needOneParam → _checkJoinZero
→ pour chaque canal (multi-canal supporté, clés splitées par ',') :
   si canal nouveau → création + op automatique du créateur
   si canal existant → _checkInviteMode → _checkKeyMode → _checkLimitMode → _consumeInvite
→ addClient() + broadcast JOIN + RPL_TOPIC/NOTOPIC + RPL_NAMREPLY + RPL_ENDOFNAMES
```

**`KICK`**
```
_isRegistered → _needTwoParam → _channelExists → _senderIsMember
→ _senderIsOp → _targetClientExists → _targetIsInChannel
→ broadcast KICK + removeClient()
```

**`INVITE`**
```
_isRegistered → _needTwoParam → _targetClientExists → _channelExists
→ _senderIsMember → _targetAlreadyInChannel → _inviteOnlyCheck
→ inviteClient() + RPL_INVITING + INVITE au client cible
```

**`TOPIC`**
```
_isRegistered → _needOneParam → _channelExists → _senderIsMember
→ _isTopicQuery  (si pas de contenu → RPL_TOPIC ou RPL_NOTOPIC, stop)
→ _senderHasTopicPrivileges  (vérifie mode 't')
→ setTopic() + broadcast TOPIC
```

**`MODE`**
```
_isRegistered → _needOneParam → _channelExists
→ _isModeQuery  (si un seul param → RPL_CHANNELMODEIS, stop)
→ _senderIsOp
→ parsing des flags (+/-) via une map<char, ModeHandler> :
   'i' → handleInvite    't' → handleTopic
   'k' → handleKey       'l' → handleLimit    'o' → handleOp
→ broadcast MODE avec les modes effectivement changés
```

**`PRIVMSG` / `NOTICE`**
```
_isRegistered → _needOneParam → _hasContent
→ pour chaque cible (multi-cible supporté, splitées par ',') :
   si '#...' → _channelExists → _senderIsMember → sendMessageToMembers()
   sinon     → _targetClientExists → addToClientOutBuffer()
```
> `NOTICE` utilise le même handler avec `bag.isNotice = true` — les validators `_channelExists` et `_targetClientExists` court-circuitent silencieusement sans envoyer d'erreur.

### Bonus

**`PART`**
```
_isRegistered → _needOneParam
→ pour chaque canal (multi-canal supporté) :
   _channelExists → _senderIsMember
   → broadcast PART + removeClient()
   → destroyChannel() si canal vide
```

**`QUIT`**
```
(pas de pipeline — pas de prérequis)
→ broadcast QUIT + prepareDisconnect()
```

**`WHO`**
```
_isRegistered → _needOneParam
→ si '#...' → _channelExists → RPL_WHOREPLY pour chaque membre + RPL_ENDOFWHO
→ sinon     → _targetClientExists → RPL_WHOREPLY + RPL_ENDOFWHO
```

**`CAP`** — Négociation de capacités (LS / REQ→NAK / END / LIST)

**`MOTD`** — Envoie l'ASCII art `ft_irc` en RPL_MOTD au client

**`PING`** — Répond `PONG` immédiatement (keepalive HexChat)

---

## 🚀 Compilation & Lancement

```bash
make               # Compile ircserv
make valgrind      # Lance avec Valgrind
./ircserv 6667 pass
CTRL+C             # SIGINT intercepté proprement
```

---

## 🔌 Se connecter

### HexChat
1. *Network List* → *Add* → adresse `localhost`, port `6667`
2. Désactiver `Use SSL for all the servers on this network`
3. Renseigner le mot de passe dans les paramètres du réseau

### Netcat (debug)
```bash
nc -C 127.0.0.1 6667
```
Le flag `-C` est obligatoire : il termine les lignes en `\r\n`, nécessaire pour que le serveur parse correctement les messages IRC.

---

## 🤖 Bonus — Le Bot (`simplebot`)

Le bot est un **client C++ autonome** qui se connecte au serveur, rejoint `#bot`, et répond à des commandes préfixées par `!`. Il utilise `poll()` avec un unique `pollfd` et deux buffers I/O, exactement comme le serveur.

### Compilation & lancement
```bash
cd BOT && make
./simplebot <ip> <port> <password>
```

### `Token` — deux implémentations

Le projet contient **deux parsers `Token` distincts** : celui du serveur (dans `INCLUDES/PARSING/`) et celui du bot (dans `BOT/`). Ils partagent le même concept mais diffèrent légèrement — le bot a besoin d'un accès indexé aux mots du contenu (`getContent(size_t index)`) pour parser les arguments de commandes comme `!rtd 6` ou `!connect_4 3`, alors que le serveur n'a besoin que du contenu brut.

### Commandes du bot

| Commande | Description |
|---|---|
| `!rtd <N>` | Lance un dé à N faces |
| `!russian_roulette <cible1> <cible2> ...` | Roulette russe — peut kick les cibles si le bot est opérateur |
| `!connect_4 <difficulté/colonne>` | Puissance 4 contre un algorithme minimax |

### Puissance 4 — le moteur `Power4`

La classe `Power4` encapsule une grille `int _grid[6][7]` et implémente un **minimax avec élagage alpha-bêta** :

```cpp
int Power4::minimax(int deepness, bool isMax, int alpha, int beta)
{
    // Cas terminaux : victoire BOT (+42000), victoire HUMAN (-42000), plateau plein
    if (checkWin(BOT))   return 42000 + deepness;  // + deepness = préfère gagner vite
    if (checkWin(HUMAN)) return -42000 - deepness;
    if (boardIsFull() || deepness == 0) return evaluate();
    // ...élagage alpha-bêta classique
}
```

La fonction `evaluate()` score la position courante en analysant toutes les fenêtres de 4 cases (verticales, horizontales, diagonales). Elle favorise aussi le **contrôle de la colonne centrale** (colonne 4), un principe bien connu aux échecs et au Puissance 4. L'ordre d'exploration `{3, 2, 4, 1, 5, 0, 6}` priorise le centre pour améliorer l'élagage.

La `_difficulty` passée à `!connect_4` fixe la **profondeur de recherche** du minimax : à difficulté 1 il joue presque au hasard, à difficulté 9 il anticipe 9 coups à l'avance et devient quasi imbattable.

---

## 📌 Contraintes du projet

- Langage : **C++98** uniquement
- I/O non-bloquants avec **`poll()`**
- Pas de variables globales (sauf `running` dans le bot pour `SIGINT`)
- Compatible **HexChat**
- Pas de fuites mémoire

---

## 📚 Ressources

- [RFC 1459 — Protocole IRC](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — Commandes & réponses numériques](https://datatracker.ietf.org/doc/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split-wide/)
- [Algorithme Minimax](https://en.wikipedia.org/wiki/Minimax)

---

## 👥 Auteurs

**Kazibuya** — [GitHub](https://github.com/Kazibuya) | **pmilner-** | **namichel**
