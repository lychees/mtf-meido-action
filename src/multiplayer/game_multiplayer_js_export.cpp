#ifdef EMSCRIPTEN
#include "game_multiplayer_js_export.h"
#include "chat_multiplayer.h"
#include "game_multiplayer_my_data.h"
#include "game_multiplayer_senders.h"
#include "game_multiplayer_connection.h"
#include "game_multiplayer_player_tracker.h"
#include "../game_player.h"
#include "../scene.h"
#include "../main_data.h"

using namespace Game_Multiplayer;

extern "C" {

	void SetWSHost(const char* host) {
		Game_Multiplayer::ConnectionData::host = host;
	}

	void gotMessage(const char* name, const char* trip, const char* msg, const char* src) {
		#if defined(INGAME_CHAT)
		Chat_Multiplayer::gotMessage(name, trip, msg, src);
		#endif
	}

	void gotChatInfo(const char* source, const char* text) {
		#if defined(INGAME_CHAT)
		Chat_Multiplayer::gotInfo(text);
		#endif
	}

	void ChangeName(const char* name) {
		Game_Multiplayer::MyData::username = name;
		Game_Multiplayer::SendMainPlayerName();
	}

	void SlashCommandSetSprite(const char* sheet, int id) {
		Game_Multiplayer::MyData::spritesheet = sheet;
		Game_Multiplayer::MyData::spriteid = id;
		if(Game_Multiplayer::MyData::spritesheet.length())
			Main_Data::game_player->SetSpriteGraphic(sheet, id);
	}

	void SetPlayersVolume(int volume) {
		Game_Multiplayer::MyData::playersVolume = volume;
	}

	void SetSwitchSync(int val) {
		Game_Multiplayer::MyData::switchsync = val;
	}

	void SetSwitchSyncWhiteList(int id, int val) {
		if(val) {
			Game_Multiplayer::MyData::syncedswitches.emplace(id);
		} else {
			Game_Multiplayer::MyData::syncedswitches.erase(id);
		}
	}

	void SetSwitchSyncLogBlackList(int id, int val) {
		if(val) {
			Game_Multiplayer::MyData::switchlogblacklist.emplace(id);
		} else {
			Game_Multiplayer::MyData::switchlogblacklist.erase(id);
		}
	}

	void LogSwitchSyncWhiteList() {
		std::string liststr = "";
		for(auto& swt : Game_Multiplayer::MyData::syncedswitches) {
			liststr += std::to_string(swt) + ",";
		}
		EM_ASM({console.log(UTF8ToString($0));}, liststr.c_str());
	}

	void SwitchNpcSync() {
		Game_Multiplayer::MyData::syncnpc = !Game_Multiplayer::MyData::syncnpc;
	}

	void TrackCommand(const char* name) {
		trackerRenderer->Track(name);
	}
	void UntrackCommand(const char* name) {
		trackerRenderer->Untrack(name);
	}
}
#endif
