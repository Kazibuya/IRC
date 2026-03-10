# ft_irc

> Et si je faisais mon propre serveur IRC ?

![C++](https://img.shields.io/badge/language-C++98-blue.svg)
![Norminette](https://img.shields.io/badge/style-Norminette-00BABC.svg)
![42](https://img.shields.io/badge/school-42-black.svg)
![Group](https://img.shields.io/badge/project-group-purple.svg)

---

## 📋 Description

`ft_irc` est un projet de groupe du cursus **École 42** consistant à implémenter un **serveur IRC** conforme au protocole [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459), entièrement en **C++98**.

Le serveur gère simultanément plusieurs clients, l'authentification, la messagerie, les opérations sur les canaux et la gestion des utilisateurs. Étant le seul projet C++ "complet" du tronc commun, il met en jeu la totalité des connaissances acquises jusqu'alors : programmation orientée objet, gestion des sockets, et architecture réseau.

```bash
./ircserv <port> <password>
# Exemple : ./ircserv 6667 pass
```

*Projet réalisé par **pmilner-** et **namichel**.*

---

## 📁 Structure du projet

```
IRC/
├── SRCS/          # Sources C++ du serveur
├── INCLUDES/      # En-têtes (.hpp)
├── BOT/           # Client bot autonome (bonus)
├── Makefile
└── README.md
```

---

## 🧠 Architecture : le pipeline de gestion d'erreurs

Le choix architectural le plus marquant de ce projet est l'utilisation d'un **pipeline de validation** pour gérer les erreurs, à la place d'une cascade de `if/else if` imbriqués.

### Le problème avec les `if`

Chaque commande IRC nécessite plusieurs validations avant d'être exécutée : le client est-il enregistré ? A-t-il les droits ? Le canal existe-t-il ? Les arguments sont-ils valides ? L'approche naïve devient vite un enfer à lire et maintenir :

```cpp
// ❌ Approche classique — ingérable dès la 3e validation
void Server::handleKick(Client &client, std::vector<std::string> &args)
{
    if (!client.isRegistered()) {
        sendError(client, ERR_NOTREGISTERED);
        return ;
    } else if (args.size() < 2) {
        sendError(client, ERR_NEEDMOREPARAMS);
        return ;
    } else if (!channelExists(args[0])) {
        sendError(client, ERR_NOSUCHCHANNEL);
        return ;
    } else if (!client.isOperator(args[0])) {
        sendError(client, ERR_CHANOPRIVSNEEDED);
        return ;
    }
    // ... et enfin, la vraie logique, enfouie sous les conditions
}
```

Avec 9 commandes obligatoires et 5 bonus, chacune ayant ses propres règles IRC (codes d'erreur numérotés `401`, `403`, `461`...), ce pattern devient un cauchemar à débugger.

### La solution : un pipeline de validators

```cpp
// ✅ Approche pipeline — chaque étape est atomique et réutilisable
typedef bool (Server::*Validator)(Client &, std::vector<std::string> &);

std::vector<Validator> pipeline;
pipeline.push_back(&Server::checkRegistered);
pipeline.push_back(&Server::checkArgs);
pipeline.push_back(&Server::checkChannelExists);
pipeline.push_back(&Server::checkIsOperator);

// Exécution : s'arrête dès qu'un validator retourne false (et envoie l'erreur lui-même)
for (size_t i = 0; i < pipeline.size(); ++i)
{
    if (!(this->*pipeline[i])(client, args))
        return ;
}
// Ici, toutes les validations sont passées — on peut exécuter la logique métier
doKick(client, args);
```

### Pourquoi c'est une idée de génie 💡

Chaque validator est une fonction indépendante qui envoie elle-même le bon code d'erreur IRC et retourne `false` pour court-circuiter le pipeline. La logique métier de la commande n'est atteinte que si **toutes** les validations passent. Les bénéfices sont immédiats :

- **Réutilisabilité** : `checkRegistered` ou `checkChannelExists` sont partagés entre toutes les commandes sans duplication.
- **Lisibilité** : la liste des validators d'une commande se lit comme une spec — on comprend d'un coup d'œil quelles erreurs peuvent être levées.
- **Maintenabilité** : corriger la gestion d'une erreur à un seul endroit la corrige pour toutes les commandes qui utilisent ce validator.
- **Conformité RFC** : chaque validator encapsule le bon code numérique IRC (`ERR_NOTREGISTERED`, `ERR_CHANOPRIVSNEEDED`...), garantissant une réponse conforme à la spec.

C'est exactement ce genre de réflexion — *"quelle est la façon la plus propre de structurer ça ?"* — qui a guidé l'architecture de ce projet.

---

## ⚙️ Commandes supportées

### Partie obligatoire

| Commande | Description |
|---|---|
| `PASS` | Authentification avec mot de passe |
| `NICK` | Définir / changer son pseudonyme |
| `USER` | Enregistrement de l'utilisateur |
| `JOIN` | Rejoindre un canal |
| `KICK` | Expulser un utilisateur d'un canal |
| `INVITE` | Inviter un utilisateur dans un canal |
| `TOPIC` | Voir / modifier le sujet d'un canal |
| `MODE` | Modifier les modes d'un canal ou d'un utilisateur |
| `PRIVMSG` | Envoyer un message privé ou vers un canal |

### Bonus

| Commande | Description |
|---|---|
| `CAP` | Négociation de capacités client |
| `MOTD` | Message du jour |
| `PART` | Quitter un canal |
| `QUIT` | Se déconnecter du serveur |
| `WHO` | Lister les utilisateurs d'un canal |

---

## 🚀 Compilation & Lancement

### Compiler le serveur

```bash
make
```

### Lancer le serveur

```bash
./ircserv <port> <password>
# Exemple :
./ircserv 6667 pass
```

### Avec Valgrind

```bash
make valgrind
```

### Arrêter le serveur

```bash
CTRL+C   # Envoie un SIGINT proprement intercepté
```

---

## 🔌 Se connecter au serveur

### Avec HexChat (recommandé)

1. Ouvrir HexChat → *Network List* → *Add*
2. Adresse : `localhost` ou `127.0.0.1` (ou l'IP via `hostname -i`)
3. Port : `6667`
4. **Désactiver** `Use SSL for all the servers on this network`
5. Définir le mot de passe dans les paramètres du réseau

> HexChat suit un ordre de registration précis et envoie des pings automatiques une fois connecté — le serveur gère ces spécificités.

### Avec Netcat (debug rapide)

```bash
nc -C 127.0.0.1 6667
```

> Le flag `-C` est indispensable : il force la terminaison des lignes en `\r\n`, obligatoire pour que le serveur IRC analyse correctement les paquets.

---

## 🤖 Bonus — Le Bot

Le bot est un client autonome qui se connecte au serveur et répond à des commandes préfixées par `!`. Il réside toujours dans le canal `#bot`.

### Lancer le bot

```bash
cd BOT && make
./simplebot <ip> <port> <password>
```

### Commandes du bot

| Commande | Description |
|---|---|
| `!rtd <N>` | Lance un dé à N faces — ex: `!rtd 6` retourne un nombre entre 1 et 6 |
| `!russian_roulette <cible1> <cible2> ...` | Joue à la roulette russe avec les cibles données (peut kick si le bot est opérateur) |
| `!connect_4 <difficulté / colonne>` | Lance ou joue au Puissance 4 contre un algorithme **minimax** |

#### Puissance 4 en détail

Le premier appel à `!connect_4` définit la **difficulté** (profondeur du minimax). Les appels suivants permettent de placer une pièce dans la colonne `1` à `7`. Plus la difficulté est haute, plus l'algorithme anticipe de coups à l'avance — et plus il devient redoutable.

---

## 📌 Contraintes du projet

- Langage : **C++98** uniquement
- Respect de la **Norme 42** (Norminette)
- Pas de variables globales
- Gestion non-bloquante des I/O (`poll()` ou équivalent)
- Le serveur ne doit jamais crasher ni se bloquer
- Compatible avec le client **HexChat**

---

## 📚 Ressources

- [RFC 1459 — Protocole IRC](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — Syntaxe des commandes & réponses numériques](https://datatracker.ietf.org/doc/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split-wide/)
- [Socket Programming en C++](https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/)
- [Algorithme Minimax (Connect 4)](https://en.wikipedia.org/wiki/Minimax)

---

## 👥 Auteurs

**Kazibuya** — [GitHub](https://github.com/Kazibuya) | **pmilner-** | **namichel**
