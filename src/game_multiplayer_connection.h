#pragma once
#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>
#include <string>
#include <time.h>

namespace Game_Multiplayer {
	//creates WebSocket connection to game server
	void ConnectToGame();
	//changes room client is connected to
	void ConnectToRoom(int room_id);

	void SetConnStatusWindowText(std::string s);

	//sends data of the string through game WebSocket
	//drops message if socket is not connected
	//even tho we send a string data, message format is still defined by emscripten websocket format
	void TrySend(const std::string& msg);

	//sends binary data trough game WebSocket, drops packet if socket is not connected
	void TrySend(const void* buffer, size_t size);

	namespace ConnectionData {
		extern std::string host;

		extern EMSCRIPTEN_WEBSOCKET_T socket;

		extern time_t lastConnect;
		extern time_t reconnectInterval;
		extern bool connected;
		extern int room_id;

		//not sure if it has in that namespace
		//true before first update in multiplayer main loop after we connected to a new room
		extern bool roomFirstUpdate;
	}
}
#endif
