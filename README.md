# Network resource monitoring tool
This is a cross-platform tool that can help you monitor a network's resources.  
Project made during Siemens internship.

# 3rd party libraries
* [wxWidgets 3.1.3](https://www.wxwidgets.org/downloads/)
* wxMathPlot (it's included with the project since I needed to do small changes to solve compatibility issues)
* SimpleIni (it's included with the project)
* [MySQL connector](https://www.mysql.com/products/connector/)
* [SOCI](https://github.com/SOCI/soci)

# Features
* OS detection
* RAM detection (total, avalabile, used)
* CPU usage detection (all cores)
* rx and tx detection for a network interface
* Real time plotting of CPU, RAM and network usage
* Running processes detection with their PID and CPU and RAM usage 
* Transmition of data through network using sockets
	* CPU usage
	* RAM
		* total
		* used
		* avalabile
	* OS
	* User
* Displaying this data on the client GUI

## New features
For a list with features that I am currently working on you can check [TODOs file](TODOs.md)

# Compilation
You can either compile client and server at the same time or each one at a time by using their CMake files.

## Prerequisites
You need to have installed wxWidgets library.  
[Compioling wxWidgets](https://wiki.wxwidgets.org/Compiling_and_getting_started)  
[How to setup wxWidgets in Visual Studio](https://www.youtube.com/watch?v=sRhoZcNpMb4)  

## Linux
You can use the make to compile it.
```
make all
```

If you don't want to use that Makefile you can also do:
```
mkdir build
cd build
cmake ..
make all
```
## Windows
You can compiling using Visual Studio.

**Define next in *Project settings*:**
* **_WINSOCK_DEPRECATED_NO_WARNINGS** (using the functions that are alike the ones in unix)
* **_WINSOCKAPI** (using winsock2)

# Usage
## Client
### Linux and Windows
Use the binary file that resulted from compilation

## Server
### Setup ini file
Go to [database_template.ini](Init/database_template.ini) and add the database type, it's name, user and password.  
If it's the first time you run this change all keys from create to **on** so the tables will be created. if it's not the first time change it to **off**.
### Linux
```
./bin/server <instance_name> <port>
```
### Windows
```
server.exe <instance_name> <port>
```