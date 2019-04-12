# ChessTCP
Client-Server chess game via SFML::Network TCP socket connection

## Project introduction
This is my client-server chess game based on my own (kind off) chess engine. 
Communication model is based on TCP/IP socket connection implemented via SFML Network module.
This project is created as submission for my school programming contest "SCI++".  

# Progress as for 12.04.2019
## Server
Server app is now almost fully functional. Only thing left to do is add more cases to request handling class.

### Features:
#### *Command line interface via which admin can check server details
Admin can use commands such as "players", "games", "debuginfo" in order to see into server runtime details.

#### *Handle multiple clients at once (request handling)
As far as real testing goes in the biggest test yet 2 games were player simultaniously from 4 separate computers with
server instance running on totally seprate machine all in LAN.

#### *Connect clients in pairs on clients request 
Game is handled throught pairing two clients as map entry and sending request between those pairs.

#### *Initialize and handle multiple games
Server can spawn and manage multiple game instances as well as exit (destroy them) after player quits.
For now only way to destroy game instance is for one player to close their client but "disconnect" button is on its way.

### Struggles:
#### *Handling game signals (such as check/checkmate/stalemate)
Its more of a client side struggle but server also needs to terminate game instance when for eg. checkmate occures.

#### *Lack of so called "idiot proofing"


## Client
Client app is at the moment mostly functional. Prototype gui can handle game invites and game initialization.
It also provides all basic information that player needs.

### Features:
#### *Sending and accepting match invites via players ID
There is a text box in which user can input other players ID and send him an invite. There also is panel which
show all pending invites and provides user with accept/decline buttons.

#### *Working game screen
Client allows to play actual game between two players. It highlights all pieces moves after selecting it (there will be an option to disable that feature).
Basic point and click in game controls (Possibly also drag and drop in the future). 

### Struggles:
##### *Lack of many criticall funcionalities such as "unmatch player" or "disconnect" from game screen.



## Other information
### Stuff used
[SFML](https://www.sfml-dev.org/download.php) version 2.5.1 and [TGUI](https://tgui.eu/download/) version 0.8

### To do:
-client GUI application <br>
-client-server communication fixes<br>
-lobbys to setup game

### What i learned
-how TCP/IP works in practice <br>
-how to design basic client-server communication <br>
-working with multiple threads
-handling multiple tcp clients

### Installation
For now you have to clone this repo and compile it yourself.
