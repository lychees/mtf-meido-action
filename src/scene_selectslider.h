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

#ifndef EP_SCENE_SELECTSLIDER_H
#define EP_SCENE_SELECTSLIDER_H

// Headers
#include "scene.h"
#include "window_picture.h"
#include "window_help.h"
#include "window_selectslider.h"
#include "configor/json.hpp"
#include <vector>

/**
 * Scene Menu class.
 */
class Scene_SelectSlider : public Scene {
public:
	/**
	 * Constructor.
	 *
	 * @param decision_index selected index in the menu.
	 */
	Scene_SelectSlider(int decision_index = 0, std::vector<configor::json> json = {});

	//void TransitionIn(SceneType prev_scene) override;

	void Start() override;
	//void Continue(SceneType prev_scene) override;
	void vUpdate() override;

	void DrawBackground(Bitmap& dst) override;

	void UpdateIndex();
	/**
	 * Creates the window displaying the options.
	 */
	void CreateSelectSliderWindow();

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
	int item_max;
	int curr_window;

	std::vector<configor::json> json;

	/** Window displaying the help. */
	std::unique_ptr<Window_Help> help_window;

	/** Window displaying pictures. */
	std::unique_ptr<Window_Picture> picture_windows[3];

	std::unique_ptr<Window_SelectSlider> main_windows[3];

	/** Options available in the menu. */
	std::vector<std::string> options;

	std::vector<std::string> discriptions;
	
	std::vector<std::string> pictures;
};

#endif
