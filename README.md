*This project has been created as part of the 42 curriculum by pmilner-, namichel*

---

##### ft_irc
###### DESCRIPTION
`ft_irc` is a group project in which the members of the group have to create a C++98 server compliant with the IRC (Internet Relay Chat) protocol.
The server itself needs to handle several aspects of an IRC server, including handling multiple clients simultaneously, user authentication, messaging, channel operations, and user management.

The goal of the project is to understand the core aspects of network programming, socket communication, and protocol integration; additionally, it touches upon error handling and scalability.

A consequence of it being the only "full" C++ project in the common core, is that we're expected to use the totality of our knowledge of the programming language until this point, and thus write clean object oriented code.

###### INSTRUCTIONS
To compile the project, simply run the `make` command in the root of the project directory.
If you would like to test the program with a memory debugging tool, you make run `make valgrind`.

Running the `ircserv` binary requires two parameters, `<port>` and `<password>`.
example: `./ircserv 6667 pass`

To connect to the server, use the IRC client HexChat, add the server to the server list, either by using `localhost` / `127.0.0.1` or the computer's IP address (`hostname -i`).
The server CANNOT be communicated to over a secure connection (disable `Use SSL for all the servers on this network`).

You may also use the Netcat command to connect to the server; see the below section for more information.

To stop the server, simply send a SIGINT (CTRL+C) to the program.

###### FEATURES
MANDATORY COMMANDS:
- INVITE
- JOIN
- KICK
- MODE
- NICK
- PASS
- PRIVMSG
- TOPIC
- USER


BONUS COMMANDS:
- CAP
- MOTD
- PART
- QUIT
- WHO




###### RESOURCES
- An expansive documentation of the IRC protocol:
	https://datatracker.ietf.org/doc/html/rfc1459
- Documentation of command syntax, numeric replies, etc
	https://datatracker.ietf.org/doc/html/rfc2812
- Basics of sockets
	https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
- Expansive guide on network programming
	https://beej.us/guide/bgnet/html/split-wide/
- cppreference


AI was used in the project, but purely as a way to design clean, scalable structures.
For example, we use prompts along the lines of: `What is the cleanest way to do X?` or `How should X be structured?`.
Every use of AI was paired with documentation, and each prompt was prefaced with `Without giving me code, ...` .

---

##### BONUS

###### DETAILS
As per the subject, the bonus of `ft_irc` is to implement a bot alongside file transfer.
As the file transfer is fully handled by our client of choice, you will only find the BOT in the code base.

###### DESCRIPTION
The bot is an automated client that can receive messages that take the form of bot commands (see below), execute commands, and send responses.
It will always be in the `#bot` channel.

###### INSTRUCTIONS
The bot is located in the BOT folder, it works as a client that is completely separate from the server.
To run the bot, you need to compile it with `make`, then run it as follows:
`./simplebot <ip> <port> <password>`.

If there is already a bot running, the server isn't reachable, or the password is incorrect, the bot will throw an error.
The bot can be stopped in the same was as the server.

###### FEATURES
The bot can receive commands in the style of `!<command>`.
The accepted commands are:

- `!rtd <param>`
A simple dice roll with the param as the upper limit of randomization.
ex: `!rtd 6` will return a random number between 1 and 6.

- `!russian_roulette <target1> <target2> ...`
The bot will play Russian roulette with the targets being the targets (victims?).
The parameters can be anything ranging from songs to the nicknames of the users in the channel.
If you want the bot to kick targets, the bot needs to be an operator.

- `!connect_4 <difficulty / column>`
A co-op game where users of the channel can play Connect 4 (Puissance 4) against an unhinged algortihm.

The first execution of the command dictates the difficulty of the game, and following executions of the command allows you to place pieces in columns 1 through 7.

The difficulty modifies the depth of the minimax algorithm, making it able to place pieces with greater foresight.

###### RESOURCES
- Basics of sockets
	https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
- The Connect4 game algorithm
	https://en.wikipedia.org/wiki/Minimax

---

##### ADDITIONAL INFORMATION
###### NETCAT
Netcat is a command line utility tool that allows you to read and write data across network connections and sockets, it is NOT an IRC client but more-so a powerful generalist networking tool.
It can be run as such:
`nc -C <address> <port>`
The -C parameter forces all finished lines sent over Netcat to be terminated with a `\r\n`, necessary for servers compliant to the IRC protocol to understand what is the end of finished packet and what is simply partially sent data.

###### HEXCHAT
HexChat is an IRC client that works with a GUI, facilitating file transfer, messaging, sending commands, and more.
One specificity with HexChat in regards to the project is that not only does HexChat follow a certain order for registration; after registration, if certain replies are sent to the client, it will consider the server as 'compliant' and do some extra special things, like automatic pings, automatically joining channels on connection, etc.
We tried to implement as many things as possible, but alas we only have so much time, and want to tackle other projects.