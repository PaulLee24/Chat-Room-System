# Chat-Room-System

This is a project originally for a Network Programming course at NYCU.

## Specification
### Server
- The command to start the server
  ` ./server <SERVER PORT>`

If the number of arguments is not one, the server program will terminate.
The server can serve multiple clients simultaneously. Once a connection is established, the server will send a hello message to the client and give the client a username `anonymous`. The client can send different commands to the server. The messages transmitted between clients and the server are shown below.

#### Hello Message
When a client connects to the server, the server will send a hello message to the client and then broadcasts this user's presence to other clients.

- Client output format
    - To the newly connected client
        `[Server] Hello, anonymous! From: <Client IP>:<Client Port>`
    - To existing clients
        `[Server] Someone is coming!`

---

#### Offline Message
  When a client disconnect from the server, the server will send an offline message to all the other online clients to tell them someone has been offline.

- Client output format
`[Server] <USERNAME> is offline.`

---
#### Who Message
  A client can type command below to list all online users.

- Client input format
`who`

The server will reply to sender a list of online users and tag the sender client. For N user, Server will send N lines. Each of them shows details of a user. 

- Client output format
    - If the user is not the client itself
`[Server] <USERNAME> <CLIENT IP>:<CLIENT PORT>`
    - If the user is the client itself
`[Server] <SENDER USERNAME> <CLIENT IP>:<CLIENT PORT> ->me`

---
#### Change Username Message
A client can type the command below to change his/her username.

- Client input format
`name <NEW USERNAME>`

The server has to verify if the new name is valid which means the input name is 
  1. not anonymous, 
  2. unique, and 
  3. **2~12** English letters.
 
It will reject user's request if this name cannot fit the rule.

- Client output format
    - If the new name is anonymous.
    `[Server] ERROR: Username cannot be anonymous.`
    - If the new name is not unique.
    `[Server] ERROR: <NEW USERNAME> has been used by others.`
    - If the new name does not consist of **2~12** English letters.
    `[Server] ERROR: Username can only consists of 2~12 English letters.`
   
The server will reply some messages to all users once a user changes his/her name.

- Client output format
    - To user which changed his/her name
    `[Server] You're now known as <NEW USERNAME>.`
    - To other users
    `[Server] <OLD USERNAME> is now known as <NEW USERNAME>.`

---
#### Private Message
  A client can send a private message to a specific client.

- Client input format
`tell <USERNAME> <MESSAGE>`

The server will send an error message back to the sender if either the sender's name or the receiver's name is anonymous.

- Client output format
    - If the sender's name is anonymous
    `[Server] ERROR: You are anonymous.`
    - If the receiver's name is anonymous
    `[Server] ERROR: The client to which you sent is anonymous.`
    - If both of the sender and the receiver's names are anonymous
    `[Server] ERROR: You are anonymous.`
    `[Server] ERROR: The client to which you sent is anonymous.`
    - If the receiver doesn't exist
    `[Server] ERROR: The receiver doesn't exist.`

Otherwise, the server sends the private message to the specific client and sends back a notification to the sender.

- Client output format
    - To the sender: If the message is sent
    `[Server] SUCCESS: Your message has been sent.`
    
    - To the receiver: If both client's name are not anonymous
    `[Server] <SENDER USERNAME> tell you <MESSAGE>`
    
Note that if the sender tells a message to himself, `[Server] SUCCESS: Your message has been sent.` should be printed first then followed by `[Server] <SENDER USERNAME> tell you <MESSAGE>`.
   
---
#### Broadcast Message
A client can send a broadcast message to all clients.

- Client input format
`yell <MESSAGE>`
  
While receiving the command from a user, the server will add `<SENDER's USERNAME>` at the head of it and broadcasts to all users including the sender.

- Client output format
`[Server] <SENDER's USERNAME> yell <MESSAGE>`

---
#### Error Command
  Commands which haven't been declared above are invalid commands. When a server receives an invalid command, it should send an error message back to the sending client.

- Client output format
`[Server] ERROR: Error command.`
   
---

### Client

  A client cannot connect to more than one server at the same time.
  Users should give the server IP and the port as the arguments of the client program.
  If the number of arguments is not two, the client program should terminates.
- The command to start the client
  ` ./client <SERVER IP> <SERVER PORT>`

---
#### Exit Command
The user can type the command below to terminate the process at any time.
- Command Format
`exit`

