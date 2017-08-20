
## ToneGenerator

Note: The Tone class and all supporting scripts to generate any sound currently assumes the user is running a Linux system that has ALSA support and has aplay installed. 

### Setup

After you clone the repository, do the following:
 - cd into the repository
 - cd ./scripts
 - ./setup

By default, all files resulting from compilation are excluded from the depot. Running ./setup will setup the required directories for the other programs in ./scripts.

To compile:
 - Make sure you have make installed
 - Go into the top directory.
 - run make

To generate the file of raw data:
 - cd ./scripts
 - ./generateSoundFile

To play the raw data with aplay:
 - cd ./scripts
 - ./playSound

### Tone

The basis of the Tone class was taken from: https://codereview.stackexchange.com/questions/83504/basic-c-tone-generator

All source files are located in ./src/Tone

#### Files:
 - Tone.h and Tone.cpp: Class definition
 - main.cpp: A test program to test the Tone class. This is the main program compiled by "make" and is run by generateSoundFile script



