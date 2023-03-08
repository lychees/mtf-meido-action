#ifdef EMSCRIPTEN
#include <string>
#include "scene.h"
#include "game_multiplayer_other_player.h"
#include "game_multiplayer_nametags.h"
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
#include "scene_map.h"

namespace Game_Multiplayer {

void GetClosestPlayerCoords(int x, int y, int& outx, int& outy) {
	int w = Game_Map::GetWidth();
	int h = Game_Map::GetHeight();

	int mpx = Main_Data::game_player->GetX();
	int mpy = Main_Data::game_player->GetY();

	//calculate distance from main player
	int mlx = std::min(abs(x - mpx), abs(x - w - mpx));
	int mly = std::min(abs(y - mpy), abs(y - h - mpy));

	//min distance found
	int mdst = mlx + mly;

	outx = mpx;
	outy = mpy;

	//calculate distance to each player and find the closest one
	for(auto& p : other_players) {
		int px = p.second.ch->GetX();
		int py = p.second.ch->GetY();

		int lx = std::min(abs(x - px), abs(x - w - px));
		int ly = std::min(abs(y - py), abs(y - h - py));

		int distance = lx + ly;

		if(distance < mdst) {
			mdst = distance;
			outx = px;
			outy = py;
		}
	}
}


void ErasePlayer(const std::string& uid) {
	auto scene_map = Scene::Find(Scene::SceneType::Map);
	auto old_list = &DrawableMgr::GetLocalList();

	DrawableMgr::SetLocalList(&scene_map->GetDrawableList());

	nameTagRenderer->deleteNameTag(uid);

	other_players.erase(uid);

	DrawableMgr::SetLocalList(old_list);
};

MPPlayer& CreatePlayer(std::string uid) {
	//get main player
	auto& main_player = Main_Data::game_player;
	//
	MPPlayer& new_player = other_players[uid];
	new_player.flashpause = 0;

	auto& new_player_character = new_player.ch;
	new_player_character = std::make_shared<Game_PlayerOther>();

	//copy most of the data from main player to new player
	//it makes so that new player appears behind main player and not visible for a split second before actuall data received
	//another idea is to tp player far away or make it invisible
	new_player_character->SetX(main_player->GetX());
	new_player_character->SetY(main_player->GetY());
	new_player_character->SetSpriteGraphic(main_player->GetSpriteName(), main_player->GetSpriteIndex());
	new_player_character->SetMoveSpeed(main_player->GetMoveSpeed());
	new_player_character->SetMoveFrequency(main_player->GetMoveFrequency());
	new_player_character->SetThrough(true);
	new_player_character->SetLayer(main_player->GetLayer());
	new_player_character->SetFacing(main_player->GetFacing());

	nameTagRenderer->createNameTag(uid, new_player_character.get());

	auto old_list = &DrawableMgr::GetLocalList();
	auto scene_map = Scene::Find(Scene::SceneType::Map);
	DrawableMgr::SetLocalList(&scene_map->GetDrawableList());
	new_player.sprite = std::make_unique<Sprite_Character>(new_player_character.get());
	new_player.sprite->SetTone(Main_Data::game_screen->GetTone());
	DrawableMgr::SetLocalList(old_list);

	return new_player;
}

//clears players and nametags
void ClearPlayers() {
	other_players.clear();
	nameTagRenderer->clearNameTags();
}

MPPlayer& GetPlayerOrCreate(std::string uid) {
	auto player = other_players.find(uid);
	if(player == other_players.cend()) {
		return CreatePlayer(uid);
	}

	return player->second;
}



void FlashAll(int r, int g, int b, int p, int t) {
	for (auto& mpplayer : other_players) {
		mpplayer.second.ch->Flash(r, g, b, p, t);
	}
}

void TintAll() {
	for (auto& mpplayer : other_players) {
		mpplayer.second.sprite->SetTone(Main_Data::game_screen->GetTone());
	}
}

	std::map<std::string, MPPlayer> other_players = std::map<std::string, MPPlayer>();

}
#endif
