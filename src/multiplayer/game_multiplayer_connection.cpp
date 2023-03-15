#ifdef EMSCRIPTEN
#include "chat_multiplayer.h"
#include "game_multiplayer_connection.h"
#include "game_multiplayer_receive_handler.h"
#include "game_multiplayer_senders.h"
#include "game_multiplayer_nametags.h"
#include "game_multiplayer_other_player.h"
#include "game_multiplayer_my_data.h"
#include "game_multiplayer_player_tracker.h"
#include "../game_map.h"
#include "../drawable_mgr.h"
#include "../player.h"

namespace Game_Multiplayer {

///////////////////////////web socket callbacks begin

EM_BOOL onopen(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData);
EM_BOOL onclose(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData);
EM_BOOL onmessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData);

namespace ConnectionData {
	std::string host = "";

	EMSCRIPTEN_WEBSOCKET_T socket;

	time_t lastConnect = 0;
	time_t reconnectInterval = 5;
	bool connected = false;
	int room_id = 0;

	bool roomFirstUpdate = true;
}

void TrySend(const std::string& msg) {
	TrySend(msg.c_str(), msg.length());
}

void TrySend(const void* buffer, size_t size) {
	unsigned short ready;
	emscripten_websocket_get_ready_state(ConnectionData::socket, &ready);

	if (ready == 1) { //1 means OPEN
		emscripten_websocket_send_binary(ConnectionData::socket, (void*)buffer, size);
	}
	MyData::shouldsync = true;
}

void ConnectToGame() {

	ConnectionData::lastConnect = time(NULL);

	SetConnStatusWindowText("Disconnected");
	ConnectionData::connected = false;

	//create emscripten WS//
	EmscriptenWebSocketCreateAttributes ws_attrs = {
		ConnectionData::host.c_str(),
		"binary",
		EM_TRUE
	};

	ConnectionData::socket = emscripten_websocket_new(&ws_attrs);
	emscripten_websocket_set_onopen_callback(ConnectionData::socket, NULL, onopen);
	emscripten_websocket_set_onclose_callback(ConnectionData::socket, NULL, onclose);
	emscripten_websocket_set_onmessage_callback(ConnectionData::socket, NULL, onmessage);
	/////////////////////////
}

//changes the room that client is connected to
void ConnectToRoom(int map_id) {
	//we want to be a host for every npc in the room untill we receive an npc move packet from another player
	//that would indicate that there's already another host for a npc
	for(int i = 0; i < HostedNpcArrayCapacity; i++) {
		MyData::hostednpc[i] = true;
	}

	ConnectionData::roomFirstUpdate = true;
	ConnectionData::room_id = map_id;
	ClearPlayers();

	//send change room packet
	//if we're not connected yet then it would be dropped and that's fine
	//since we call this function again in websocket onopen callback
	uint16_t room_id16[] = {(uint16_t)ConnectionData::room_id};
	TrySend((void*)room_id16, sizeof(uint16_t));

	//connect to local chat and let JS know room id
	EM_ASM({
		ConnectToLocalChat($0);
		SetRoomID($0);
	}, ConnectionData::room_id);

	#if defined(INGAME_CHAT)
		Chat_Multiplayer::setStatusRoom(ConnectionData::room_id);
		Chat_Multiplayer::refresh();
	#endif

	// initialize nametag renderer
	if(nameTagRenderer == nullptr) {
		nameTagRenderer = std::make_unique<DrawableNameTags>();
	}
	if(trackerRenderer == nullptr) {
		trackerRenderer = std::make_unique<PlayerTracker>();
	}
	trackerRenderer->ResetLastPositions();
}

void SetConnStatusWindowText(std::string s) {
	#if defined(INGAME_CHAT)
		Chat_Multiplayer::setStatusConnection(s=="Connected");
	#endif
}

#define RECEIVE_BUFFER_SIZE 8192
char receiveBuffer[RECEIVE_BUFFER_SIZE];

///////////////////////////web socket callbacks begin

EM_BOOL onopen(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData) {
	ClearPlayers();
	SetConnStatusWindowText("Connected");
	ConnectionData::connected = true;

	//tell server that we want to use game handler
	TrySend(Player::emscripten_game_name + "game");

	ConnectToRoom(Game_Map::GetMapId());
	SendPlayerData();

	return EM_TRUE;
}

EM_BOOL onclose(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData) {
	SetConnStatusWindowText("Disconnected");
	ConnectionData::connected = false;

	emscripten_websocket_deinitialize();

	return EM_TRUE;
}


EM_BOOL onmessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData) {
	if(websocketEvent->isText) {
		//i might be doing some extra work here with copying received data to reveiveBuffer and ther passing it into a function that copies that buffer again
		strcpy(receiveBuffer, (char*)websocketEvent->data);
		HandleReceivedPacket(receiveBuffer);
	}
	return EM_TRUE;
}
///////////////////////////web callbacks end

}
#endif
