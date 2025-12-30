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
#include "scene_selectslider.h"
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

//constexpr int menu_selectslider_width = 88;
//constexpr int gold_window_width = 88;
constexpr int menu_help_height = 64;

Scene_SelectSlider::Scene_SelectSlider(int decision_index, configor::json json) :
	decision_index(decision_index), json(json) {
	type = Scene::SelectSlider;
}

void Scene_SelectSlider::Start() {
	CreateSelectSliderWindow();
}

void Scene_SelectSlider::DrawBackground(Bitmap& dst) {
    return;
}

void Scene_SelectSlider::vUpdate() {
	//help_window->Update();
	main_windows[0]->Update();
	main_windows[1]->Update();
	main_windows[2]->Update();
	picture_windows[0]->Update();
	picture_windows[1]->Update();
	picture_windows[2]->Update();
	if (main_windows[0]->GetActive() && !main_windows[0]->IsMovementActive() && main_windows[1]->GetActive() && !main_windows[1]->IsMovementActive() && main_windows[2]->GetActive() && !main_windows[2]->IsMovementActive()) {
		UpdateCommand();
	}
}

void Scene_SelectSlider::UpdateIndex() {
	main_windows[(curr_window-1+3)%3]->SetIndex((decision_index-1+item_max)%item_max);
	main_windows[curr_window]->SetIndex(decision_index);
	main_windows[(curr_window+1)%3]->SetIndex((decision_index+1)%item_max);
}

void Scene_SelectSlider::CreateSelectSliderWindow() {
    std::vector<std::string> noptions{"解剖记录\n死者死于下午四时\n死因为高铁血红蛋白症", "一瓶胶囊\n含有致命剂量的亚硝酸钠", "3"};
	std::vector<std::string> discriptions{"1", "2", "3"};
	std::vector<std::string> pictures{"证物-解剖记录", "证物-亚硝酸钠", "daodao-1"};

	curr_window = 1;
	decision_index = 1;
    options = noptions;
	item_max = options.size();

	main_windows[0].reset(new Window_SelectSlider(noptions, discriptions, pictures, -320));
	main_windows[1].reset(new Window_SelectSlider(noptions, discriptions, pictures, 0));
	main_windows[2].reset(new Window_SelectSlider(noptions, discriptions, pictures, 320));
	picture_windows[0].reset(new Window_Picture(10 - 320, 80, 60, 60));
	picture_windows[1].reset(new Window_Picture(10, 80, 60, 60));
	picture_windows[2].reset(new Window_Picture(10 + 320, 80, 60, 60));
	//main_window->SetX(Player::menu_offset_x);
	//main_window->SetY(Player::menu_offset_y);
	//picture_windows[0]->Set("证物-解剖记录");
	//picture_windows[1]->Set("证物-亚硝酸钠");
	main_windows[0]->SetPictureWindow(picture_windows[0].get());
	main_windows[1]->SetPictureWindow(picture_windows[1].get());
	main_windows[2]->SetPictureWindow(picture_windows[2].get());
	//main_window->SetHelpWindow(help_window.get());
	UpdateIndex();
}

void Scene_SelectSlider::UpdateCommand() {
	auto move_right = [&]() {
            Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cursor));
			picture_windows[curr_window]->InitMovement(10, 80, 10 - 320, 80, 15);
			picture_windows[(curr_window+1)%3]->InitMovement(10 + 320, 80, 10, 80, 15);
			picture_windows[(curr_window-1+3)%3]->InitMovement(10 - 320, 80, 10 + 320, 80, 0);
			main_windows[curr_window]->InitMovement(0, 70, -320, 70, 15);
			main_windows[(curr_window+1)%3]->InitMovement(320, 70, 0, 70, 15);
			main_windows[(curr_window-1+3)%3]->InitMovement(-320, 70, 320, 70, 0);
			curr_window = (curr_window+1)%3;
			decision_index = (decision_index+1)%item_max;
			UpdateIndex();
	};
	auto move_left = [&]() {
            Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cursor));
			picture_windows[curr_window]->InitMovement(10, 80, 10 + 320, 80, 15);
			picture_windows[(curr_window-1+3)%3]->InitMovement(10 - 320, 80, 10, 80, 15);
			picture_windows[(curr_window+1)%3]->InitMovement(10 + 320, 80, 10 - 320, 80, 0);
			main_windows[curr_window]->InitMovement(0, 70, 320, 70, 15);
			main_windows[(curr_window-1+3)%3]->InitMovement(-320, 70, 0, 70, 15);
			main_windows[(curr_window+1)%3]->InitMovement(320, 70, -320, 70, 0);
			curr_window = (curr_window-1+3)%3;
			decision_index = (decision_index-1+item_max)%item_max;
			UpdateIndex();
	};
	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		Scene::Pop();
	} else if (Input::IsTriggered(Input::DECISION)) {

		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		Main_Data::game_variables->Set(99, decision_index);
		Scene::Pop();		
	} else if (Input::IsTriggered(Input::LEFT)) {
		move_left();
	} else if (Input::IsTriggered(Input::RIGHT)) {
		move_right();
	}
}