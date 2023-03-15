#ifndef EP_CHAT_MULTIPLAYER_H
#define EP_CHAT_MULTIPLAYER_H

#include <string>

namespace Chat_Multiplayer {
	void refresh(); // initializes chat or refreshes its theme
	void update(); // called once per logical frame

	void gotMessage(std::string name, std::string trip, std::string msg, std::string src);
	void gotInfo(std::string msg);
	void setStatusConnection(bool status);
	void setStatusRoom(unsigned int roomID);
}

#endif