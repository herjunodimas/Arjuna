/**
 * Arjuna: Alat Bantu Pembelajaran Piano untuk Tunanetra
 *
 * Developed by:
 * Ilham Imaduddin
 * Ahmad Shalahuddin
 * Piquitha Della Audyna
 *
 * Elektronika dan Instrumentasi
 * Universitas Gadjah Mada
 * 
 * Copyright (c) 2015 Arjuna
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#ifndef _ARJUNA_H_
#define _ARJUNA_H_

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "Setup.h"
#include "MidiFile.h"
#include "FingerData.h"

#define 	SELECT_SONG_BUTTON	'A'
#define		PLAY_SONG_BUTTON	'B'
#define		EVALUATOR_BUTTON	'C'
#define 	STOP_BUTTON			'D'

#define		BOTH_HANDS_MODE_BUTTON	'1'
#define		RIGHT_HAND_MODE_BUTTON	'2'
#define 	LEFT_HAND_MODE_BUTTON 	'3'

struct Key
{
	int track;
	unsigned char note;
	unsigned char finger;
};

enum PlayMode {BOTH_HANDS, LEFT_HAND, RIGHT_HAND};
enum MPUOperation {PLAYER, EVALUATOR};

/**
 * Main Function
 *
 * This is the starting point of the program.
 * 
 * @param  argc arg count
 * @param  argv arg vector
 * @return      status
 */
int main(int argc, char *argv[]);

/**
 * Start Application Routine
 *
 * This routine start by showing main menu. It will handle user input
 * and call other methods depending on the input.
 */
void startRoutine(Container *container);

/**
 * Show Application Menu
 *
 * In this screen, user can select the application operation.
 */
void showMenu(void);

/**
 * Song Selector
 *
 * This method will start the song selector.
 * 
 * @param container hardware handler
 */
std::string songSelector(Container *container);

/**
 * Print Song List
 *
 * This method will parse every line of the file handler and print it
 * to stdout
 * 
 * @param songList [description]
 */
void printSongList(std::ifstream *songList);

/**
 * Select Song
 *
 * This method will ask the user to choose from opened song list
 * 
 * @param  songList song list handler
 * @return          song name
 */
std::string selectSong(Container *container, std::ifstream *songList);

/* Start MIDI Processing Algorithm
 *
 * This function is a bootstrap for the MIDI Processing Algorithm.
 * It will call the player or the evaluator, depending on user selection
 * 
 * @param container hadrware handler
 * @param songPath  selected song path
 * @param mode      MPA operation mode
 */
void startMPA(Container *container, std::string songPath, MPUOperation operation);

/**
 * Song Player
 *
 * This method is used to play MIDI song. It will open the MIDI file,
 * calculate some numbers, and send MIDI message to output port
 * 
 * @param  container hardware handler
 * @param  midi 	 MIDI file handler
 * @param  finger 	 finger data handler
 * @param  mode 	 selected play mode
 */
void play(Container *container, MidiFile *midi, FingerData *finger, PlayMode mode);

/**
 * Song Evaluator
 *
 * This function is used to compare MIDI input with MIDI data and give
 * response to hands odule
 * @param container handware handler
 * @param midi      MIDI file handler
 * @param finger    finger data handler
 * @param mode      selected play mode
 */
void evaluate(Container *container, MidiFile *midi, FingerData *finger, PlayMode mode);

/**
 * Get Unison Note
 *
 * This function groups note played at the same time
 * 
 * @param  midi MIDI file handler
 * @param  m    MIDI file index
 * @param  t    MIDI track number
 * @param  keys Keys container
 * @return      status
 */
bool getUnisonNote(MidiFile *midi, int *m, int t, std::vector<Key> *keys);

/**
 * Get Unison Finger
 * This function groups finger played at the same time
 * 
 * @param finger finger data handler
 * @param f      finger data index
 * @param keys   Keys container
 */
void getUnisonFinger(FingerData *finger, std::vector<char> *f, std::vector<Key> *keys);

/**
 * Get MIDI Input
 * 
 * @param io       MIDI IO handler
 * @param expected number of expected input
 */
void getInputAndEvaluate(Container *container, std::vector<Key> keys, char *keypress, MidiFile *midi, int m, int t);

/**
 * Compare MIDI Input with MIDI Data
 *
 * @param  rf 	Trasceiver handler
 * @param  keys MIDI Data
 * @param  note MIDI Input
 * @return      Compare result
 */
bool compare(ORF24 *rf, std::vector<Key> *keys, unsigned char note);

/**
 * Get Play Mode
 *
 * This function ask the user to select the play mode
 * 
 * @param  keypad 	keypad handler
 * @return          play mode
 */
PlayMode getPlayMode(WiringPiKeypad *keypad);

/**
 * Set Play Mode
 *
 * This function prepares the MIDI file to play in right, left, or both hand modes
 * 
 * @param midi MIDI object
 * @param mode Chosen mode
 */
void setPlayMode(MidiFile *midi, PlayMode mode);

/**
 * Get Tempo in Second Per Tick
 * 	
 * @param e   	MidiEvent
 * @param tpq 	tick peq quarter
 * @param spt 	second per tick
 */
void getTempoSPT(MidiEvent e, int tpq, double *spt);

/**
 * Send MIDI Message
 *
 * This method will form a MIDI message container and send it to MIDI output port
 * 
 * @param io MIDI i/O port
 * @param e  MIDI event
 */
void sendMidiMessage(MidiIO *io, MidiEvent e);

/**
 * Send Feedback to Hand Module
 *
 * This method use the radio transceiver to send payload to hand module
 * 
 * @param rf 		radio handler
 * @param f  		finger data
 * @param t  		active track
 * @param right 	turn on right vibrator. If false, then turn on left vibrator
 */
void sendFeedback(ORF24 *rf, char f, int t, bool right);

/**
 * Inverse Finger Number
 *
 * Command for right hand module need to be inverted before sending.
 * This function will do the inversion.
 * 
 * @param  finger finger number
 * @return        inverted finger number
 */
unsigned char inverse(unsigned char finger);

/**
 * Keypad Handler
 *
 * This function should be called in separate thread to wair for user action
 * while doing other operation
 * 
 * @param keypad     keypad handler
 * @param keypress   keypress handler
 * @param terminator keypad terminator
 */
void keypadHandler(WiringPiKeypad *keypad, char *keypress, bool *terminator);

#endif