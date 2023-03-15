#include "sliding_puzzle.h"
#include "main_data.h"
#include "game_pictures.h"
#include "game_map.h"
#include "game_system.h"
#include "input.h"
#include "output.h"
#include "rect.h"
#include <vector>
#include <set>
#include <cstdlib>
#include <stack>
#include <numeric>

#include "game_message.h"


namespace SlidingPuzzle {
	using namespace std;
	vector<int> p; int w, h, hw, w0, h0, i0, p0;
	string name;

	bool ok() {
		for (int i=0;i<w*h;++i) {
			if (p[i] != i) return 0;
		}
		return 1;
	}

	Game_Pictures::ShowParams getShowParams(int id, int pos) {
		Game_Pictures::ShowParams z = {};
		z.name = name;
		z.fixed_to_map = true;
		z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = pos / h, j = pos % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int id) {
		Game_Pictures::MoveParams z = {};
		//z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = id / h, j = id % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		z.duration = 1;
		return z;
	}

	void NewGame(string _name, int _w, int _h) {
		name = _name; w = _w; h = _h; w0 = 320 / w, h0 = 240 / h; hw = h*w;
		int id = 0;
		Game_Pictures::ShowParams params = {};
		params.name = name;
		params.fixed_to_map = true;
		i0 = rand() % (w*h);

		for (int i=0;i<w;++i)
			for (int j=0;j<h;++j)
				p.push_back(id++);
		std::random_shuffle(p.begin(), p.end());

		id = 0;
		for (int i=0;i<w;++i) {
			for (int j=0;j<h;++j) {
				if (p[id] == i0) {
					p0 = id;
				} else {
					Main_Data::game_pictures->Show(p[id]+1, getShowParams(p[id], id));
				}
				++id;
			}
		}
	}

	void LeaveGame() {
		for (int i=0;i<hw;++i) if (i != p0) Main_Data::game_pictures->Erase(p[i]+1);
		p.clear();
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
	}

	void Move(int d) {
		if (d == 0) {
			if ((p0+1) % h == 0) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0+1]; swap(p[p0], p[p0+1]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0+=1;
			}
		} else if (d == 2) {
			if (p0 % h == 0) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0-1]; swap(p[p0], p[p0-1]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0-=1;
			}
		} else if (d == 1) {
			if (p0 < h) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0-h]; swap(p[p0], p[p0-h]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0-=h;
			}
		} else if (d == 3) {
			if (p0 + h >= w*h) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0+h]; swap(p[p0], p[p0+h]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0+=h;
			}
		}
		if (ok()) {
			lcf::rpg::Sound sound;
			sound.name = "Key";
			sound.volume = 100;
			sound.tempo = 100;
			sound.balance = 50;

			Main_Data::game_system->SePlay(sound);
			//Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		}
	}

	void Update() {
		if (Input::IsTriggered(Input::UP)) {
			Move(0);
		} else if (Input::IsTriggered(Input::RIGHT)) {
			Move(1);
		} else if (Input::IsTriggered(Input::DOWN)) {
			Move(2);
		} else if (Input::IsTriggered(Input::LEFT)) {
			Move(3);
		} else if (Input::IsTriggered(Input::CANCEL)) {
			LeaveGame();
		}
	}

	bool On() {
		return !p.empty();
	}
}

namespace SlidingPuzzle2048 {
	using namespace std;
	vector<int> id; vector<int> lv;

	int w, h, wh, w0, h0;

	Game_Pictures::ShowParams getShowParams(int pos, int lv, bool merge = false) {
		Game_Pictures::ShowParams z = {};
		z.name = "2048";
		z.fixed_to_map = true;
		z.myRect = {0,(lv-1)*32,32,32};
		z.magnify = merge ? 161 : 0;
		int i = pos / 4, j = pos % 4;
		z.position_x = i*32+16+i*4+4;
		z.position_y = j*32+16+j*4+4;
		// z.map_layer = 123;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int pos) {
		Game_Pictures::MoveParams z = {};
		int i = pos / h, j = pos % h;
		z.position_x = i*w0+w0/2+i*4+4;
		z.position_y = j*h0+h0/2+j*4+4;
		z.duration = 1;
		z.magnify = 100;
		return z;
	}

