#include "nxjson.h"


namespace Game_Multiplayer {
	void HandleReceivedPacket(const char* data);

	void ResolveObjectSyncPacket(const nx_json* json);

	void HandleDisconnect(const nx_json* json);
}