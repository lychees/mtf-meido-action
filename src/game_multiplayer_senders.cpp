#ifdef EMSCRIPTEN
#include "game_multiplayer_senders.h"
#include "game_multiplayer_js_export.h"
#include "game_multiplayer_my_data.h"
#include "game_multiplayer_other_player.h"
#include "game_multiplayer_connection.h"
#include "game_player.h"

#include <map>
#include <memory>
#include <queue>
#include <set>

#include "output.h"
#include "game_player.h"
#include "sprite_character.h"
#include "window_base.h"
#include "drawable_mgr.h"
#include "scene.h"
#include "bitmap.h"
#include "font.h"
#include "input.h"
#include "nxjson.h"
#include "cache.h"
#include "game_screen.h"
#include "game_variables.h"
#include "game_switches.h"
#include "game_map.h"
#include "player.h"

namespace Game_Multiplayer {

#define SEND_BUFFER_SIZE 8192
char sendBuffer[SEND_BUFFER_SIZE];


void SendPlayerData() {
	auto& player = Main_Data::game_player;
	SendMainPlayerPos();
	SendMainPlayerSprite(player->GetSpriteName(), player->GetSpriteIndex());
	SendMainPlayerName();
	SendMainPlayerMoveSpeed((int)(player->GetMoveSpeed()));

	if(MyData::spritesheet != "")
		SlashCommandSetSprite( MyData::spritesheet.c_str(),  MyData::spriteid);
}

void SendMainPlayerPos() {
	auto& player = Main_Data::game_player;
	uint16_t cmsg[3] = {
		PacketTypes::movement,
		(uint16_t)player->GetX(),
		(uint16_t)player->GetY()
	};
	TrySend((void*)cmsg, sizeof(uint16_t) * 3);
}

void SendMainPlayerName() {
	if(MyData::username.length() == 0)
		return;
	int s = MyData::username.length() + sizeof(uint16_t);
	memcpy(sendBuffer, &PacketTypes::name, sizeof(uint16_t));
	memcpy(sendBuffer + sizeof(uint16_t), MyData::username.c_str(), MyData::username.length());
	TrySend(sendBuffer, s);
}

void SendMainPlayerMoveSpeed(int spd) {
	uint16_t m[2] = {PacketTypes::movementAnimationSpeed, (uint16_t)spd};
	TrySend(m, 4);
}


void SendMainPlayerSprite(std::string name, int index) {
	//sprite packet [uint16_t, uint16_t, null terminated string] (packet type, sprite id, sprite sheet)

	uint16_t msgprefix[2] = {
		PacketTypes::sprite,
		(uint16_t)index,
	};
	size_t s = sizeof(char) * (name.length()) + sizeof(uint16_t) * 2;

	memcpy(sendBuffer, msgprefix, sizeof(uint16_t) * 2);
	memcpy(sendBuffer + sizeof(uint16_t) * 2, name.c_str(), name.length());
	TrySend(sendBuffer, s);
}


void MainPlayerMoved(int dir) {
	SendMainPlayerPos();
}

void MainPlayerChangedMoveSpeed(int spd) {
	SendMainPlayerMoveSpeed(spd);
}

void MainPlayerChangedSpriteGraphic(std::string name, int index) {
	SendMainPlayerSprite(name, index);
}

void SePlaySync(const lcf::rpg::Sound& sound) {
	if(sound.volume == 0)
		return;
	size_t s = sizeof(uint16_t) * 4 + sound.name.length();
	uint16_t vtd[4] = {PacketTypes::sound, (uint16_t)sound.volume, (uint16_t)sound.tempo, (uint16_t)sound.balance};
	memcpy(sendBuffer, vtd, sizeof(uint16_t) * 4);
	memcpy(sendBuffer + sizeof(uint16_t) * 4, sound.name.c_str(), sound.name.length());
	TrySend(sendBuffer, s);
}

void WeatherEffectSync(int type, int strength) {
	MyData::nextWeatherType = type;
	MyData::nextWeatherStrength = strength;
}

void VariableSync(int32_t id, int32_t val) {
	//will not use it untill whitelist for variables is added
	return;
	//uint16_t ptype = PacketTypes::variable;
	//memcpy(sendBuffer, &ptype, sizeof(uint16_t));
	//int32_t m[2] = {id, val};
	//memcpy(sendBuffer + sizeof(uint16_t), m, sizeof(int32_t) * 2);
	//TrySend(&sendBuffer, sizeof(int16_t) * 5);
}

void SwitchSync(int32_t id, int32_t val) {
	if( MyData::switchsync) {
		if( MyData::syncedswitches.find(id) !=  MyData::syncedswitches.cend()) {
			uint16_t ptype = PacketTypes::switchsync;
			memcpy(sendBuffer, &ptype, sizeof(uint16_t));
			int32_t m[2] = {id, val};
			memcpy(sendBuffer + sizeof(uint16_t), m, sizeof(int32_t) * 2);
			TrySend(&sendBuffer, sizeof(uint16_t) * 5);
		}
		std::string setswtstr = std::to_string(id) + " " + std::to_string(val);
		if(MyData::switchlogblacklist.find(id) == MyData::switchlogblacklist.cend()) {
			EM_ASM({
				console.log("my switch " + UTF8ToString($0));
			}, setswtstr.c_str());
		}
	}
}

void AnimFrameSync(uint16_t frame) {
	uint16_t m[] = {PacketTypes::animframe, (uint16_t)frame};
	TrySend(m, sizeof(uint16_t) * 2);
}

void FacingSync(uint16_t facing) {
	uint16_t m[] = {PacketTypes::facing, (uint16_t)facing};
	TrySend(m, sizeof(uint16_t) * 2);
}

void SetTypingStatus(uint16_t status) {
	uint16_t m[] = {PacketTypes::typingstatus, status};
	TrySend(m, sizeof(uint16_t) * 2);
}

void FlashSync(int r, int g, int b, int p, int t) {
	if(!MyData::flashpause) {
		uint16_t m[] = {PacketTypes::flash, (uint16_t)r, (uint16_t)g, (uint16_t)b, (uint16_t)p, (uint16_t)t};
		TrySend(m, sizeof(uint16_t) * 6);
	}
}

void FlashPauseSync(bool pause) {
	if(MyData::flashpause != pause) {
		uint16_t m[] = {PacketTypes::flashpause, (uint16_t)pause};
		TrySend(m, sizeof(uint16_t) * 2);
		MyData::flashpause = pause;
	}
}

void NpcMoveSync(int x, int y, int facing, Game_Character* character) {
	NpcMoveSync(x, y, facing, ((Game_Event*)character)->GetId());
}

void NpcMoveSync(int x, int y, int facing, int id) {
	if(MyData::hostednpc[id]) {
		uint16_t m[] = {PacketTypes::npcmove, (uint16_t)x, (uint16_t)y, (uint16_t)facing, (uint16_t)id};
		TrySend(m, sizeof(uint16_t) * 5);
	} else {
		//try to becomne a host for an npc just in case if somebody has turned off their npc sync
		if((rand() % 16) == 0)
			MyData::hostednpc[id] = true;
	}
}

void SendSystem(std::string name) {
	memcpy(sendBuffer, &PacketTypes::system, sizeof(uint16_t));
	memcpy(sendBuffer + sizeof(uint16_t), name.c_str(), name.length());

	TrySend(sendBuffer, sizeof(uint16_t) + name.length());
}

void NpcSpriteSync(Game_Character* character, uint16_t index, std::string sheet) {
	uint16_t m[] = {PacketTypes::npcsprite, (uint16_t)(((Game_Event*)character)->GetId()), index};
	memcpy(sendBuffer + sizeof(uint16_t) * 3, sheet.c_str(), sheet.length());

	TrySend(sendBuffer, sizeof(uint16_t) * 3 + sheet.length());
}

void NpcActivitySync(Game_Character* character, bool active) {
	uint16_t m[] = {PacketTypes::npcactive, (uint16_t)(((Game_Event*)character)->GetId()), (uint16_t)active};
	TrySend(m, sizeof(uint16_t) * 3);
}

void SyncMe() {
	uint16_t m[] = {PacketTypes::syncme, (uint16_t)0};
	TrySend(m, sizeof(uint16_t) * 2);
}

}
#endif
