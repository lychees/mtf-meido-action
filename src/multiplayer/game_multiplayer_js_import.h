//we can just call any ws function by using EM_ASM
//so "importing" might not actually make sense
//but what we actually do is just wrap js code in c++ function
//i guess?

extern "C" {
	void SendChatMessage(const char* msg);
};