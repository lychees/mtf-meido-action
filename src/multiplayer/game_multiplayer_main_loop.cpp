#ifdef EMSCRIPTEN
#include "game_multiplayer_main_loop.h"
#include "chat_multiplayer.h"
#include "game_multiplayer_other_player.h"
#include "game_multiplayer_senders.h"
#include "game_multiplayer_js_export.h"
#include "game_multiplayer_nametags.h"
#include "game_multiplayer_my_data.h"
#include "game_multiplayer_connection.h"
#include "nxjson.h"
#include <map>
#include <memory>
#include <queue>
#include <set>

#include "../output.h"
#include "../game_player.h"
#include "../sprite_character.h"
#include "../window_base.h"
#include "../drawable_mgr.h"
#include "../scene.h"
#include "../bitmap.h"
#include "../font.h"
#include "../input.h"
#include "../cache.h"
#include "../game_screen.h"
#include "../game_variables.h"
#include "../game_switches.h"
#include "../game_map.h"
#include "../player.h"

namespace Game_Multiplayer {

std::list<std::pair<int, std::pair<int, int>>> npcmoves;

//this assumes that the player is stopped
void MoveChatacterToPos(Game_Character* player, int x, int y) {
	if (!player->IsStopping()) {
		Output::Debug("MovePlayerToPos unexpected error: the player is busy being animated");
	}
	int dx = x - player->GetX();
	int dy = y - player->GetY();
	if (abs(dx) > 1 || abs(dy) > 1 || dx == 0 && dy == 0) {
		player->SetX(x);
		player->SetY(y);
		return;
	}
	int dir[3][3] = {{Game_Character::Direction::UpLeft, Game_Character::Direction::Up, Game_Character::Direction::UpRight},
					 {Game_Character::Direction::Left, 0, Game_Character::Direction::Right},
					 {Game_Character::Direction::DownLeft, Game_Character::Direction::Down, Game_Character::Direction::DownRight}};
	player->Move(dir[dy+1][dx+1]);
}

void Update() {


	if(MyData::shouldsync) {
		SyncMe();
		MyData::shouldsync = false;
	}

	//apply weather
	if(MyData::nextWeatherType != -1) {
		MyData::weatherT++;
		if(MyData::weatherT > MyData::weatherSetDelay) {
			MyData::weatherT = 0;
			uint16_t msg[3] = {PacketTypes::weather, (uint16_t)MyData::nextWeatherType, (uint16_t)MyData::nextWeatherStrength};
			TrySend(msg, sizeof(uint16_t) * 3);
			MyData::nextWeatherType = -1;
			MyData::nextWeatherStrength = -1;
		}
	}

	for (auto& p : other_players) {
		auto& q = p.second.mvq;
		if (!q.empty() && p.second.ch->IsStopping()) {
			auto posX = q.front().first;
			auto posY = q.front().second;
			MoveChatacterToPos(p.second.ch.get(), posX, posY);
			nameTagRenderer->moveNameTag(p.first, posX, posY);
			if(q.size() > 8) {
				p.second.ch->SetMoveSpeed(6);
				while(q.size() > 16)
						q.pop();
			} else {
				p.second.ch->SetMoveSpeed(p.second.moveSpeed);
			}
			q.pop();
		}
		p.second.ch->SetProcessed(false);

		for(auto npc : npcmoves) {
			Game_Character* character = Game_Map::GetEvent(npc.first);
			if(character) {
				if(character->IsStopping()) {
					MoveChatacterToPos(character, npc.second.first, npc.second.second);
					npcmoves.remove(npc);
				}
			}
		}

		Color ch_prev_flash_c = p.second.ch->GetFlashColor();
		int ch_prev_flash_p = p.second.ch->GetFlashLevel();
		int ch_prev_flash_t = p.second.ch->GetFlashTimeLeft();

		p.second.ch->Update();
		p.second.sprite->Update();

		if(p.second.flashpause) {
			p.second.ch->Flash(ch_prev_flash_c.red, ch_prev_flash_c.green, ch_prev_flash_c.blue, ch_prev_flash_p, ch_prev_flash_t);
		}
	}



	if(!ConnectionData::connected) {
		time_t currentTime = time(NULL);

		if(currentTime - ConnectionData::lastConnect > ConnectionData::reconnectInterval) {
			ConnectToGame();
			ConnectionData::lastConnect = currentTime;
		}
	}

	if(ConnectionData::roomFirstUpdate) {
		if(MyData::spritesheet != "")
			SlashCommandSetSprite(MyData::spritesheet.c_str(), MyData::spriteid);
		auto& player = Main_Data::game_player;
		SendMainPlayerPos();
		SendMainPlayerSprite(player->GetSpriteName(), player->GetSpriteIndex());
		SendMainPlayerName();
		SendMainPlayerMoveSpeed((int)(player->GetMoveSpeed()));

		ConnectionData::roomFirstUpdate = false;
	}
}

}
#endif
