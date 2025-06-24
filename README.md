 # request_logic_VM
---
This Repository contains all the source code for the MRSTV talking vending machine project. MRSTV stands for Machine Raspberry Pi Snack Talking Vendor. It is a VMC program capable of addressing all of the typical vending machine functions. What makes our projects special is the additional feature allowing users to select snacks within the vending machine through audio interaction. Similar to a cellular automative assistant, the user talks to the vending machine, and the vending machine seeks key words to navigate the vending options and make a selection.

For this project we used a Raspberry Pi 5 to run the vendor program. The RPI uses its on board GPIO pins to communicate with the keypad, and a arduino mega 2560. The mega2560 handled control of the motorized coils within the vending machine. The main program runs on the RPI. The RPI connects via USB to an MDB-USB interface which connects to the payment peripherals within the vending machine allowing serial communication. For more details see the attached files in the project_details repository.

 Repo Creation Date 10/13/2024  
 Project Completion Date 4/14/2025

## Contributors
Dante Gordon dxg2@students.uwf.edu  
Devan Rivera dmr76@students.uwf.edu




## VENDING_LOGIC_PI  
### Overview:  
The Vending Logic Directory Contains 
This Directory contains the source code to run the vending machine controller program on the Raspberry Pi 5 
It also contains the cpp port of OpenAI's Whisper transcription software https://github.com/ggml-org/whisper.cpp as well as a directory containing the responses the vending machine will  
respond with during different user interactions

### Dev Info:
To compile the VMC program use:
make -f vendor.mk

Since the vendor now includes whisper to access the whisper library you must run this command before running the program:  
export LD_LIBRARY_PATH=./whisper.cpp-master/build/src:$LD_LIBRARY_PATH  

To run the program you may use
./main
the following flags are applicable
./main -d no_charge dev
-d  debug mode  
no_charge bypasses payment stage in vending machine  
dev allows command line input to be used instead of voice for faster debugging  


### Program History:
- 11/14/24 
*Dante Gordon*:
Basic loop created,
The program requests a string from the command line. and compares each token to certain request words
currently there is an error with the token vector where it the tokens have changed unexpectedly  

- 1/23/25  
*Dante Gordon*:
Fixed vector error working on implementing menu data structure

- 1/24/25
*Dante Gordon*:  
Started header for tree, and node classes
Added plaintext tree config plan

- 1/25/25
*Devan Rivera*:
Worked on Node class (node.hpp)
Worked on Tree class (menu-tree.hpp)

- 1/28/25
*Devan Rivera*:
Worked on Node class (node.cpp)
Worked on Tree class (menu-tree.cpp)

- 2/2/25 
*Devan Rivera*:
Implemented Tree manipulation functions 
Fixed general errors within Node class

- 2/3/25
*Devan Rivera*:
Began creating Main menu to take in cin to travel the tree
Fixed general errors within Tree class
Updated Makefile to accomodate for the Tree and Node class
Updated main.cpp to accomodate for the Main Menu

- 2/4/25
*Devan Rivera*
Identified Makefile Errors
Began Debugging of Main Menu 
Corrected Minor Errors in Main Driver

- 2/6/25
*Devan Rivera*
Fixed Clean Rule in Makefile
Finsished Most Debugging of Main Menu

- 2/7/25
*Dante Gordon*:  
Tokenizer patch for unpredictable output  

- 2/8/25
*Dante Gordon*: 
Patched bugs, completed vending selection sequence 
Unified menu tree with vendor 
Still need dispense, Pay, Audio

- 2/17/25
*Devan Rivera*
Removed unecessary functionality
Began Dispense and Pay functions
Updated helper functions for the node class

- 2/24/25
*Dante Gordon*:  
Integrated transcription instead of command line prompts.  
Removed namespaces and adjusted product names to better align  
with whispers output.

- 3/2/25
*Devan Rivera*:
Began implementation of decoder device to play .wav files
Researched into whispers decoder capabilties

- 3/2/25  
*Dante Gordon*:
Implemented functions to enable and configure the MDB payment peripherals

- 3/4/25
*Devan Rivera*:
Implemented more of the decoder and added a function to play the .wav files
Added "wav files" folder into "vending_logic" to reach the audio files

- 3/5/25
*Devan Rivera*:
Managed to compile decoder device (Unable to test locally currently)
Began implementing audio playback logic down tree structure
Added All class to Tree Data Structure 

