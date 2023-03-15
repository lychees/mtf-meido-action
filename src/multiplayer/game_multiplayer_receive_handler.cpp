#ifdef EMSCRIPTEN

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>

#include "game_multiplayer_receive_handler.h"
#include "game_multiplayer_other_player.h"
#include "game_multiplayer_nametags.h"
#include "game_multiplayer_my_data.h"
#include "game_multiplayer_main_loop.h"
#include <string.h>
#include <map>
#include <memory>
#include <queue>
#include <set>

#include "game_player.h"
#include "drawable_mgr.h"
#include "cache.h"
#include "game_screen.h"
#include "game_variables.h"
#include "game_switches.h"

#include "game_map.h"
#include "player.h"
#include "main_data.h"
#include "game_system.h"
#include "game_multiplayer_rng.h"

namespace Game_Multiplayer {

void HandleReceivedPacket(const char* data) {
	char* data_copy = strdup(data);
	const nx_json* json = nx_json_parse(data_copy, NULL);

	if(json) {
		const nx_json* typeNode = nx_json_get(json, "type");

		if(typeNode->type == nx_json_type::NX_JSON_STRING) {
			if(strcmp(typeNode->text_value, "objectSync") == 0) {
				ResolveObjectSyncPacket(json);
			}
			else if(strcmp(typeNode->text_value, "disconnect") == 0) {
				HandleDisconnect(json);
			} else if(strcmp(typeNode->text_value, "rngSeed") == 0) {
				room_seed = nx_json_get(json, "seed")->num.u_value;
			}
		}

		nx_json_free(json);
	}
	delete[] data_copy;
}

void HandleDisconnect(const nx_json* json) {
	const nx_json* uid_json;

	const nx_json* uid = nx_json_get(json, "uuid");
	if(uid->type == nx_json_type::NX_JSON_STRING) {
		ErasePlayer(uid->text_value);
	}

	for(int i = 0; i < HostedNpcArrayCapacity; i++) {
		MyData::hostednpc[i] = true;
	}
}

void ResolveObjectSyncPacket(const nx_json* json) {
	if(json->type == nx_json_type::NX_JSON_OBJECT) {
		const nx_json* pos = nx_json_get(json, "pos");
		const nx_json* path = nx_json_get(json, "path");;
		const nx_json* sprite = nx_json_get(json, "sprite");
		const nx_json* sound = nx_json_get(json, "sound");
		const nx_json* uid = nx_json_get(json, "uid");
		const nx_json* name = nx_json_get(json, "name");
		const nx_json* weather = nx_json_get(json, "weather");
		const nx_json* mAnimSpd = nx_json_get(json, "movementAnimationSpeed");
		const nx_json* variable = nx_json_get(json, "varialbe");
		const nx_json* switchsync = nx_json_get(json, "switchsync");
		const nx_json* animtype = nx_json_get(json, "animtype");
		const nx_json* animframe = nx_json_get(json, "animframe");
		const nx_json* facing = nx_json_get(json, "facing");
		const nx_json* typingstatus = nx_json_get(json, "typingstatus");
		const nx_json* flash = nx_json_get(json, "flash");
		const nx_json* flashpause = nx_json_get(json, "flashpause");
		const nx_json* npcmove = nx_json_get(json, "npcmove");
		const nx_json* system = nx_json_get(json, "system");
		const nx_json* npcsprite = nx_json_get(json, "npcsprite");
		const nx_json* npcactive = nx_json_get(json, "npcactive");

		if(uid->type == nx_json_type::NX_JSON_STRING) {

			std::string uid_string = std::string(uid->text_value);

			if(strcmp(uid->text_value, "room")) {
				MPPlayer& mpplayer = GetPlayerOrCreate(uid_string);

				if(pos->type == nx_json_type::NX_JSON_OBJECT) {
					mpplayer.mvq.push(std::make_pair(nx_json_get(pos, "x")->num.u_value, nx_json_get(pos, "y")->num.u_value));
				}
				else if(path->type == nx_json_type::NX_JSON_ARRAY) {
					for(int i = 0; i < path->children.length; i++) {
						pos = nx_json_item(path, i);
						if(pos->type == nx_json_type::NX_JSON_OBJECT) {
							mpplayer.mvq.push(std::make_pair(nx_json_get(pos, "x")->num.u_value, nx_json_get(pos, "y")->num.u_value));
						}
					}
				}

				if(sprite->type == nx_json_type::NX_JSON_OBJECT) {
					const nx_json* sheet = nx_json_get(sprite, "sheet");
					const nx_json* id = nx_json_get(sprite, "id");
					mpplayer.ch->SetSpriteGraphic(std::string(sheet->text_value), id->num.u_value);
					mpplayer.ch->ResetAnimation();
				}

				if(sound->type == nx_json_type::NX_JSON_OBJECT && MyData::sfxsync) {
					const nx_json* volume = nx_json_get(sound, "volume");
					const nx_json* tempo = nx_json_get(sound, "tempo");
					const nx_json* balance = nx_json_get(sound, "balance");
					const nx_json* name = nx_json_get(sound, "name");

					lcf::rpg::Sound soundStruct;
					auto& p = mpplayer;
					int w = Game_Map::GetWidth();
					int h = Game_Map::GetHeight();
					int dx = std::min(std::abs(p.ch->GetX() - Main_Data::game_player->GetX()), std::abs(p.ch->GetX() - w - Main_Data::game_player->GetX()));
					int dy = std::min(std::abs(p.ch->GetY() - Main_Data::game_player->GetY()), std::abs(p.ch->GetY() - h - Main_Data::game_player->GetY()));
					int distance = std::sqrt(dx * dx + dy * dy);
					float falloffFactor = 100.0f / ((float)MyData::sfxfalloff);
					soundStruct.volume = std::max(0,
					(int)
					((100.0f - ((float)distance) * falloffFactor) * (float(MyData::playersVolume) / 100.0f) * (float(volume->num.u_value) / 100.0f))
					);
					soundStruct.tempo = tempo->num.u_value;
					soundStruct.balance = balance->num.u_value;
					soundStruct.name = std::string(name->text_value);

					Main_Data::game_system->SePlay(soundStruct);
				}

				if(name->type == nx_json_type::NX_JSON_STRING) {
					nameTagRenderer->setTagName(uid_string, name->text_value);
					mpplayer.nickname = name->text_value;
				}

				if(weather->type == nx_json_type::NX_JSON_OBJECT) {
					const nx_json* type = nx_json_get(weather, "type");
					const nx_json* strength = nx_json_get(weather, "strength");
					Main_Data::game_screen.get()->SetWeatherEffect(type->num.u_value, strength->num.u_value);
				}

				if(mAnimSpd->type == nx_json_type::NX_JSON_INTEGER) {
					mpplayer.moveSpeed = mAnimSpd->num.u_value;
				}

				if(variable->type == nx_json_type::NX_JSON_OBJECT && false) {
					const nx_json* id = nx_json_get(variable, "id");
					const nx_json* value = nx_json_get(variable, "value");
					Main_Data::game_variables->Set(id->num.u_value, value->num.s_value);
					Game_Map::SetNeedRefresh(true);

					std::string setvarstr = std::to_string(id->num.u_value) + " " + std::to_string(value->num.s_value);
					std::string varstr = "var";
					EM_ASM({
						PrintChatInfo(UTF8ToString($0), UTF8ToString($1));
					}, setvarstr.c_str(), varstr.c_str());
				}

				if(switchsync->type == nx_json_type::NX_JSON_OBJECT && MyData::switchsync) {
					const nx_json* id = nx_json_get(switchsync, "id");
					const nx_json* value = nx_json_get(switchsync, "value");
					if(MyData::syncedswitches.find(id->num.u_value) != MyData::syncedswitches.cend()) {
						Main_Data::game_switches->Set(id->num.u_value, value->num.s_value);
						Game_Map::SetNeedRefresh(true);
					}
					std::string setswtstr = std::to_string(id->num.u_value) + " " + std::to_string(value->num.s_value);
					if(MyData::switchlogblacklist.find(id->num.u_value) == MyData::switchlogblacklist.cend()) {
						EM_ASM({
							console.log("switch " + UTF8ToString($0));
						}, setswtstr.c_str());
					}
				}

				if(animtype->type == nx_json_type::NX_JSON_INTEGER) {
					mpplayer.ch->SetAnimationType((lcf::rpg::EventPage::AnimType)animtype->num.u_value);
				}

				if(animframe->type == nx_json_type::NX_JSON_INTEGER) {
					mpplayer.ch->SetAnimFrame(animframe->num.u_value);
				}

				if(facing->type == nx_json_type::NX_JSON_INTEGER) {
					if(facing->num.u_value <= 4)
						mpplayer.ch->SetFacing(facing->num.u_value);
				}

				if(typingstatus->type == nx_json_type::NX_JSON_INTEGER) {
					mpplayer.typingstatus = typingstatus->num.u_value;
				}

				if(flash->type == nx_json_type::NX_JSON_ARRAY) {
					mpplayer.ch->Flash(
						nx_json_item(flash, 0)->num.u_value,
						nx_json_item(flash, 1)->num.u_value,
						nx_json_item(flash, 2)->num.u_value,
						nx_json_item(flash, 3)->num.u_value,
						nx_json_item(flash, 4)->num.u_value
					);
				}

				if(flashpause->type == nx_json_type::NX_JSON_INTEGER) {
					mpplayer.flashpause = flashpause->num.u_value;
				}

				if(system->type == nx_json_type::NX_JSON_STRING) {
					nameTagRenderer->setTagSystem(uid_string, system->text_value);
				}
			}

			if(npcmove->type == nx_json_type::NX_JSON_OBJECT) {
				MyData::hostednpc[nx_json_get(npcmove, "id")->num.u_value] = false;
				if(MyData::syncnpc) {
					Game_Event* character = Game_Map::GetEvent(nx_json_get(npcmove, "id")->num.u_value);
					if(character) {
						if(character->GetX() != nx_json_get(npcmove, "x")->num.u_value || character->GetY() != nx_json_get(npcmove, "y")->num.u_value) {
							character->SetX(nx_json_get(npcmove, "x")->num.u_value);
							character->SetY(nx_json_get(npcmove, "y")->num.u_value);
							character->SetRemainingStep(SCREEN_TILE_SIZE);
						}
						if(character->GetDirection() != nx_json_get(npcmove, "facing")->num.u_value) {
							character->SetDirection(nx_json_get(npcmove, "facing")->num.u_value);
							character->UpdateFacing();
						}
					}
				}
			}

			if(MyData::npcspritesync) {
				if(npcsprite->type == nx_json_type::NX_JSON_OBJECT) {
					Game_Event* character = Game_Map::GetEvent(nx_json_get(npcsprite, "id")->num.u_value);
					if(character) {
						character->data()->sprite_name = nx_json_get(npcsprite, "sheet")->text_value;
						character->data()->sprite_id = nx_json_get(npcsprite, "index")->num.u_value;
					}
				}
			}

			if(MyData::npcactivitysync) {
				if(npcactive->type == nx_json_type::NX_JSON_OBJECT) {
					Game_Event* character = Game_Map::GetEvent(nx_json_get(npcactive, "id")->num.u_value);
					if(character) {
						character->data()->active = nx_json_get(npcactive, "active")->num.u_value;
					}
				}
			}
		}
	}
}


}
#endif
