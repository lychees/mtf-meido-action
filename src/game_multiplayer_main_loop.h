#include <list>
namespace Game_Multiplayer {
	//list of npc moves
	extern std::list<std::pair<int, std::pair<int, int>>> npcmoves;
	void Update();
}