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
* Real time tracking of a system
* User roles (normal and administrator)
* Adding new users and systems or changing cuurent data via admin interface
* Client can create local usage data log and send it after to the server to put it in the database
* Storing usage data in a database

## New features
For a list with features that I am currently working on you can check [TODOs file](TODOs.md)

# Compilation
You can either compile client and server at the same time or each one at a time by using their CMake files.

## Prerequisites
You need to have installed wxWidgets library, SOCI and the database you want to use.  
[Compiling wxWidgets](https://wiki.wxwidgets.org/Compiling_and_getting_started)  
[How to setup wxWidgets in Visual Studio](https://www.youtube.com/watch?v=sRhoZcNpMb4)  

You can compile the project using cmake.
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
You can compile it using Visual Studio.

### Propriety sheet
Only for client.
Go to *Propriety manager - Add existing propriety sheet* and select the propriety sheet that is in the root directory of wxWidgets. (wxwidgets.props)

### Windows SDK Version
Go to *Propriety manager - General* and change Windows SDK Version to 10.0 (latest installed version)

### Build mode
Compile it as Release (wxMathPlot does not have support for Debug)

### SOCI
Only for server.
Go to *Propriety manager - VC++ Directories*.  
Add at **include directories** SOCI's include directory.  
Add at **Library directories** SOCI's library directory.

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