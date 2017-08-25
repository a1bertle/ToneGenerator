#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include "Tone.h"
#include <string>

using namespace std;

#define PI 3.14159265
#define MIDDLE_C 261
#define NUM_VOICES 4
#define MIDDLE_OCT 4
#define NOTES_PER_OCTAVE 12
#define MAX_NOTES 128
#define NUM_REPETITIONS 4

// Source Code: https://codereview.stackexchange.com/questions/83504/basic-c-tone-generator

void write_sample(std::ostream &stream, double sample);
double getNoteFreq(double distFromMidC);
int getDistFromKey(string keyChar);

int main(int argc, char *argv[])
{
    constexpr double SAMPLE_RATE = 44100;
	constexpr int BPM = 60;
	
	std::ofstream outfile("./compile/songfile", std::ofstream::binary);
	
	double attack, decay, amplitude, strength;
	decay = 0.05;
	amplitude = 0.5; // Starts clipping around amplitude ~= 1

	// Array of pointers to Tone objects
	Tone *genTones[NUM_VOICES];
	Tone *freqMod[NUM_VOICES];

	for (int gt = 0; gt < NUM_VOICES; gt++) {
		genTones[gt] = new Tone(SAMPLE_RATE);
		freqMod[gt] = new Tone(SAMPLE_RATE);
	}
	
	// Play Viva La Vida
	string voices[NUM_VOICES][18] = {
	{"C3s", "C3s", "C3s", "C3s", "D3s", "D3s", "D3s", "D3s", "D3s", "G2s", "G2s", "G2s", "G2s", "F2n", "F2n", "F2n", "F2n", "F2n"},
	{"G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "C4n", "C4n", "C4n", "C4n", "C4n", "C4n", "C4n", "C4n", "C4n"},
	{"G4s", "G4s", "G4s", "G4s", "A4s", "A4s", "A4s", "A4s", "A4s", "D4s", "D4s", "D4s", "D4s", "F4n", "F4n", "F4n", "F4n", "F4n"},
	{"C5s", "C5s", "C5s", "C5s", "C5s", "C5s", "C5s", "C5s", "C5s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s", "G4s"}};

	double dur[] = {0.5, 0.5, 0.5, 0.25, 0.5, 0.5, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.25, 0.5, 0.5, 0.25, 0.5, 0.5};

	double frequency[NUM_VOICES], sample[NUM_VOICES];
	double duration, totalSample;
	int dist[NUM_VOICES];

	// Generate the required samples
	for (int round = 0; round < NUM_REPETITIONS; round++) {
		for (int index = 0; index < (sizeof(dur)/sizeof(dur[0])); index++) {

			duration = dur[index]/ 1.5; 
			attack = 0.01;
			
			// Reset all Tone objects so sine waves start at time 0
			for (int i = 0; i < NUM_VOICES; i++) {
				genTones[i]->reset();
				freqMod[i]->reset();
			}

			for (double t = 0; t < (duration + decay); t += 1 / SAMPLE_RATE) {

				// Linear attack envelope, Constant until the decay time, then interpolate linearly out
				if  (t < attack)
					strength = t / attack;
				// Maintain a constant envelope for the duration of the note
				else if (t < duration) 
 					strength = 1;
				// Interpolate linearly out for the decay
				else
					strength = 1 - (t - (duration)) / decay;

				// Reset the sample
				totalSample = 0;

				// Go through each voice and generate a total sample by summing up the samples from each voice
				for (int d = 0; d < NUM_VOICES; d++) {
					
					// Calculate the frequency of the given note
					dist[d] = getDistFromKey(voices[d][index]);

					// If there is a rest, set the frequency to 0
					frequency[d] = (voices[d][index] == "R") ? 0 : (getNoteFreq(dist[d]));

					// Grab the next sample
					sample[d] = genTones[d]->nextSample(frequency[d], strength * amplitude);

					// Accumulate the samples for each voice (superposition property)
					totalSample += sample[d];
				}
				
				// Average out the total sample to avoid clipping 
				totalSample /= NUM_VOICES;
			
				// Write the sample to an output file
				if (outfile.good()) {
				    	write_sample(outfile, totalSample);
				}
				else {
			    		return 1;
				}
			}
		}
	}
	return 0;	
}

double getNoteFreq(double distFromMidC) {
	return MIDDLE_C * pow(2, (distFromMidC / NOTES_PER_OCTAVE));
}

// Enter keyChar as a string:
// C4n -> C4
// C4b -> C4 flat
// C4s -> C4 sharp
int getDistFromKey(string keyChar) {
	char key;
	char orn; // ornament
	int oct, initialDist, plusOct, ornAdj, finalDist;

	key = keyChar[0];

	if (keyChar.length() == 3) {
		oct = keyChar[1] - 48; // Need to subtract 48 because ASCII starts from 48
		orn = keyChar[2];
	}

	// Set inital distance from C irrespective of octave
	if 		(key == 'C') initialDist = 0;
	else if (key == 'D') initialDist = 2;
	else if (key == 'E') initialDist = 4;
	else if (key == 'F') initialDist = 5;
	else if (key == 'G') initialDist = 7;
	else if (key == 'A') initialDist = 9;
	else if (key == 'B') initialDist = 11;
	else if (key == 'R') return 0; // Rest
	else {
		printf("Error: invalid key!!!\n");
		return 0;
	}

	// Add octaves
	plusOct = (oct - MIDDLE_OCT) * NOTES_PER_OCTAVE;

	// Adjust for sharps, flats, or neutral
	if      (orn == 'n') ornAdj = 0;
	else if (orn == 's') ornAdj = 1;
	else if (orn == 'b') ornAdj = -1;
	else {
		printf("Error: invalid ornament!!!\n");
		return 0;
	}

	finalDist = initialDist + plusOct + ornAdj;

	return finalDist;
}

void write_sample(std::ostream &stream, double sample) {
	// Limit bits per sample to 16 bits by zeroing out upper 16 bits
    	int16_t output = (int16_t) (0x7FFF * sample);
    	stream.write((char *) &output, sizeof(int16_t) / sizeof(char));
}
