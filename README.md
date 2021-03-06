# VitaChess
VitaChess is simple chess platform that includes server app and its own
chess client app. Platform is based on TCP connection handled by SFML Network module. 
Client and server have their own custom communication protocol. Vita chess enable playing
online vs friends as well as solo offline.

## Project introduction
This is my client-server chess game based on my own (kind off) chess engine. 
Communication model is based on TCP socket connection implemented via SFML Network module.
This project is created as submission for my school programming contest "SCI++". 
As for 1.05.2019 project is closed and i don't plan on further development. 



## Server

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





## Other information
### Stuff used
[SFML](https://www.sfml-dev.org/download.php) version 2.5.1 and [TGUI](https://tgui.eu/download/) version 0.8

### To do:
Nothing, project closed.

### I have learned:
-how TCP/IP works in practice <br>
-how to design basic client-server communication <br>
-working with multiple threads <br>



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

Run:
```bash
cd build && ./server
```
