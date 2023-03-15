#ifdef EMSCRIPTEN
#include "game_multiplayer_settings_scene.h"
#include "game_multiplayer_connection.h"
#include "chat_multiplayer.cpp"
#include "game_multiplayer_other_player.h"
#include "game_multiplayer_my_data.h"
#include "input.h"
#include "game_system.h"
#include "bitmap.h"
#include "output.h"
#include "font.h"
#include "player.h"
#include <algorithm>
#include <cmath>
#include <math.h>

namespace Game_Multiplayer {

	void LoadSetting(std::string name, int* ptr) {
		*ptr = EM_ASM_INT({
			let settingName = "MultiplayerSettings[" + UTF8ToString($0) + "]";
			return (window.localStorage.hasOwnProperty(settingName)) ? window.localStorage[settingName] : $1;
		}, name.c_str(), *ptr);
	}

	void SaveSetting(std::string name, int value) {
		EM_ASM({
			window.localStorage["MultiplayerSettings[" + UTF8ToString($0) + "]"] = $1;
		}, name.c_str(), value);
	}

	bool Reconnect(SettingsItem* item, Input::InputButton action) {
		SetConnStatusWindowText("Disconnected");
		ConnectionData::connected = false;
		emscripten_websocket_deinitialize();
		return true;
	}

	bool ToggleGlobalChatVisivility(SettingsItem* item, Input::InputButton action) {
		chatBox->toggleVisibilityFlag(CV_GLOBAL);
		return true;
	}