	void NewTile() {
		set<int> s; for (int i=2;i<=wh+1;++i) s.insert(i);
		vector<int> t; for (int i=0;i<wh;++i) if (!id[i]) t.push_back(i); else s.erase(id[i]);
		if (t.empty()) return;
		int r = rand() % t.size();
		int i = t[r];
		id[i] = *s.begin();
		lv[i] = 1;
		Main_Data::game_pictures->Show(id[i], getShowParams(i, lv[i]));
		Main_Data::game_pictures->Move(id[i], getMoveParams(i));
	}

	void NewGame() {
		w = h = 4; w0 = h0 = 32; wh = w*h; id.resize(wh); lv.resize(wh);

		Game_Pictures::ShowParams background = {};
		background.name = "background";
		background.fixed_to_map = true;
		background.position_x = 74;
		background.position_y = 74;
		Main_Data::game_pictures->Show(1, background);
		NewTile(); NewTile();
	}

	void LeaveGame() {
		for (int i=0;i<wh;++i) if (id[i]) Main_Data::game_pictures->Erase(id[i]);
		Main_Data::game_pictures->Erase(1);

		id.clear(); lv.clear();
		lcf::rpg::Sound sound;
		sound.name = "Key";
		sound.volume = 100;
		sound.tempo = 100;
		sound.balance = 50;
		Main_Data::game_system->SePlay(sound);
	}

	bool isInside(int x, int y) {
		return 0 <= x && x < w && 0 <= y && y < h;
	}

	void Move(int dx, int dy) {
		bool ok = false;
		int x0 = 0, x1 = w, xd = 1, y0 = 0, y1 = h, yd = 1;
		if (dx > 0) x0 = w-1, x1 = -1, xd = -1;
		if (dy > 0) y0 = h-1, y1 = -1, yd = -1;

		for (int x=x0;x!=x1;x+=xd) {
			for (int y=y0;y!=y1;y+=yd) {
				int i = x*h+y; if (!id[i]) continue;
				int i0 = i, lv0 = lv[i];
				int xx = x + dx, yy = y + dy, ii = xx*h+yy;
				while (isInside(xx, yy) && (!id[ii] || lv[ii] == lv[i])) {
					if (lv[i] == lv[ii]) {
						Main_Data::game_pictures->Erase(id[ii]);
						id[ii] = id[i]; lv[ii] += 1; id[i] = lv[i] = 0;
						i = ii; break;
					} else {
						swap(id[i], id[ii]); swap(lv[i], lv[ii]);
						i = ii; xx += dx; yy += dy; ii = xx*h+yy;
					}
				}
				if (i != i0) {
					if (lv[i] != lv0) Main_Data::game_pictures->Show(id[i], getShowParams(i0, lv[i], 1));
					Main_Data::game_pictures->Move(id[i], getMoveParams(i));
					ok = true;
				}
			}
		}

		if (ok) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			NewTile();
		} else {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
		}
	}

	void Update() {
		if (Input::IsTriggered(Input::CANCEL)) {
			LeaveGame();
		} else if (Input::IsTriggered(Input::LEFT)) {
			Move(-1, 0);
		} else if (Input::IsTriggered(Input::RIGHT)) {
			Move(1, 0);
		} else if (Input::IsTriggered(Input::UP)) {
			Move(0, -1);
		} else if (Input::IsTriggered(Input::DOWN)) {
			Move(0, 1);
		}
	}

	bool On() {
		return !id.empty();
	}
}

namespace TowerOfHanoi {
	using namespace std;
	vector<stack<int>> p;
	int pointer, diskid, max;

	Game_Pictures::ShowParams getShowParams(string name, int pos, int high, int rect) {
		Game_Pictures::ShowParams z = {};
		z.name = name;
		z.fixed_to_map = true;
		if(rect) z.myRect = {0,0,rect*4+8,8};
		z.use_transparent_color = true;
		z.position_x = pos*32+8+pos*6;
		z.position_y = 81-high*8+4+24;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int pos, int high) {
		Game_Pictures::MoveParams z = {};
		z.position_x = pos*32+8+pos*6;
		z.position_y = 81-high*8+4+24;
		z.duration = 1;
		z.magnify = 100;
		return z;
	}

