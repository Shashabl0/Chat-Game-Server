# Chat&Game Server using TCP in C programming language

A simple Server-Client chat application using TCP protocol and multithreading. Here multiple clients can connect to the server. They can LIST connected clients, SEND msgs to client, and can PLAY Tic-Tac-Toe. Further a client can deny if someone wants to send msgs<br>

Available commands:<br>
* LIST - List all connected clients<br>
* SEND - Send a message to a client<br>
* PLAY - Play Tic-Tac-Toe with a client<br>
* QUIT - Exit the program<br>
* SELF - Show self ID<br>

PS: This is a simple chat application, so it does not have any security features. Further I have used _wsl_ to run the server and client on my Windows machine.

![Preview1](https://drive.google.com/uc?export=view&id=1VfYRvYsyZtiCZNBJSfwkPSrCCNixw7X7)


Use Makefile to compile the program.</br>
* make server --- Compile the server program.
* make client --- Compile the client program.
* make update --- Compile both server and client.