#include "game_multiplayer_player_tracker.h"
#include <cmath>
#include "cache.h"
#include "text.h"
#include "font.h"

namespace Game_Multiplayer
{
	BitmapRef PlayerTracker::trackerGraphic;

	PlayerTracker::PlayerTracker() : Drawable(Priority_Window, Drawable::Flags::Global) {
		if(!PlayerTracker::trackerGraphic.get()) {
			Rect rect = Font::Default()->GetSize("$s");
			PlayerTracker::trackerGraphic = Bitmap::Create(rect.width, rect.height);
			Text::Draw(*PlayerTracker::trackerGraphic, 0, 0, *Font::Default(), *Cache::SystemOrBlack(), 0, "$s");
		}
		DrawableMgr::Register(this);
	}

	void PlayerTracker::Draw(Bitmap& dst) {
		for(std::string name : this->targetNames) {
			std::string uuid = "";
			
			for(auto& p : other_players) {
				if(p.second.nickname == name) {
					uuid = p.first;
					break;
				}
			}

			if(lastPositions.find(name) == lastPositions.cend() && uuid.length() == 0) continue;

			std::pair<int, int>& lastPosition = lastPositions[name];

			if(uuid.length()) {
				MPPlayer& target = other_players[uuid];
				lastPosition = std::pair<int, int>(target.ch->GetSpriteX(), target.ch->GetSpriteY());
			}

			Rect rect = PlayerTracker::trackerGraphic->GetRect();

			float x = lastPosition.first - Main_Data::game_player->GetSpriteX();
			float y = lastPosition.second - Main_Data::game_player->GetSpriteY();
			float l = std::sqrt(x*x + y*y);
			float r = TILE_SIZE*1.3f;
			float a = M_PI - std::atan2(x, y);
			dst.RotateZoomOpacityBlit(Main_Data::game_player->GetScreenX() + x / l * r, Main_Data::game_player->GetScreenY() + y / l * r - TILE_SIZE, rect.width/2, rect.height/2, *PlayerTracker::trackerGraphic, rect, a, 1, 1, Opacity::Opaque());
		}
	}

	void PlayerTracker::ResetLastPositions() {
		this->lastPositions.clear();
	}

	std::unique_ptr<PlayerTracker> trackerRenderer;
}
