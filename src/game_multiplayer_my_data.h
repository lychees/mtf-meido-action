#include <string>
#include <set>

namespace Game_Multiplayer {
	namespace MyData {
		extern std::string username;
		extern int playersVolume;
		extern bool shouldsync;

		extern int switchsync;
		extern std::set<int> syncedswitches;
		//set of switch ids that souldn't be logged
		extern std::set<int> switchlogblacklist; 

		//used for custom sprites
		extern std::string spritesheet;
		extern int spriteid;

		//adding delay before setting weather since i dont fucking understand when it sets weather on entering another room
		//todo: move it to update? make it static var in update function
		extern uint8_t weatherSetDelay;
		extern uint8_t weatherT;
		extern int nextWeatherType;
		extern int nextWeatherStrength;
		extern bool syncnpc;
		extern bool sfxsync;
		extern bool systemsync;
		extern int sfxfalloff;
		extern bool rendernametags;
		extern bool execeventsync;
		extern bool npcspritesync;
		extern bool npcactivitysync;

		extern bool flashpause;
		#define HostedNpcArrayCapacity 100000
		extern bool hostednpc[HostedNpcArrayCapacity];

	}
}