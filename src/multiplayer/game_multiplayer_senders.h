#pragma once
#include <string>

class Game_Character;

namespace lcf {
	namespace rpg {
		class Sound;
	}
}

namespace Game_Multiplayer {

	namespace PacketTypes {
		const uint16_t movement = 1;
		const uint16_t sprite = 2;
		const uint16_t sound = 3;
		const uint16_t weather = 4;
		const uint16_t name = 5;
		const uint16_t movementAnimationSpeed = 6;
		const uint16_t variable = 7;
		const uint16_t switchsync = 8;
		const uint16_t animtype = 9;
		const uint16_t animframe = 10;
		const uint16_t facing = 11;
		const uint16_t typingstatus = 12;
		const uint16_t syncme = 13;
		const uint16_t flash = 14;
		const uint16_t flashpause = 15;
		const uint16_t npcmove = 16;
		const uint16_t system = 17;
		const uint16_t npcsprite = 18;
		const uint16_t npcactive = 19;
	};

	void SendPlayerData();

	void SendMainPlayerPos();
	void SendMainPlayerMoveSpeed(int spd);
	void SendMainPlayerSprite(std::string name, int index);
	void SendMainPlayerName();
	//TO-DO:
	//remove MainPlayerMoved, MainPlayerChangedMoveSpeed and MainPlayerChangeSpriteGraphic 
	//and fully replace it with SendMainPlayerPos, SendMainPlayerMoveSpeed and SendMainPlayerSprite
	//TO-DO #2:
	//rename for consistency:
	//SendMainPlayerPos to PositionSync
	//SendMainPlayerMoveSpeed to MoveAnimSpeedSync
	//SendMainPlayerSprite to SpriteSync
	//(SendMainPlayerName to NameSync (not sure about this one since it's related to chat and every other method with "Sync" suffix is related to game))
	/////////////////////////////////////
	
	void MainPlayerMoved(int dir);
	void MainPlayerChangedMoveSpeed(int spd);
	void MainPlayerChangedSpriteGraphic(std::string name, int index);
	

	void SePlaySync(const lcf::rpg::Sound& sound);
	void WeatherEffectSync(int type, int sthrength);
	void VariableSync(int32_t id, int32_t val);
	void SwitchSync(int32_t id, int32_t val);
	void AnimFrameSync(uint16_t frame);
	void FacingSync(uint16_t facing);
	void SetTypingStatus(uint16_t status);
	void FlashSync(int r, int g, int b, int p, int t);
	void FlashPauseSync(bool pause);
	void NpcMoveSync(int x, int y, int facing, int id);
	void NpcMoveSync(int x, int y, int facing, Game_Character* character);
	void SendSystem(std::string name);
	void NpcSpriteSync(Game_Character* character, uint16_t index, std::string sheet);
	void NpcActivitySync(Game_Character* character, bool active);

	void SyncMe();
}