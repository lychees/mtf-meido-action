#pragma once
#ifdef EMSCRIPTEN
#include <string>
#include <map>
#include <memory>
#include <queue>
#include <set>

#include "game_player.h"
#include "sprite_character.h"
#include "window_base.h"
#include "drawable_mgr.h"
#include "scene.h"
#include "game_map.h"
#include "player.h"
#include "game_character.h"


/**
 * Game_PlayerOther class
 * game character of other clients
 */
class Game_PlayerOther : public Game_PlayerBase {
public:
	Game_PlayerOther() : Game_PlayerBase(PlayerOther)
	{
		SetDirection(lcf::rpg::EventPage::Direction_down);
		SetMoveSpeed(4);
		SetAnimationType(lcf::rpg::EventPage::AnimType_non_continuous);
	}

	void UpdateNextMovementAction() override {
		//literally just do nothing
	}

	void UpdateAnimation() override {

	}

	void Update() {
		Game_Character::Update();
	}
};

namespace Game_Multiplayer {

	void GetClosestPlayerCoords(int x, int y, int& outx, int& outy);

	struct MPPlayer {
		std::queue<std::pair<int,int>> mvq; //queue of move commands
		std::shared_ptr<Game_PlayerOther> ch; //character
		uint16_t typingstatus;
		//this one is used to save player speed before setting it to max speed when move queue is too long
		int moveSpeed;
		std::unique_ptr<Sprite_Character> sprite;
		int flashpause;
		std::string nickname;
	};

	extern std::map<std::string, MPPlayer> other_players;

	void ErasePlayer(const std::string& uid);
	MPPlayer& CreatePlayer(std::string uid);
	MPPlayer& GetPlayerOrCreate(std::string uid);
	void ClearPlayers();

	void FlashAll(int r, int g, int b, int p, int t);
	void TintAll();


}
#endif
