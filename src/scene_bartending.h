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

#ifndef EP_SCENE_BARTENDING_H
#define EP_SCENE_BARTENDING_H

// Headers
#include "scene.h"
#include "window_bartending.h"
#include "configor/json.hpp"

/**
 * Scene Menu class.
 */
class Scene_Bartending : public Scene {
public:
	/**
	 * Constructor.
	 *
	 * @param decision_index selected index in the menu.
	 */
	Scene_Bartending(int decision_index = 0, configor::json json = {});

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
	/** Selected index on startup. */
	int decision_index;

	configor::json json;

	/** Window displaying the bartending selections. */
	std::unique_ptr<Window_Bartending> bartending_window;

	/** Window displaying the help. */
	std::unique_ptr<Window_Help> help_window;

	/** Window displaying pictures. */
	std::unique_ptr<Window_Picture> picture_window;

	/** Options available in the menu. */
	std::vector<std::string> bartending_options;
};

#endif
