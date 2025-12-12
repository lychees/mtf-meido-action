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

Scene_Bartending::Scene_Bartending(int decision_index) :
	decision_index(decision_index) {
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

	
	configor::json json = {
		{"Mojito", {
			{"name", "Mojito"},
			{"name_zh", "莫吉托"},
			{"ingredients", {"白朗姆 50 ml", "青柠 20 ml", "糖 2 勺", "薄荷叶 8-10", "苏打水"}},
			{"price", 68},
			{"abv", 14},
			{"taste", "薄荷凉、青柠酸、气泡清爽"},
			{"mood", "夏日街头、周青春、无酒精版友好"},
			{"description", "古巴传奇调酒，清凉薄荷与酸爽青柠的完美碰撞，夏日解暑圣品"}
		}},
		{"Long Island Iced Tea", {
			{"name", "Long Island Iced Tea"},
			{"name_zh", "长岛冰茶"},
			{"ingredients", {"伏特加 15 ml", "金酒 15 ml", "朗姆 15 ml", "龙舌兰 15 ml", "橙酒 15 ml", "酸甜 25 ml", "可乐补满"}},
			{"price", 88},
			{"abv", 28},
			{"taste", "可乐、柠檬、烈"},
			{"mood", "买醉、失恋、学生党"},
			{"description", "四种基酒伪装成冰茶的经典之作，外表温和实则烈性十足"}
		}},
		{"Margarita", {
			{"name", "Margarita"},
			{"name_zh", "玛格丽特"},
			{"ingredients", {"龙舌兰 50 ml", "橙味利口酒 20 ml", "青柠汁 20 ml", "盐边"}},
			{"price", 80},
			{"abv", 26},
			{"taste", "酸、咸、龙舌兰植物香"},
			{"mood", "海边度假、失恋疗愈、派对开场"},
			{"description", "龙舌兰的灵魂之作，盐边与酸橙的戏剧性组合，墨西哥国饮"}
		}},
		{"Blue Hawaii", {
			{"name", "Blue Hawaii"},
			{"name_zh", "蓝色夏威夷"},
			{"ingredients", {"白朗姆 45 ml", "蓝橙酒 15 ml", "菠萝汁 60 ml", "椰奶 30 ml"}},
			{"price", 78},
			{"abv", 16},
			{"taste", "椰子、菠萝、海洋色"},
			{"mood", "泳池浮床、度假、拍照打卡"},
			{"description", "视觉与味觉的双重热带风暴，蓝色的海洋风情装在杯里"}
		}},
		{"Negroni", {
			{"name", "Negroni"},
			{"name_zh", "尼格罗尼"},
			{"ingredients", {"金酒 30 ml", "金巴利 30 ml", "红威末 30 ml", "橙皮 1 片"}},
			{"price", 85},
			{"abv", 28},
			{"taste", "苦甜平衡、草本、柑橘尾韵"},
			{"mood", "餐前开胃、老友重逢、初尝苦酒"},
			{"description", "意式苦甜哲学的结晶，金巴利的草本苦与金酒的杜松子香完美交融"}
		}},
		{"Piña Colada", {
			{"name", "Piña Colada"},
			{"name_zh", "椰林飘香"},
			{"ingredients", {"白朗姆 60 ml", "椰奶 60 ml", "菠萝汁 90 ml", "碎冰"}},
			{"price", 82},
			{"abv", 15},
			{"taste", "椰香、菠萝、奶昔"},
			{"mood", "泳池派对、热带假期、无酒精版"},
			{"description", "波多黎各的流动假期，椰子菠萝的奶昔式调酒，度假模式启动器"}
		}},
		{"Bomb Shot", {
			{"name", "Bomb Shot"},
			{"name_zh", "炸弹酒"},
			{"ingredients", {"功能饮料 150 ml", "野格 30 ml 或 伏特加 30 ml", "shot杯"}},
			{"price", 55},
			{"abv", 18},
			{"taste", "甜、气泡、烈酒冲击"},
			{"mood", "夜店、游戏、快速上头"},
			{"description", "派对文化符号，能量饮料与烈酒的狂欢式碰撞，一饮而尽的快感"}
		}},
		{"Clover Club", {
			{"name", "Clover Club"},
			{"name_zh", "三叶草俱乐部"},
			{"ingredients", {"金酒 45 ml", "覆盆子糖浆 20 ml", "柠檬汁 20 ml", "蛋清"}},
			{"price", 82},
			{"abv", 22},
			{"taste", "莓果酸甜、绵密泡沫、少女粉"},
			{"mood", "闺蜜之夜、拍照打卡、酸甜控"},
			{"description", "费城绅士俱乐部的粉色浪漫，蛋清带来云朵般的绵密口感"}
		}},
		{"Cosmopolitan", {
			{"name", "Cosmopolitan"},
			{"name_zh", "大都会"},
			{"ingredients", {"伏特加 40 ml", "君度 15 ml", "蔓越莓汁 30 ml", "青柠汁 15 ml"}},
			{"price", 78},
			{"abv", 22},
			{"taste", "酸甜、蔓越莓、粉红"},
			{"mood", "欲望都市、闺蜜、粉红滤镜"},
			{"description", "《欲望都市》中的都市女性标志，蔓越莓的粉红酸甜诱惑"}
		}},
		{"Strawberry Daiquiri", {
			{"name", "Strawberry Daiquiri"},
			{"name_zh", "草莓代基里"},
			{"ingredients", {"白朗姆 50 ml", "草莓 60 g", "糖浆 15 ml", "青柠汁 20 ml"}},
			{"price", 76},
			{"abv", 20},
			{"taste", "草莓、酸甜、冰沙"},
			{"mood", "少女、冰沙、拍照"},
			{"description", "古巴代基里的草莓变奏，新鲜草莓的冰沙式甜蜜革命"}
		}},
		{"White Lady", {
			{"name", "White Lady"},
			{"name_zh", "白色佳人"},
			{"ingredients", {"金酒 45 ml", "君度 25 ml", "柠檬汁 20 ml", "可选蛋清"}},
			{"price", 80},
			{"abv", 26},
			{"taste", "顺滑、柑橘、柔和"},
			{"mood", "优雅女、复古、轻熟"},
			{"description", "金酒的优雅三重奏，君度的柑橘香与柠檬的酸爽完美平衡"}
		}},
		{"Zombie", {
			{"name", "Zombie"},
			{"name_zh", "僵尸"},
			{"ingredients", {"混合朗姆 75 ml", "青柠汁 25 ml", "菠萝汁 45 ml", "百香果糖浆 15 ml", "苦精 1 dash"}},
			{"price", 98},
			{"abv", 30},
			{"taste", "果香、朗姆、烈"},
			{"mood", "Tiki派对、限量两杯、断片警告"},
			{"description", "Tiki文化的终极烈酒怪兽，三种朗姆打造的复活节岛传奇"}
		}},
		{"Martini", {
			{"name", "Martini"},
			{"name_zh", "马天尼"},
			{"ingredients", {"伦敦干金 60 ml", "干威末 10 ml", "柠檬皮或橄榄"}},
			{"price", 95},
			{"abv", 34},
			{"taste", "干冽、杜松子、丝滑"},
			{"mood", "商务谈判、詹姆斯·邦德、优雅正装"},
			{"description", "鸡尾酒之王，007的标志性选择，干与烈的终极平衡艺术"}
		}},
		{"Rum", {
			{"name", "Rum"},
			{"name_zh", "朗姆酒"},
			{"ingredients", {"纯饮 - 白朗姆或陈酿朗姆 45 ml", "或加冰直饮"}},
			{"price", 40},
			{"abv", 40},
			{"taste", "甘蔗甜香、橡木、焦糖"},
			{"mood", "加勒比海、航海、自由"},
			{"description", "加勒比海盗的液体黄金，甘蔗蒸馏的航海灵魂，调酒界的万能基酒"}
		}},
		{"Beer", {
			{"name", "Beer"},
			{"name_zh", "啤酒"},
			{"ingredients", {"拉格或艾尔啤酒 330 ml", "可加柠檬片"}},
			{"price", 35},
			{"abv", 5},
			{"taste", "麦香、清爽、微苦"},
			{"mood", "看球赛、烧烤、解渴"},
			{"description", "人类最古老的酒精饮料，从美索不达米亚流淌至今的欢乐泉水"}
		}},
		{"Vodka", {
			{"name", "Vodka"},
			{"name_zh", "伏特加"},
			{"ingredients", {"纯饮 - 伏特加 45 ml", "或作为基酒使用"}},
			{"price", 45},
			{"abv", 40},
			{"taste", "纯净、中性、轻微谷物香"},
			{"mood", "战斗民族、调酒基底、直接"},
			{"description", "斯拉夫文化的纯净灵魂，蒸馏技术的极致追求，调酒界的隐形冠军"}
		}}
	};

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
