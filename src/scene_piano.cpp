/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <cassert>
#include "Scene_Piano.h"
#include "audio.h"
#include "cache.h"
#include "game_party.h"
#include "game_system.h"
#include "game_variables.h"
#include "input.h"
#include "player.h"
#include "bitmap.h"
#include "feature.h"
#include "main_data.h"
#include "configor/json.hpp"


// piano
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <mmsystem.h>

// Link against the Windows Multimedia library
#pragma comment(lib, "winmm.lib")

// MIDI Status Bytes
const int NOTE_ON = 0x90;
const int NOTE_OFF = 0x80;
const int PROGRAM_CHANGE = 0xC0;

// Global MIDI handle
HMIDIOUT hMidiOut;

// Function to send a short MIDI message
void sendMidiMsg(DWORD msg) {
    midiOutShortMsg(hMidiOut, msg);
}

// Function to play a note
void noteOn(int note, int velocity = 127) {
    // Construct the MIDI message: Status | Note | Velocity
    // Packed into a DWORD: 0x00vvnnss
    DWORD msg = (velocity << 16) | (note << 8) | NOTE_ON;
    sendMidiMsg(msg);
}

// Function to stop a note
void noteOff(int note) {
    DWORD msg = (0 << 16) | (note << 8) | NOTE_ON; // Velocity 0 acts as Note Off
    sendMidiMsg(msg);
}

constexpr int menu_bartending_width = 88;
//constexpr int gold_window_width = 88;
constexpr int menu_help_height = 64;

Scene_Piano::Scene_Piano() {	
	type = Scene::Debug;
}

void Scene_Piano::Start() {
	// 3. Define Key Mappings (Virtual Key Codes -> MIDI Notes)
	// Middle C is 60
	// Lower Octave
	keyMap['Z'] = { 48, false, "C3" };
	keyMap['S'] = { 49, false, "C#3" };
	keyMap['X'] = { 50, false, "D3" };
	keyMap['D'] = { 51, false, "D#3" };
	keyMap['C'] = { 52, false, "E3" };
	keyMap['V'] = { 53, false, "F3" };
	keyMap['G'] = { 54, false, "F#3" };
	keyMap['B'] = { 55, false, "G3" };
	keyMap['H'] = { 56, false, "G#3" };
	keyMap['N'] = { 57, false, "A3" };
	keyMap['J'] = { 58, false, "A#3" };
	keyMap['M'] = { 59, false, "B3" };

	// Middle Octave
	keyMap[VK_OEM_COMMA] = { 60, false, "C4" }; // ,
	keyMap['L'] = { 61, false, "C#4" };
	keyMap[VK_OEM_PERIOD] = { 62, false, "D4" }; // .
	keyMap[VK_OEM_1] = { 63, false, "D#4" };     // ;
	keyMap[VK_OEM_2] = { 64, false, "E4" };      // /
	
	// Alternative Upper Row mapping (More standard for typing)
	keyMap['Q'] = { 60, false, "C4" };
	keyMap['2'] = { 61, false, "C#4" };
	keyMap['W'] = { 62, false, "D4" };
	keyMap['3'] = { 63, false, "D#4" };
	keyMap['E'] = { 64, false, "E4" };
	keyMap['R'] = { 65, false, "F4" };
	keyMap['5'] = { 66, false, "F#4" };
	keyMap['T'] = { 67, false, "G4" };
	keyMap['6'] = { 68, false, "G#4" };
	keyMap['Y'] = { 69, false, "A4" };
	keyMap['7'] = { 70, false, "A#4" };
	keyMap['U'] = { 71, false, "B4" };
	keyMap['I'] = { 72, false, "C5" };
	keyMap['9'] = { 73, false, "C#5" };
	keyMap['O'] = { 74, false, "D5" };
	keyMap['0'] = { 75, false, "D#5" };
	keyMap['P'] = { 76, false, "E5" };
	MMRESULT result = midiOutOpen(&hMidiOut, 0, 0, 0, CALLBACK_NULL);
	sendMidiMsg(PROGRAM_CHANGE | 0);	
}

//void Scene_Piano::Continue(SceneType /* prev_scene */) {
//	menustatus_window->Refresh();
//	gold_window->Refresh();
//}


void Scene_Piano::vUpdate() {	
	UpdateCommand();	
}

void Scene_Piano::UpdateCommand() {

	if (Input::IsTriggered(Input::CANCEL)) {
		midiOutClose(hMidiOut);
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		Scene::Pop();
	} else {
		// 1. Open the default MIDI Output device
		// MMRESULT result = midiOutOpen(&hMidiOut, 0, 0, 0, CALLBACK_NULL);
		// if (result != MMSYSERR_NOERROR) {
			// std::cerr << "Error opening MIDI output device!" << std::endl;
			// return;
		// }

		// 2. Set Instrument to Acoustic Grand Piano (Program 0)
		// Channel 0, Program 0
		/*std::cout << "==========================================" << std::endl;
		std::cout << "      C++ Console Piano (MIDI)            " << std::endl;
		std::cout << "==========================================" << std::endl;
		std::cout << "Mapping:" << std::endl;
		std::cout << "   [2] [3]     [5] [6] [7]     [9] [0]    " << std::endl;
		std::cout << "[Q] [W] [E] [R] [T] [Y] [U] [I] [O] [P]   " << std::endl;
		std::cout << " C   D   E   F   G   A   B   C   D   E    " << std::endl;
		std::cout << "==========================================" << std::endl;
		std::cout << "Also supports lower octave on Z-M keys." << std::endl;
		std::cout << "Press ESC to quit." << std::endl;*/
		
		// Iterate through all mapped keys
		for (auto &pair : keyMap) {
			int vKey = pair.first;
			KeyState &state = pair.second;

			// Check high bit of return value (indicates key is currently down)
			bool isDown = (GetAsyncKeyState(vKey) & 0x8000) != 0;

			if (isDown && !state.isPressed) {
				// Key just pressed
				noteOn(state.midiNote);
				state.isPressed = true;
				// std::cout << "Note On: " << state.name << std::endl; // Optional logging
			}
			else if (!isDown && state.isPressed) {
				// Key just released
				noteOff(state.midiNote);
				state.isPressed = false;
			}
		}		
	}
}