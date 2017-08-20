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

// Source Code: https://codereview.stackexchange.com/questions/83504/basic-c-tone-generator

void write_sample(std::ostream &stream, double sample);
double getNoteFreq(double distFromMidC);
int getDistFromKey(string keyChar);

int main(int argc, char *argv[])
{
    constexpr double SAMPLE_RATE = 44100;
	constexpr int BPM = 60;
	
	std::ofstream outfile("./compile/songfile", std::ofstream::binary);
	
	double attack, decay, amplitude;
	decay = 0.05;
	amplitude = 0.5; // Starts clipping around amplitude ~= 1

	// Array of pointers to Tone objects
	Tone *genTones[NUM_VOICES];
	//Tone *freqMod[HUM_VOICES];

	for (int gt = 0; gt < NUM_VOICES; gt++) {
		genTones[gt] = new Tone(SAMPLE_RATE);
	}

	Tone *freqMod[NUM_VOICES];
	for (int fm = 0; fm < NUM_VOICES; fm++) {
		freqMod[fm]= new Tone(SAMPLE_RATE);

	} 
//	string notes1[] = {"C3n"};
//	string notes2[] = {"G3n"};
//	string notes3[] = {"C4n"};
//	string notes4[] = {"E4n"};
//	string notes5[] = {"G4n"};


//	string notes1[] = {"C4n", "D4n", "E4n", "F4n", "G4n", "A4n", "B4n", "C5n"};

	string notes1[] = {"C3s", "C3s", "C3s", "C3s",
					   "D3s", "D3s", "D3s", "D3s", "D3s",
					   "G2s", "G2s", "G2s", "G2s",
					   "F2n", "F2n", "F2n", "F2n", "F2n"};

	string notes2[] = {"G4s", "G4s", "G4s", "G4s",
	 				   "G4s", "G4s", "G4s", "G4s", "G4s",
	 				   "C4n", "C4n", "C4n", "C4n",
	 				   "C4n", "C4n", "C4n", "C4n", "C4n"};
	string notes3[] = {"G4s", "G4s", "G4s", "G4s",
					   "A4s", "A4s", "A4s", "A4s", "A4s",
					   "D4s", "D4s", "D4s", "D4s", 
					   "F4n", "F4n", "F4n", "F4n", "F4n"};
	string notes4[] = {"C5s", "C5s", "C5s", "C5s",
					   "C5s", "C5s", "C5s", "C5s", "C5s",
					   "G4s", "G4s", "G4s", "G4s",
					   "G4s", "G4s", "G4s", "G4s", "G4s"};

	//double dur[] = {1, 1, 1, 1, 1, 1, 1, 1};
	double dur[] = {0.5, 0.5, 0.5, 0.25, 0.5, 0.5, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.25, 0.5, 0.5, 0.25, 0.5, 0.5};

	//double dur[] = {10};

	double genFreq, duration;

	double freq1, freq2, freq3, freq4, freq5;
	double sample1, sample2, sample3, sample4, sample5, totalSample;

	// Generate the required samples
	for (int round = 0; round < 4; round++) {
		for (int index = 0; index < (sizeof(dur)/sizeof(dur[0])); index++) {


			duration = dur[index]/ 1.5;
			attack = 0.01;

			for (int i = 0; i < NUM_VOICES; i++) {
				genTones[i]->reset();
				freqMod[i]->reset();
			}
			

			for (double t = 0; t < (duration + decay); t += 1 / SAMPLE_RATE)
			{

				double strength;

				// Linear attack envelope, Constant until the decay time, then interpolate linearly out
				if  (t < attack)
					strength = t / attack;
				else if (t < duration)
 					strength = 1;
				else
					strength = 1 - (t - (duration)) / decay;

				int dist[] = {getDistFromKey(notes1[index]),
							  getDistFromKey(notes2[index]), 
							  getDistFromKey(notes3[index]),
							  getDistFromKey(notes4[index])};

				// Superimpose the samples from each tone generator
				freq1 = (notes1[index] == "R") ? 0 : (getNoteFreq(dist[0]));// + freqMod[0]->nextSample(2, getNoteFreq(dist[0]));
				sample1 = genTones[0]->nextSample(freq1, strength * amplitude);

				freq2 = (notes2[index] == "R") ? 0 : (getNoteFreq(dist[1]));// + freqMod[1]->nextSample(2, getNoteFreq(dist[1]));
				sample2 = genTones[1]->nextSample(freq2, strength * amplitude);

				freq3 = (notes3[index] == "R") ? 0 : (getNoteFreq(dist[2]));// + freqMod[2]->nextSample(2, getNoteFreq(dist[2]));
				sample3 = genTones[2]->nextSample(freq3, strength * amplitude);

				freq4 = (notes4[index] == "R") ? 0 : (getNoteFreq(dist[3]));// + freqMod[3]->nextSample(2, getNoteFreq(dist[3]));
				sample4 = genTones[3]->nextSample(freq4, strength * amplitude);


				totalSample = (sample1 + sample2 + sample3 + sample4) / NUM_VOICES;
				
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
// C4f -> C4 flat
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
	if 		(orn == 'n') ornAdj = 0;
	else if (orn == 's') ornAdj = 1;
	else if (orn == 'f') ornAdj = -1;
	else {
		printf("Error: invalid ornament!!!\n");
		return 0;
	}

	finalDist = initialDist + plusOct + ornAdj;

	return finalDist;
}

void write_sample(std::ostream &stream, double sample)
{
    	int16_t output = (int16_t) (0x7FFF * sample);
    	stream.write((char *) &output, sizeof(int16_t) / sizeof(char));
}
