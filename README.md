# ChessTCP
Client-Server chess game via SFML::Network TCP socket connection

## Project introduction
This is my client-server chess game based on my own (kind off) chess engine. 
Communication model is based on TCP/IP socket connection implemented via SFML Network module.
This project is created as submission for my school programming contest "SCI++".  

## Progress as for 06.04.2019
### Server can:
-handle multiple clients at once (request handling) <br>
-provide (kind off) chat communication between clients <br>
-connect clients in pairs on clients request <br> 
-spawn game thread on clients request <br>

### Server struggles with:
-handling in game communication <br>
-syncing clients <br>

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