	Window_Multiplayer::Window_Multiplayer(int ix, int iy, int iwidth, int iheight) :
	Window_Selectable(ix, iy, iwidth, iheight) {
		this->column_max = 1;
		this->index = 0;

		this->UpdateHelpFn = [this](Window_Help& win, int index) {
			if (index >= 0 && index < static_cast<int>(this->items.size())) {
				win.SetText(items[index]->help);
			} else {
				win.SetText("");
			}
		};

		if (Player::IsCP936()) {
			items.push_back(std::make_unique<SwitchOption>("音效同步", "可以听见其他玩家的音效", &MyData::sfxsync));
			items.push_back(std::make_unique<RangeOption>("音效距离", "可以听见其他玩家的音效的最大距离", &MyData::sfxfalloff, 4, 32, 1));
			items.push_back(std::make_unique<RangeOption>("音效音量", "其他玩家产生音效的音量", &MyData::playersVolume, 0, 100, 2));
			items.push_back(std::make_unique<SwitchOption>("NPC 同步", "你和其他玩家看见的 NPC 的位置相同", &MyData::syncnpc));
			items.push_back(std::make_unique<SwitchOption>("显示昵称", "可以看见其它玩家们的昵称", &MyData::rendernametags));
			items.push_back(std::make_unique<SwitchOption>("多彩名片", "将玩家的昵称染上其正在使用的系统颜色", &MyData::systemsync));
			items.push_back(std::make_unique<ActionOption>("全局聊天", "是否开启全局聊天", &ToggleGlobalChatVisivility));
			items.push_back(std::make_unique<ActionOption>("断线重连", "断开连接，清除玩家并重新连接", &Reconnect));
		} else if (Player::IsBig5()) {
			items.push_back(std::make_unique<SwitchOption>("音效同步", "可以聽見其他玩家的音效", &MyData::sfxsync));
			items.push_back(std::make_unique<RangeOption>("音效距離", "可以聽見其他玩家的音效的最大距離", &MyData::sfxfalloff, 4, 32, 1));
			items.push_back(std::make_unique<RangeOption>("音效音量", "其他玩家產生音效的音量", &MyData::playersVolume, 0, 100, 2));
			items.push_back(std::make_unique<SwitchOption>("NPC 同步", "你和其他玩家看見的 NPC 的位置相同", &MyData::syncnpc));
			items.push_back(std::make_unique<SwitchOption>("顯示昵稱", "可以看見其它玩家們的昵稱", &MyData::rendernametags));
			items.push_back(std::make_unique<SwitchOption>("多彩名片", "將玩家的昵稱染上其正在使用的系統顏色", &MyData::systemsync));
			items.push_back(std::make_unique<ActionOption>("全局聊天", "是否開啟全局聊天", &ToggleGlobalChatVisivility));
			items.push_back(std::make_unique<ActionOption>("斷線重連", "斷開連接，清除玩家並重新連接", &Reconnect));
		} else if (Player::IsCP932()) {
			items.push_back(std::make_unique<SwitchOption>("効果音同期", "他のプレイヤーの音が聞こえます", &MyData::sfxsync));
			items.push_back(std::make_unique<RangeOption>("効果音距離", "他のプレイヤーの効果音が聞こえる最大距離", &MyData::sfxfalloff, 4, 32, 1));
			items.push_back(std::make_unique<RangeOption>("効果音音量", "他のプレイヤーが発する効果音の音量", &MyData::playersVolume, 0, 100, 2));
			items.push_back(std::make_unique<SwitchOption>("NPC 同期", "あなたと他のプレイヤーは同じ位置にNPCを見る", &MyData::syncnpc));
			items.push_back(std::make_unique<SwitchOption>("ニックネームを表示", "他のプレイヤーのニックネームを見ることができます", &MyData::rendernametags));
			items.push_back(std::make_unique<SwitchOption>("カラフルな名刺", "プレーヤーのニックネームを、使用しているシステムの色で色付けします", &MyData::systemsync));
			items.push_back(std::make_unique<ActionOption>("グローバルチャット", "グローバルチャットを有効にするかどうか", &ToggleGlobalChatVisivility));
			items.push_back(std::make_unique<ActionOption>("接続切断後に再接続する", "接続を解除し、プレーヤーをクリアしてから再接続する", &Reconnect));
		} else {
			items.push_back(std::make_unique<SwitchOption>("SFX sync", "Lets you hear sound effects that other players produce", &MyData::sfxsync));
			items.push_back(std::make_unique<RangeOption>("SFX falloff", "Maximum distance at which you can hear players", &MyData::sfxfalloff, 4, 32, 1));
			items.push_back(std::make_unique<RangeOption>("Players SFX volume", "Volume of the sound effects that other players produce", &MyData::playersVolume, 0, 100, 2));
			items.push_back(std::make_unique<SwitchOption>("Event movement synchronisation", "Lets you and other players see NPCs at same positions", &MyData::syncnpc));
			items.push_back(std::make_unique<SwitchOption>("Event sprite synchronisation", "Lets you and other players see NPCs with the same sprites", &MyData::npcspritesync));
			items.push_back(std::make_unique<SwitchOption>("Event activity synchronisation", "Lets you and other players see same active NPCs", &MyData::npcactivitysync));
			items.push_back(std::make_unique<SwitchOption>("Nametags", "Lets you see usernames of the players in game", &MyData::rendernametags));
			items.push_back(std::make_unique<SwitchOption>("Nametags system sync", "Render nametags with system colors that player is using", &MyData::systemsync));
			items.push_back(std::make_unique<ActionOption>("Global chat", "Toggle global chat visibility", &ToggleGlobalChatVisivility));
			items.push_back(std::make_unique<ActionOption>("Reconnect", "Disconnect, clear players & connect again", &Reconnect));
		}

		this->item_max = items.size();
		CreateContents();
		SetEndlessScrolling(false);
	}

	void Window_Multiplayer::Update() {
		Window_Selectable::Update();
	}

	void Window_Multiplayer::Refresh() {
		DrawItems();
	}

	void Window_Multiplayer::DrawItems() {
		for(int i = 0; i < items.size(); i++) {
			DrawItem(i);
		}
	}

