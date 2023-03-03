#ifndef SLIDING_PUZZLE_H
#define SLIDING_PUZZLE_H

#include <string>

namespace SlidingPuzzle {
	void NewGame(std::string, int, int);
	void LeaveGame();
	void Update();
	bool On();
}

namespace SlidingPuzzle2048 {
	void NewGame();
	void LeaveGame();
	void Update();
	bool On();
}

namespace TowerOfHanoi {
	void NewGame();
	void LeaveGame();
	void Update();
	bool On();
}

namespace MineSweeper {
	void NewGame();
	void LeaveGame();
	void Update();
	bool On();
}

namespace Nokia {
	void Run();
	void Update();
	bool On();
	void Leave();
}
#endif
