# Champion_System

### Project developed for 'Operative Systems', a second year subject @ISEC

Made in collaboration with another student

### This project was developed in a virtual machine using LINUX (does not work in Windows)

#### Guide:

This project consists of a game championship management system called Champion, where the basic concept of such an ongoing system involves a referee (system administrator), several players (called clients and it is mandatory to be at least 2) and one or more games.<br/>All games are singleplayer, that is, there are no players directly competing against each other.<br/>Communication between games and players is done through the referee. He also has the power to select messages from players and act upon their request, as well as administrator commands.<br/>At the end of the championship, all games are over and each player receives his score. These can then return to play in a new game if they wish.<br/><br/>
You need to lauch the arbitro.c first, and then the client.c. You can modify the duration for the championship and the time left for new clients to connect using arguments when launching arbitro.c (```-dXX``` for the duration of the championship, ```-tYY``` for the time left for new clients to connect) (XX is in minutes, YY is in seconds). If no argument is detected, the system will use a standard time.

#### Commands:

* Arbitro.c:<br/>
```exit``` -> exits the system (all clients are notified)<br/>
```players``` -> lists all players (clients) connected<br/>
```games``` -> lists all games available<br/>
```end``` -> ends championship immediately<br/>
```sXXXX``` -> suspends communication with XXXX (client name) player<br/>
```rXXXX``` -> reactivate communication with XXXX (client name) player<br/>
```kXXXX``` -> kick XXXX (client name) player<br/>
* Client.c:<br/>
```#quit``` -> quit and exits the championship<br/>
```#mygame``` -> game information<br/>
```info``` -> debug command for critical information<br/>

#### Implemented Games:
* g_8_puzzle – Consists of ordering the puzzle numbers from 1 to 8.
* g_divinha_num – Consists of guessing a number, from 0 to 10.
* g_contas – Consists of performing simple arithmetic operations.
* g_translate – It consists of translating words.
<br/><br/>For each correct answer, a point is added.