	void NewGame() {
		pointer = 1; diskid = 0; max = 3;

		for (int i = 1; i <= 3; i++) {
			stack<int> s;
			p.push_back(s);
			Main_Data::game_pictures->Show(i, getShowParams("rod", i, 3, 0));
		}

		for (int i = max; i > 0; i--) {
			p[0].push(i);
			Main_Data::game_pictures->Show(max+i, getShowParams("disk", 1, max+1-i, i));
		}


		Main_Data::game_pictures->Show(max+4, getShowParams("pointer", 1, 9, 0));

	}

	void Pop(int pos) {
		if(!diskid && !p[pos-1].empty()) {
			diskid = p[pos-1].top();
			p[pos-1].pop();
			Main_Data::game_pictures->Move(max+diskid, getMoveParams(pos, 8));
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		} else Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
	}

	void Push(int pos) {
		if(diskid /*  && p[pos-1].empty()  ||  diskid < p[pos-1].top())*/) {
			p[pos-1].push(diskid);

			int h = p[pos-1].size();
			Main_Data::game_pictures->Move(max+diskid, getMoveParams(pos, h));
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			diskid = 0;
		} else Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
	}

	void Move(int pos) {
		if(diskid) {
			Main_Data::game_pictures->Move(max+diskid, getMoveParams(pos, 8));
		}
		Main_Data::game_pictures->Move(max+4, getMoveParams(pos, 9));
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
	}

	void Update() {
		if (Input::IsTriggered(Input::CANCEL)) {
			LeaveGame();
		} else if (Input::IsTriggered(Input::LEFT)) {
			if(pointer-1 >= 1) {
				pointer -= 1;
				Move(pointer);
			} else Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
		} else if (Input::IsTriggered(Input::RIGHT)) {
			if(pointer+1 <= 3) {
				pointer += 1;
				Move(pointer);
			} else Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
		} else if (Input::IsTriggered(Input::UP)) {
			Pop(pointer);
		} else if (Input::IsTriggered(Input::DOWN)) {
			Push(pointer);
		}
	}

	void LeaveGame() {
		for (int i = 1; i <= max+4; i++) Main_Data::game_pictures->Erase(i);
		p.clear();

		lcf::rpg::Sound sound;
		sound.name = "Key";
		sound.volume = 100;
		sound.tempo = 100;
		sound.balance = 50;
		Main_Data::game_system->SePlay(sound);
	}

	bool On() {
		return !p.empty();
	}
}

namespace MineSweeper {
	using namespace std;
	vector<bool> opened;
	vector<bool> marked;
	vector<bool> mines;
	bool gameovermark;
	int h, w, hw, cx, cy;
	int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

	bool ok() {
		for(int i=0;i<hw;i++) {
			if(mines[i]^marked[i]) return false;
		}
		return true;
	}

	bool inGrid(int x, int y) {
		return 0 <= x && x < h && 0 <= y && y < w;
	}

	int count(int x, int y) {
		int z = 0;
		for (int i=0;i<8;++i) {
			int xx = x + dx[i], yy = y + dy[i];
			if (!inGrid(xx, yy)) continue;
			if (mines[xx*w+yy]) ++z;
		}
		return z;
	}

	void DrawCell(int x, int y) {
		int id = x*w+y;
		Game_Pictures::ShowParams z = {};
		z.name = "minesweeper";
		z.fixed_to_map = true;
		z.magnify = 50;
		z.position_x = x*16+8+x*2+2;
		z.position_y = y*16+8+y*2+2;
		if (opened[id]) {
			if (mines[id]) {
				z.myRect = {0,32*10,32,32};
			} else {
				int c = count(x, y);
				z.myRect = {0,32*c,32,32};
			}
		} else {
			if (marked[id]) {
				z.myRect = {0,32*11,32,32};
			} else {
				z.myRect = {0,32*9,32,32};
			}
		}
		Main_Data::game_pictures->Show(id+1, z);
	}

