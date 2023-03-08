#include "game_multiplayer_other_player.h"
#include "bitmap.h"
#include "drawable.h"
#include <set>

namespace Game_Multiplayer {
	class PlayerTracker : Drawable {
		private:
		static BitmapRef trackerGraphic;
		std::set<std::string> targetNames;
		std::map<std::string, std::pair<int, int>> lastPositions;
		public:
		PlayerTracker();
		inline void Track(std::string targetUUID) {
			this->targetNames.emplace(targetUUID);
		}
		inline void Untrack(std::string targetUUID) {
			this->targetNames.erase(targetUUID);
		}
		void Draw(Bitmap& dst);
		void ResetLastPositions();
	};

	//global tracker renderer
	extern std::unique_ptr<PlayerTracker> trackerRenderer;
}