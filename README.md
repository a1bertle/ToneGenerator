
## ToneGenerator

Note: The Tone class and all supporting scripts to generate any sound currently assumes the user is running a Linux system that has ALSA support and has [aplay](https://linux.die.net/man/1/aplay) installed. 

### Setup

After you clone the repository, go into the repository folder and run the following commands:
 
 ```console
cd ./scripts
./setup
```

By default, all files resulting from compilation are excluded from the depot. Running ./setup will setup the required directories for the other programs in ./scripts.

### Compilation
To compile, run ``` make ``` from the top folder of the repository.

### Usage

To generate the file of raw audio sample data, run the following commands
 ```console
cd ./scripts
./generateSoundFile
```

To play the raw data with **aplay**, run the following commands:
 ```console
cd ./scripts
./playSound
```
Note that if no raw audio file has been generated, **playSound** script with call **generateSoundFile** to generate it before proceeding to call **aplay**

### Tone

The basis of the Tone class was borrowed from: https://codereview.stackexchange.com/questions/83504/basic-c-tone-generator

All source files are located in ./src/Tone

#### Files:
 - Tone.h and Tone.cpp: Class definition
 - main.cpp: A test program to test the Tone class. This is the main program compiled by "make" and is run by the *generateSoundFile* script



