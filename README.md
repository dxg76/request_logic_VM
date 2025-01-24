# request_logic_VM
This is the directory for all MRSTV logic 


Dante Gordon dxg2@students.uwf.edu\n
Devan Rivera\n
Repo Creation Date 10/13/24



## VENDING ALGORITHM


### Dante Gordon
#### Overview:
The intention of this program is to run as a beta of the vending logic used for communicating with the customer. 
Currently the program supports parsing and checking strings for a few key words. These key words will be used to navigate a tree data structure representing menus in which to communicate with the machine. These will include a  main menu which will act as the root node of the tree with pointers to each of the submenus. These submenus will include the menus for each type of snack as well as a help menu.

### DEV INFO:
Compile the tokenizer using:
make -f token.mk

run tokenizer:
./main 
for debug mode
./main -d 

### Program History:
11/14/24 
*Dante Gordon*
Basic loop created,
The program requests a string from the command line. and compares each token to certain request words
currently there is an error with the token vector where it the tokens have changed unexpectedly

1/23/25
*Dante Gordon*
Fixed vector error working on implementing menu data structure

## AUDIO

### Dante Gordon
### Overview:

This program is inteded to handle the audio programming necessary for interacting with machine via voice. 
Currently the beta I programmed runs as its own executable and supports the generation of multiple audio files via communication with the command line. 
This must eventually be adapted to handle the initiation of recording, saving of files and transcribing using Whisper.cpp when necessary in our vending logic


### DEV INFO:
*Dante Gordon*
Currently supports single wav file audio generation 
compile with mingw using: 

make -f audio.mk

run with: 
./main

to remove unused files:

make clean -f audio.mk

### Program History:
1/17/25
*Dante Gordon*
Added miniaudio beta

1/23/25
*Dante Gordon*
Multifile added
Needs  Interfacing, and file conversion, adaption to linux and transcribing etc.

## Serial

### Dante Gordon
### Overview:
This program will be intended to initiate a serial connection via usb with the MDB interface. 
It will interface with the vending algorithm and send the correct commands via the serial connection
This must be done for a linux system, complications may occur using a linux subsystem

### Program History:
1/17/25
folder created

MDB device Serial communication information
Baudrate - 115200
Data bits - 8
Parity - none
Stop Bits - 1

Need Linux Machine