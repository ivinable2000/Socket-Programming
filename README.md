# Socket-Programming
Using Server-Client interaction to transfer files 
The client will establish a connection with the server and request a file, the server will then check if it has the file, notify the client 
whether it does, and if so - it will send the file contents to the client so that the client can save it under a new filename.

For client.c: The command line arguments are - server IP address in dot notation, server port
number, filename to request from server, new filename to save under on client side

For server.c: The command line argument is just server port number