	void DrawCursor() {
		int x = cx, y = cy;
		int id = x*w+y;
		Game_Pictures::ShowParams z = {};
		z.name = "minesweeper";
		z.position_x = x*16+8+x*2+2;
		z.position_y = y*16+8+y*2+2;
		z.fixed_to_map = true;
		z.magnify = 50;
		z.myRect = {0,32*12,32,32};
		z.top_trans = 33;
		z.bottom_trans = 33;
		Main_Data::game_pictures->Show(hw+1, z);
	}

	void NewGame() {
		h = 8; w = 8; hw = h*w; mines.resize(hw); opened.resize(hw); marked.resize(hw); gameovermark = false;
		vector<int> t; t.resize(hw);
		iota(t.begin(),t.end(),0);
    	random_device rd;
    	mt19937 g(rd());
    	shuffle(t.begin(), t.end(), g);

		for (int i=0;i<10;++i) mines[t[i]] = 1;

		for (int i=0;i<h;++i) {
			for (int j=0;j<w;++j) {
				DrawCell(i, j);
			}
		}
		cx = 4; cy = 4;
		DrawCursor();
	}

	void Move(int dx, int dy) {
		if(gameovermark) return;
		if (!inGrid(cx+dx, cy+dy)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			return;
		}
		cx += dx; cy += dy;
		int x = cx, y = cy;
		Game_Pictures::MoveParams z = {};
		z.magnify = 50;
		z.position_x = x*16+8+x*2+2;
		z.position_y = y*16+8+y*2+2;
		z.top_trans = 33;
		z.bottom_trans = 33;
		z.duration = 1;
		Main_Data::game_pictures->Move(hw+1, z);
	}

	void LeaveGame() {
		for (int i=1;i<=hw+1;i++) Main_Data::game_pictures->Erase(i);
		if(gameovermark) Main_Data::game_pictures->Erase(hw+2);
		mines.clear(); opened.clear(); marked.clear();

		lcf::rpg::Sound sound;
		sound.name = "Key";
		sound.volume = 100;
		sound.tempo = 100;
		sound.balance = 50;
		Main_Data::game_system->SePlay(sound);
	}

	void Open(int x, int y) {
		if(gameovermark) return;
		if (!inGrid(x, y)) return;
		if(mines[x*w+y]) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_EnemyKill));
			gameovermark = true;
			Game_Pictures::ShowParams z = {};
			z.name = "gameover";
			z.fixed_to_map = true;
			z.magnify = 50;
			z.position_x = (w*16+w*2)/2;
			z.position_y = (h*16+h*2)/2;
			Main_Data::game_pictures->Show(hw+2, z);
		}
		int id = x*w + y; if (opened[id]) return;
		opened[id] = true; marked[id] = false; DrawCell(x, y);
		if (!count(x, y)) {
			Open(x+1, y);
			Open(x, y+1);
			Open(x-1, y);
			Open(x, y-1);
		}
	}

	void Mark(int x, int y) {
		if(gameovermark) return;
		int id = x*w + y;
		if(marked[id] == false)
			marked[id] = true;
		else
			marked[id] = false;
		DrawCell(x, y);
		if(ok()) {
			gameovermark = true;
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_UseItem));
		}
	}

	void Update() {
		if (Input::IsTriggered(Input::CANCEL)) {
			LeaveGame();
		} else if (Input::IsTriggered(Input::LEFT)) {
			Move(-1, 0);
		} else if (Input::IsTriggered(Input::RIGHT)) {
			Move(1, 0);
		} else if (Input::IsTriggered(Input::UP)) {
			Move(0, -1);
		} else if (Input::IsTriggered(Input::DOWN)) {
			Move(0, 1);
		} else if (Input::IsRawKeyPressed(Input::Keys::SPACE)) {
			Open(cx, cy);
		} else if (Input::IsTriggered(Input::DECISION)) {
			Mark(cx, cy);
		}
	}

	bool On() {
		return !mines.empty();
	}
}

