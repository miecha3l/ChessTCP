# VitaChess
VitaChess is simple chess platform that includes server app and its own
chess client app. Platform is based on TCP connection handled by SFML Network module. 
Client and server have their own custom communication protocol. Vita chess enable playing
online vs friends as well as solo offline.

## Project introduction
This is my client-server chess game based on my own (kind off) chess engine. 
Communication model is based on TCP socket connection implemented via SFML Network module.
This project is created as submission for my school programming contest "SCI++".  




# Progress as for 22.04.2019
## Server
Server app is now almost fully functional and stable. Only thing left to do is add more cases to request handling class.

### Features:
#### *Command line interface via which admin can check server details
Admin can use commands such as "players", "games", "debuginfo" in order to see into server runtime details.

#### *Handle multiple clients at once (request handling)
As far as real testing goes in the biggest test yet 2 games were player simultaniously from 4 separate computers with
server instance running on totally seprate machine all in LAN.

#### *Connect clients in pairs on clients request & disconnecting existing pairs 
Game is handled throught pairing two clients as map entry and sending request between those pairs.
Such entry can be erased on clients request.

#### *Initialize and handle multiple games
Server can spawn and manage multiple game instances as well as exit (destroy them) after player quits.
Player can quit via in game "leave" button or by closing application.






## Client
Client app is at the moment mostly functional. Refreshed and implemented probably final UI look. Got rid of most of the bugs.
Client is now close to stable 1.0 version (needs far more testing).

### Features:
#### *Sending and accepting match invites via players ID
There is a text box in which user can input other players ID and send him an invite. There also is panel which
show latest pending invite and provides user with accept/decline buttons.

#### *Working game screen
Client allows to play actual game between two players. It highlights all pieces moves after selecting it (there will be an option to disable that feature).
Basic point and click in game controls (Possibly also drag and drop in the future). 

#### *Disconnectiong from game, unmatching
Client provides in game disconnect button (as well as forfeit button but funcionality of that is to be added soon). In lobby screen
there is also button to kick matched player.


### Struggles:
##### *Unexpected random crashes
While tested on my machine (old intel pentium laptop, 2 clients and server running at once) client sometimes unexpectedly crashed. Sometimes threw
read violation exception, sometimes just crashed without any particular reason. After fixing read violation and some testing on better machine I assume
that this is the case of my weak processor.




## Other information
### Stuff used
[SFML](https://www.sfml-dev.org/download.php) version 2.5.1 and [TGUI](https://tgui.eu/download/) version 0.8

### To do:
-Bug fixing <br>
-Chess bot algorithm <br>

### I have learned:
-how TCP/IP works in practice <br>
-how to design basic client-server communication <br>
-working with multiple threads <br>
-how NOT TO design this kind of solution <br>


### Installation

#### Windows 
(Download link soon)

#### Linux
Tested only on linux mint. Only server available. <br>

Clone repo:
```bash
git clone https://github.com/miecha3l/VitaChess/
cd VitaChess && mkdir build
```

Install sfml:
```bash
sudo apt-get install libsfml-dev
```

Build server:
```bash
g++ -o build/server chess-server/*.cpp -lsfml-network -lsfml-system -lsfml-window -lsfml-audio -lsfml-graphics
```
