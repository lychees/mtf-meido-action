#ifdef EMSCRIPTEN
#include "game_multiplayer_nametags.h"
#include "game_multiplayer_my_data.h"
#include <map>
#include <memory>
#include <queue>
#include <set>

#include "chat_multiplayer.h"
#include "nxjson.h"

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

unsigned long DrawableNameTags::coordHash(int x, int y) {
	// uniquely map integers to whole numbers
	unsigned long a = 2*x;
	if(x < 0) { a = -2*x-1; }
	unsigned long b = 2*y;
	if(y < 0) { b = -2*y-1; }
	// Cantor pairing function
	return ((a+b)*(a+b+1))/2+b;
}

void DrawableNameTags::buildTagGraphic(Tag* tag) {
	Rect rect = Font::Default()->GetSize(tag->name);
	tag->renderGraphic = Bitmap::Create(rect.width+1, rect.height+1);
	Color shadowColor = Color(0, 0, 0, 255); // shadow color
	Text::Draw(*tag->renderGraphic, 1, 1, *Font::Default(), shadowColor, tag->name); // draw black fallback shadow
	Text::Draw(*tag->renderGraphic, 0, 0, *Font::Default(), (tag->system.length() && Game_Multiplayer::MyData::systemsync) ? *Cache::System(tag->system) : *Cache::SystemOrBlack(), 0, tag->name);
}

DrawableNameTags::DrawableNameTags() : Drawable(Priority_Window, Drawable::Flags::Global) {
	DrawableMgr::Register(this);
}

void DrawableNameTags::Draw(Bitmap& dst) {
	if(!Game_Multiplayer::MyData::rendernametags) return;

	const unsigned int stackDelta = 8;
	for(auto& it : nameStacks) {
		unsigned int nTags = it.second.stack.size();
		if(nTags >= 10) {
			// swaying
			it.second.swayAnim += 0.01;
		} else {
			it.second.swayAnim = 0;
		}
		for(int i = 0; i < nTags; i++) {
			Tag* tag = it.second.stack[i];
			auto rect = tag->renderGraphic->GetRect();
			if(it.second.swayAnim == 0) {
				// steady stack
				dst.Blit(tag->anchor->GetScreenX()-rect.width/2, tag->anchor->GetScreenY()-rect.height-TILE_SIZE*1.75-stackDelta*i, *tag->renderGraphic, rect, Opacity::Opaque());
			} else {
				// jenga tower be like
				float sway = sin(it.second.swayAnim);
				float swayRadius = 192/sway;
				float angleStep = stackDelta/swayRadius;
				int rx = tag->anchor->GetScreenX();
				int ry = tag->anchor->GetScreenY()-TILE_SIZE*1.75;
				rx = rx+(cos(i*angleStep)-1)*swayRadius;
				ry = ry-sin(i*angleStep)*swayRadius;
				float angle = -angleStep*i;
				dst.RotateZoomOpacityBlit(rx, ry, rect.width/2, rect.height, *tag->renderGraphic, rect, angle, 1, 1, Opacity::Opaque());
			}
		}
	}
}

void DrawableNameTags::createNameTag(std::string uid, Game_Character* anchor) {
	assert(!nameTags.count(uid)); // prevent creating nametag for uid that already exists

	std::unique_ptr<Tag> tag = std::make_unique<Tag>();
	tag->anchor = anchor;
	tag->name = "";
	tag->system = "";
	buildTagGraphic(tag.get());

	nameStacks[coordHash(tag->x, tag->y)].stack.push_back(tag.get());
	nameTags[uid] = std::move(tag);
}

void DrawableNameTags::deleteNameTag(std::string uid) {
	assert(nameTags.count(uid)); // prevent deleting non existent nametag

	Tag* tag = nameTags[uid].get();
	std::vector<Tag*>& stack = nameStacks[coordHash(tag->x, tag->y)].stack;
	stack.erase(std::remove(stack.begin(), stack.end(), tag), stack.end());
	nameTags.erase(uid);
}

void DrawableNameTags::clearNameTags() {
	nameTags.clear();
	nameStacks.clear();
}

void DrawableNameTags::moveNameTag(std::string uid, int x, int y) {
	assert(nameTags.count(uid)); // prevent moving non existent nametag

	Tag* tag = nameTags[uid].get();
	std::vector<Tag*>& stack = nameStacks[coordHash(tag->x, tag->y)].stack;
	stack.erase(std::remove(stack.begin(), stack.end(), tag), stack.end());
	tag->x = x;
	tag->y = y;
	nameStacks[coordHash(tag->x, tag->y)].stack.push_back(tag);
}

void DrawableNameTags::setTagName(const std::string& uid, const std::string& name) {
	assert(nameTags.count(uid)); // prevent using non existent nametag
	nameTags[uid]->name = name;
	buildTagGraphic(nameTags[uid].get());
}

void DrawableNameTags::setTagSystem(const std::string& uid, const std::string& system) {
	assert(nameTags.count(uid));

	FileRequestAsync* request = AsyncHandler::RequestFile("System", system);
	Tag* tag = this->nameTags[uid].get();

	systemRequest = request->Bind([this, tag](FileRequestResult* result) {
		tag->system = result->file;
		this->buildTagGraphic(tag);
	});

	request->SetGraphicFile(true);
	request->Start();
}

std::unique_ptr<DrawableNameTags> nameTagRenderer; //global nametag renderer

/*
	================
	================
	================
*/
#endif
