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
#include "scene_bartending.h"
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

constexpr int menu_bartending_width = 88;
//constexpr int gold_window_width = 88;
constexpr int menu_help_height = 64;

Scene_Bartending::Scene_Bartending(int decision_index, configor::json json) :
	decision_index(decision_index), json(json) {
	type = Scene::Bartending;
}

void Scene_Bartending::Start() {
	CreateBartendingWindow();
}

//void Scene_Bartending::Continue(SceneType /* prev_scene */) {
//	menustatus_window->Refresh();
//	gold_window->Refresh();
//}


void Scene_Bartending::vUpdate() {
	bartending_window->Update();
	help_window->Update();
	//gold_window->Update();
	//menustatus_window->Update();

	if (bartending_window->GetActive()) {
		UpdateCommand();
	}
	//else if (menustatus_window->GetActive()) {
	//	UpdateActorSelection();
	//}
}

void Scene_Bartending::CreateBartendingWindow() {
	// Create Options Window
	std::vector<std::string> options;
	std::vector<std::string> discriptions;
	std::vector<std::string> pictures;
	picture_window.reset(new Window_Picture(Player::menu_offset_x + 88, Player::menu_offset_y, Player::screen_width - menu_bartending_width, 176));
	help_window.reset(new Window_Help(Player::menu_offset_x, Player::menu_offset_y + 176, MENU_WIDTH, menu_help_height));


	for (configor::json::iterator it = json.begin(); it != json.end(); ++it) {
		std::string name = (*it)["name"];
		bartending_options.push_back(name);
		name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
		pictures.push_back(name);
  		discriptions.push_back((*it)["description"]);
	}
	

	options = bartending_options;

	bartending_window.reset(new Window_Bartending(options, discriptions, pictures, menu_bartending_width, 10));
	bartending_window->SetX(Player::menu_offset_x);
	bartending_window->SetY(Player::menu_offset_y);
	bartending_window->SetPictureWindow(picture_window.get());
	bartending_window->SetHelpWindow(help_window.get());
	bartending_window->SetIndex(decision_index);

	// Disable items
	/*
	for (it = bartending_options.begin(); it != bartending_options.end(); ++it) {
		switch(*it) {
		case Save:
			// If save is forbidden disable this item
			if (!Main_Data::game_system->GetAllowSave()) {
				bartending_window->DisableItem(it - bartending_options.begin());
			}
		case Wait:
		case Quit:
		case Settings:
		case Debug:
			break;
		case Order:
			if (Main_Data::game_party->GetActors().size() <= 1) {
				bartending_window->DisableItem(it - bartending_options.begin());
			}
			break;
		default:
			if (Main_Data::game_party->GetActors().empty()) {
				bartending_window->DisableItem(it - bartending_options.begin());
			}
			break;
		}
	}
	*/
}

void Scene_Bartending::UpdateCommand() {
	if (Input::IsTriggered(Input::DECISION)) {
		decision_index = bartending_window->GetIndex();

		if (decision_index < bartending_options.size()) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Main_Data::game_variables->Set(99, decision_index);
			Scene::Pop();
		}
	}
}
/*
void Scene_Bartending::UpdateActorSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		bartending_window->SetActive(true);
		menustatus_window->SetActive(false);
		menustatus_window->SetIndex(-1);
	} else if (Input::IsTriggered(Input::DECISION)) {
		switch (bartending_options[bartending_window->GetIndex()]) {
		case Skill:
			if (!menustatus_window->GetActor()->CanAct()) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
				return;
			}
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Scene::Push(std::make_shared<Scene_Skill>(Main_Data::game_party->GetActors(), menustatus_window->GetIndex()));
			break;
		case Equipment:
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Scene::Push(std::make_shared<Scene_Equip>(Main_Data::game_party->GetActors(), menustatus_window->GetIndex()));
			break;
		case Status:
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Scene::Push(std::make_shared<Scene_Status>(Main_Data::game_party->GetActors(), menustatus_window->GetIndex()));
			break;
		case Row:
		{
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			// Don't allow entire party in the back row.
			const auto& actors = Main_Data::game_party->GetActors();
			int num_in_back = 0;
			for (auto* actor: actors) {
				if (actor->GetBattleRow() == Game_Actor::RowType::RowType_back) {
					++num_in_back;
				}
			}
			Game_Actor* actor = actors[menustatus_window->GetIndex()];
			if (actor->GetBattleRow() == Game_Actor::RowType::RowType_front) {
				if (num_in_back < int(actors.size() - 1)) {
					actor->SetBattleRow(Game_Actor::RowType::RowType_back);
				}
			} else {
				actor->SetBattleRow(Game_Actor::RowType::RowType_front);
			}
			menustatus_window->Refresh();
			break;
		}
		default:
			assert(false);
			break;
		}

		bartending_window->SetActive(true);
		menustatus_window->SetActive(false);
		menustatus_window->SetIndex(-1);
	}
}
*/
