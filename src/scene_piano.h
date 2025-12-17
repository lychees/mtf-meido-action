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

#ifndef EP_SCENE_PIANO_H
#define EP_SCENE_PIANO_H

// Headers
#include "scene.h"
#include <map>

// Structure to track key state
struct KeyState {
    int midiNote;
    bool isPressed;
    std::string name;
};

/**
 * Scene Menu class.
 */
class Scene_Piano : public Scene {
public:
	/**
	 * Constructor.
	 *
	 * @param decision_index selected index in the menu.
	 */
	Scene_Piano();

	void Start() override;
	//void Continue(SceneType prev_scene) override;
	void vUpdate() override;

	/**
	 * Creates the window displaying the options.
	 */
	void CreateBartendingWindow();

	/**
	 * Update function if command window is active.
	 */
	void UpdateCommand();

	/**
	 * Update function if status window is active.
	 */
	//void UpdateActorSelection();

	/** Options available in a Rpg2k3 menu. */
	enum CommandOptionType {
		Item = 1,
		Skill,
		Equipment,
		Save,
		Status,
		Row,
		Order,
		Wait,
		Quit,
		// EasyRPG extra
		Debug = 100,
		Settings = 101,
	};

private:
	std::map<int, KeyState> keyMap;
};

#endif
