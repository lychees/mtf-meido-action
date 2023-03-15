
#include "game_multiplayer_rng.h"
#include "game_multiplayer_my_data.h"
#include "rand.h"

namespace Game_Multiplayer {
	unsigned int room_seed = 623485727;
	unsigned int funny_values[] = {1023708963, 1958922015, 3364503282, 4193438549, 1991946321, 1351978205, 1908595757, 2325041562, 623485727, 1562451344};

	unsigned int FunnyValue(int n) {
		return funny_values[n % 10];
	}
	
	unsigned int GetSyncedRng(unsigned int from, unsigned int to, unsigned int seed2) {

		if(!MyData::syncnpc) {
			return Rand::GetRandomNumber(from, to);
		}

		unsigned int range = to - from;
		if(!range) return 0;
		unsigned int randomint = FunnyValue(to * room_seed + seed2) * (to + seed2) + from * FunnyValue(from * room_seed + seed2) * FunnyValue(FunnyValue(from + room_seed) + to); //random enough?

		return from + randomint % range;
	}
}