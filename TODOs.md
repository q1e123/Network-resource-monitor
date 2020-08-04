# Client
## Service
Make the client to run in the background as a service.
You should be able to change from running in backgroud to running the client with a GUI and reverse.

## Admin interface
Separate the user and the admin interfaces.  
How to add admins? (Maybe let server decide)  

### Real time tracking of a single system
Administrator should be able to track one single system in real time.

### Compare a system with a previous state
Administrator should be able to see the diferences between the current and the previus states of the machine (Maybe use the data from the plots)

## Get more data

### Collect
The client should be able to also collect:
* storage
* users
* installed software
* last logins
* environment variables
* Hardware and software errors
* IP (IPv4 and IPv6)
### Checks
The client should be able to check:
* if a software is installed
* what patches/updates are installed

## Send reports when the client wants
You should be able to schedule when to send some reports to the server

# Server

## Sanity checks
Server should do sanity checks for the received data.

## Saving data
Server should save the recived data. (maybe use some ini files?)

## Request data from a client
Server should be able to request a report from a client.

# Networking
## Login
The mechanism for login should be:  
1. Client sends the user (maybe also a password)
1. Server responds with:
    * OK if login is successful
    * OK_ADMIN if the user is also an admin
    * RETRY if login wasn't successful
1. If login was not successful the GUI should show a messsage and the user should be able to try again to login
1. If login was successful the GUI should change show the network (name, maybe other users without data)
    * if user is also an admin the GUI should show more data

## Send text files
You should be able to send text file thourgh the network for sending bigger reports.  
Use a buffer.

## Commands
The networking is done by sending and reciving commands from another system.
The general template for a command should be:
```
TYPE;ADDITIONAL_DATA
```
## Template for commands
Description  

**Sent by:**    
**Recived by:**  
**Template:**  
```

```
**Response:**  

## Send system data
Sends data about a system

**Sent by:** Client  
**Recived by:** Server  
**Template:** 
```
SYS;USER;OS;TOTAL_RAM;AVALABILE_RAM;USED_RAM;CPU_0:USAGE-CPU_N:USAGE 
``` 
**Responses:**  
* **OK** if it passed sanity checks  
* **SANITY_ERROR** if it failed sanity checks  
* **CORRUPT** if the reciveed data was corrupted  

## Request system data
Request data about a system

**Sent by:** Server  
**Recived by:** Client  
**Template:** 
```
REQ;DATA  
```
**Responses:**  
* The client sends system data

# Extra
Do this only if you've finished all the others
## Mobile app
## Web app