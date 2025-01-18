# EE533_lab1
EE533 Lab 1: Communication Between Server and Client with Sockets

*** Folder descriptions ***

original_serverclient: Code given in the lab. The only changes were that libraries were added to allow compiling. Given code could not compile due to missing libraries.

enhanced_serverclient: Edited original server code to allow server to accept multiple connections. The client code in this folder is the same as the original client code. Also resolved zombie problem by adding "signal" library and using "signal(SIGCHLD, SIG_IGN);". This involved editing only the server file (the original client file remained untouched).

indefinite_enhanced_serverclient: Edited code from enhanced_serverclient to allow the server to maintain connection with multiple clients, even after a client sends a message. Termination of a connection between the server and a client only occurs when the client sends the termination keyword "exit". This involved editing both the server and client files.

udp: Edited enhanced_serverclient files (both server and client) to use UDP Protocol instead of TCP Protocol.

unix: Unix server and client files provided in the lab.