namespace Nokia {
	using namespace std;
	int offset = 50;
	vector<int> p;
	bool isOn = false;
	int cursor = 0;
	int cursor_x0 = 160;
	int cursor_y0 = 52;

	int screen_x0 = 205;
	int screen_y0 = 115;

	int mode = 0;

	Game_Pictures::ShowParams getShowParams(string name, int x, int y, int magnify, int rect = 0) {
		Game_Pictures::ShowParams z = {};
		z.name = name;
		z.fixed_to_map = true;
		if(rect) {
			z.myRect = {0,0,50,40};
			z.top_trans = 50;
		}
		z.use_transparent_color = true;
		z.position_x = x;
		z.position_y = y;
		z.magnify = magnify;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int x, int y, int magnify, int duration) {
		Game_Pictures::MoveParams z = {};
		z.position_x = x;
		z.position_y = y;
		z.duration = duration;
		z.magnify = magnify;
		z.top_trans = 50;
		return z;
	}

	bool ShowPicture(string name, int x, int y, int magnify = 100, int rect = 0) {
		int id = offset + p.size() + 1;
		p.push_back(id);
		return Main_Data::game_pictures->Show(id, getShowParams(name, x, y, magnify, rect));
	}

	void MovePicture(int id, int x, int y, int magnify = 100, int duration = 1) {
		Main_Data::game_pictures->Move(id, getMoveParams(x, y, magnify, duration));
	}

	void Run() {
		if(!isOn) {
			auto pm = PendingMessage();
			pm.PushLine("Nokia");
			pm.SetIsEventMessage(true);
			Game_Message::SetPendingMessage(std::move(pm));
			//auto& ce = Game_Map::GetCommonEvents()[1];
			//Game_Map::GetInterpreter().Push(&ce);
			cursor = 0; mode = 0;
			ShowPicture("Nokia_Phone", 200, 200, 50);
			ShowPicture("black", cursor_x0, cursor_y0, 100, 1);
			ShowPicture("Nokia_1", 205, 115, 50);
			isOn = true;
		}
	}

	void Leave() {
		if(isOn) {
			Output::Debug("~Leave~");
			//auto& ce = Game_Map::GetCommonEvents()[1];
			//Game_Map::GetInterpreter().Push(&ce);
			for(auto id : p) Main_Data::game_pictures->Erase(id);
			p.clear();
			isOn = false;
		}
	}

	void Move(int d) {
		cursor += d; if (cursor < 0) cursor = 0;
		int x = cursor % 3, y = cursor / 3;
		MovePicture(52, x*48+cursor_x0, y*39+cursor_y0);
	}

	void Update() {
		if (Input::IsTriggered(Input::CANCEL)) {
			Leave();
		} else if (Input::IsTriggered(Input::DECISION)) {
			if (mode == 0) {
				++mode;
				Main_Data::game_pictures->Show(53, getShowParams("Nokia_2", screen_x0, screen_y0, 50));
			} else if (mode == 1) {
				++mode;
				Main_Data::game_pictures->Show(53, getShowParams("Nokia_3", screen_x0, screen_y0, 50));
			} else if (mode == 2) {
				++mode;
				Main_Data::game_pictures->Erase(53);
			} else if (mode == 3) {
				if (cursor == 0) {
					Leave();
					MineSweeper::NewGame();
				} else if (cursor == 1) {
					Leave();
					SlidingPuzzle2048::NewGame();
				} else if (cursor == 2) {
					Leave();
					TowerOfHanoi::NewGame();
				} else if (cursor == 3) {
					Leave();
					SlidingPuzzle::NewGame("AliceDUTCHIE256", 4, 3);
				}
			}
		} if (Input::IsTriggered(Input::LEFT)) {
			Move(-1);
		} else if (Input::IsTriggered(Input::RIGHT)) {
			Move(1);
		} else if (Input::IsTriggered(Input::UP)) {
			Move(-3);
		} else if (Input::IsTriggered(Input::DOWN)) {
			Move(3);
		}
	}

	bool On() {
		return isOn;
	}

}
