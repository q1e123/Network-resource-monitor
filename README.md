# Proiect pentru practica
Acesta este repository-ul pentru proiectul de la practica.

# Biblioteci
* [wxWidgets 3.1.3](https://www.wxwidgets.org/downloads/)
* wxMathPlot (este inclusa in proiect pentru ca a trebuit sa fac mici modificari pentru compatibilitate)

# Implementari
* Detectarea sistemului de operare
* Detectare ram (total, folositi, liberi)
* Detectarea si reprezentarea grafica in timp real a utilizarii procesorului, a memoriei ram si a unei interfete de retea
* Detectarea rx si tx a unei interfete de retea 
* Listarea proceselor impreuna cu PID, consum CPU si RAM
* Transmiterea datelor prin retele folosind socket-uri folosind un server pe post de centralizator (aplicatia care ruelaza pe server nu are GUI)
	* Utilizare fiecare CPU
	* Ram
		* Total
		* Folosit
		* Valabil
	* OS
	* User
* Afisarea acestor date pe fiecare client conectat

# Rulare
## Client
Se compileaza programul si dupa se foloseste executabilul.
## Linux
Se poate compila folosind make.

```
make all
./bin/client
```
## Windows
Se poate compila folosind Visual Studio si dupa se poate folosi executabilul generat. [Utilizare wxWidgets in VS](https://www.youtube.com/watch?v=sRhoZcNpMb4)

**Declarati urmatoarele definitii pentru preprocese din *Project settings*:**
* **_WINSOCK_DEPRECATED_NO_WARNINGS** (pentru folosirea functilor care sunt asemanatoare cu functiile unix)
* **_WINSOCKAPI** (pentru folosirea winsock2)

## Server
Se compileaza programul in mod asemanator si dupa se foloseste executabilul dintr-un terminal:
### Linux
```
./bin/server nume_server port
```
### Windows
```
server.exe nume_server port
```