	void Window_Multiplayer::DrawItem(int i) {
		contents->ClearRect(Rect(0, menu_item_height * i, contents->GetWidth() - 0, menu_item_height));
		contents->TextDraw(0, menu_item_height * i + menu_item_height / 8, items[i]->color, items[i]->name);
		contents->TextDraw(GetWidth() - 16, menu_item_height * i + menu_item_height / 8, items[i]->color_right, items[i]->text_right, Text::AlignRight);
	}

	void Window_Multiplayer::Action(Input::InputButton action) {
		items[index]->HandleAction(action);
	}

	Scene_MultiplayerSettings::Scene_MultiplayerSettings() {
		Scene::type = Scene::Multiplayer;
	}

	void Scene_MultiplayerSettings::Start() {
		settings_window.reset(new Window_Multiplayer(0, 32, SCREEN_TARGET_WIDTH, SCREEN_TARGET_HEIGHT - 32));
		help_window.reset(new Window_Help(0, 0, SCREEN_TARGET_WIDTH, 32));
		settings_window->SetHelpWindow(help_window.get());
		settings_window->Refresh();
	}
	void Scene_MultiplayerSettings::Update() {
		settings_window->Update();
		help_window->Update();

		//exit settings event
		if (Input::IsTriggered(Input::CANCEL)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			Scene::Pop();
		} else if (Input::IsTriggered(Input::DECISION)) {
			settings_window->Action(Input::DECISION);
			settings_window->Refresh();
		} else if (Input::IsTriggered(Input::LEFT)) {
			settings_window->Action(Input::LEFT);
			settings_window->Refresh();
		} else if (Input::IsTriggered(Input::RIGHT)) {
			settings_window->Action(Input::RIGHT);
			settings_window->Refresh();
		}
	}

	void ActionOption::HandleAction(Input::InputButton action) {
		if(this->onAction(this, action)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		} else {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
		}
	}

	ActionOption::ActionOption(const std::string& name, const std::string& help, bool (*onAction) (SettingsItem* item, Input::InputButton action)) {
		this->name = name;
		this->help = help;
		this->onAction = onAction;
		this->color = Font::SystemColor::ColorDefault;
	}

	void SwitchOption::HandleAction(Input::InputButton action) {
		*(this->switch_ref) = !(*(this->switch_ref));
		this->text_right = *(this->switch_ref) ? "[ON]" : "[OFF]";
		this->color_right = *(this->switch_ref) ? Font::SystemColor::ColorDefault : Font::SystemColor::ColorDisabled;
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		SaveSetting(name, *switch_ref);
	}

	SwitchOption::SwitchOption(const std::string& name, const std::string& help, bool* switch_ref) {
		LoadSetting(name, (int*)switch_ref);
		this->name = name;
		this->help = help;
		this->switch_ref = switch_ref;
		this->color = Font::SystemColor::ColorDefault;
		this->text_right = *(this->switch_ref) ? "[ON]" : "[OFF]";
		this->color_right = *(this->switch_ref) ? Font::SystemColor::ColorDefault : Font::SystemColor::ColorDisabled;
	}


	void RangeOption::HandleAction(Input::InputButton action) {
		if(action == Input::InputButton::LEFT) {
			*(this->range) -= this->step;
		} else if (action == Input::InputButton::RIGHT) {
			*(this->range) += this->step;
		}

		if(min > *range || *range > max) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
		} else {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cursor));
		}

		//for some reason i can't get std::clamp to work???
		if(*(this->range) > this->max) *(this->range) = this->max;
		if(*(this->range) < this->min) *(this->range) = this->min;

		this->text_right = std::to_string(*range);
		SaveSetting(name, *range);
	}
	RangeOption::RangeOption(const std::string& name, const std::string& help, int* range, int min, int max, int step) {
		LoadSetting(name, range);
		this->name = name;
		this->help = help;
		this->range = range;
		this->min = min;
		this->max = max;
		this->step = step;
		this->color = Font::SystemColor::ColorDefault;
		this->color_right = Font::SystemColor::ColorDefault;
		this->text_right = std::to_string(*range);
	}
}
#endif