- 3/9/25
*Devan Rivera*:
Added aditional audio logic for statements to be played outside of the tree
Debugged audio logic
Needs testing with a speaker to ensure audio output

- 3/10/25
*Devan Rivera*:
Successful compilation of decoder logic 
Needs further implementation of length to play audio files to completion

- 3/22/25
*Devan Rivera*
Created logic for quantity control within the vending algorithm
Successful testing in seperate file to ensure smooth implementation

- 3/29/25
*Devan Rivera*
Began Implementing "Boogie" function
Cleaned up code of unecessary comments

## CAPTURE

#### Dante Gordon
### Overview:

This program is inteded to handle the audio programming necessary for interacting with machine via voice. 
Currently the beta I programmed runs as its own executable and supports the generation of multiple audio files via communication with the command line. 
This must eventually be adapted to handle the initiation of recording, saving of files and transcribing using Whisper.cpp when necessary in our vending logic


### Dev Info:
*Dante Gordon*:
compile with mingw using: 

make -f audio.mk

run with: 
./main

to remove unused files:

make clean -f audio.mk

### Program History:
- 1/17/25
*Dante Gordon*:
Added miniaudio beta

- 1/23/25
*Dante Gordon*:
Multifile added
Needs  Interfacing, and file conversion, adaption to linux and transcribing etc.

- 1/25/25
*Dante Gordon*:  
Adjusted format to be compatible with whisper.cpp. Used a file generated by audio_test to be transcribed by whisper  
It worked well, the total time for whisper to process one 18 sec query was 1.47 serconds. More data 
- 2/5/25
*Dante Gordon*:  
Audio buffer created. Supports a 10 file buffer of 5 second clips, upon reaching the 10th file the buffer overrides  
older files starting from the first file. 

- 2/6/25
*Devan Rivera*
Researched Arguments and Methods to use in whisper.cpp

- 2/17/25
*Dante Gordon*:  
Fully integrated with whisper. section closed  

## SERIAL

#### Dante Gordon
### Overview:
This program will be intended to initiate a serial connection via usb with the MDB interface. 
It will interface with the vending algorithm and send the correct commands via the serial connection
This must be done for a linux system, complications may occur using a linux subsystem


### Dev Info:  
MDB device Serial communication information
Baudrate - 115200
Data bits - 8
Parity - none
Stop Bits - 1

Need Linux Machine
### Program History:
- 1/17/25
*Dante Gordon*:
folder created



- 1/30/25 
*Dante Gordon*:  
Fleshing out and annotating serial connection using POSIX commands

- 2/17/25
*Devan Rivera*:
Researched possible MDB commands to be passed through qibbix to the card reader

- 2/18/25
*Devan Rivera*:
Determined appropriate command format through Generic Master
Began Setup/Config Process Commands

- 2/23/25
*Devan Rivera*:
Setup/Config Process Commands
Vend Process Commands

- 2/24/25
*Dante Gordon*: 
Crypto Key error fixed
Full vend communication testing can begin 

## Whisper
### Overview
Capture and transcription program is written.  
Upon running the program the users voice is recorded and echoed to the command line.  
Clip sizes are adjustable but are currently set to 5 seconds. This will be tweaked as we
go on the process of unifying this program and the main program has begun. 
### Dev Info

compile: 

cd whisper.cpp-master
make base.en
cd ..
make voice_assistant -f whisper.mk 

pre-run:  
export LD_LIBRARY_PATH=./whisper.cpp-master/build/src:$LD_LIBRARY_PATH

*WIP*:  
Potential removal of the former:  
echo 'export LD_LIBRARY_PATH=./whisper.cpp-master/build/src:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc  

run:  
./voice_assistant

### Program History
- 2/7/25
*Dante Gordon*:  
working on initializing whisper. Needs more dependencies I think  

- 2/8/25
*Dante Gordon*:  
Building steps complete working on programming live audio transcription

- 2/9/25
*Dante Gordon*:  
Transcription complete whisper stores transcription in char* segments  

- 2/12/26
*Dante Gordon*:  
Begin unifying capture and transcription  
Convert filename vector to linked list Where the capture adds new nodes and follows  
the tail of the linked list and the transcriber reads the file at the head before deleting  
that node from the list  

- 2/17/26
*Dante Gordon*:  
Fully unified capture and transcription

- 2/24/25
*Dante Gordon*:  
Unified with main vending logic  
Program history will remain open to address changes in latency.  
Strategy to try to reduce this is padding the audio files allowing the recordings to be done faster  
