
extern "C" {
	void SetWSHost(const char* host);
	
	////Chat

	void gotMessage(const char* name, const char* trip, const char* msg, const char* src);
	void gotChatInfo(const char* source, const char* text);
	void ChangeName(const char* name);

	////Switch sync

	void SetSwitchSync(int val);
	void SetSwitchSyncWhiteList(int id, int val);
	void SetSwitchSyncLogBlackList(int id, int val);
	void LogSwitchSyncWhiteList();

	////Commands

	void SlashCommandSetSprite(const char* sheet, int id);
	void SetPlayersVolume(int volume);
	void SwitchNpcSync();	
	void TrackCommand(const char* name);
	void UntrackCommand(const char* name);
};
