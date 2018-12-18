# Systems_Work18
Systems Work 18

## Directions:

- Check out the files on https://github.com/mks65/networking
    - Copy them all.
- Fill in the function definitions for server_handshake and client_handshake in pipe_networking.c.
    - These should do everything needed to establish a 3-way handshake between the client and server.
    - make sure you include diagnostic print messages in your functions.
- make and test
- You ***should not*** modify any other files for this assignment.

## Protocol:

1. Setup
    1. Server creates a well known pipe (name is public) and waits for connection.
    2. Client creates a private pipe (use process id for nice uniqueness).
2. Handshake
    1. Client connects to server and send the private pipe name. Client then waits for a response from the server.
    2. Server recives client's message and removes (not closes, since a connection will still be mantained) the well known pipe.
    3. Server connects to client's pipe, sending an ackknowledgement message.
    4. Clients recives server message, and removes (not closes!) its pipe.
    5. Client sends a response message to the server.
3. Operation
    - Server and client send data back and forth. Whatever.
4. Reset
    1. Client exists, server closes any connections to the client.
    2. Server creates a new well known pipe and waits for another client.
