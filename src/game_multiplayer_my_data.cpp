
#include "game_multiplayer_my_data.h"

namespace Game_Multiplayer {

std::string MyData::username;
int MyData::playersVolume = 50;
bool MyData::shouldsync = false;

int MyData::switchsync = 0;
std::set<int> MyData::syncedswitches = std::set<int>();
std::set<int> MyData::switchlogblacklist = std::set<int>(); //set of switch ids that souldn't be logged

//used for custom sprites
std::string MyData::spritesheet = "";
int MyData::spriteid = 0;

uint8_t MyData::weatherSetDelay = 25;
uint8_t MyData::weatherT = 0;
int MyData::nextWeatherType = -1;
int MyData::nextWeatherStrength = -1;

bool MyData::syncnpc = false;
bool MyData::npcspritesync = false;
bool MyData::npcactivitysync = false;
bool MyData::execeventsync = true;
bool MyData::sfxsync = true;
bool MyData::systemsync = true;
int MyData::sfxfalloff = 16;

bool MyData::rendernametags = true;

bool MyData::flashpause = false;

bool MyData::hostednpc[HostedNpcArrayCapacity];

}