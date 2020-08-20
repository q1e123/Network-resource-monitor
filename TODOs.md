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

### Add more clients/ promote admins
Administrators should be able to promote normal users to admins

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

## Request data from a client
Server should be able to request a report from a client.

# Networking

## Commands
The networking is done by sending and reciving commands from another system.
The general template for a command should be:
```
TYPE;ADDITIONAL_DATA
```
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