/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include "Window_SelectSlider.h"
#include "color.h"
#include "bitmap.h"
#include "util_macro.h"
#include "game_party.h"
#include "game_system.h"
#include "input.h"
#include "player.h"
#include <iostream>

Window_SelectSlider::Window_SelectSlider(std::vector<std::string> in_commands, std::vector<std::string> in_discriptions, std::vector<std::string> in_pictures, int x) :
	Window_Base(x, 70, 320, 100)
{
	ReplaceCommands(std::move(in_commands));
	discriptions = std::move(in_discriptions);
	pictures = std::move(in_pictures);
}

void Window_SelectSlider::CreateContents() {
	int w = std::max(0, width - border_x * 2);
	int h = std::max(0, std::max(height - border_y * 2, 100));

	SetContents(Bitmap::Create(w, h));
}

// Properties

int Window_SelectSlider::GetIndex() const {
	return index;
}
int Window_SelectSlider::GetItemMax() const {
    return item_max;
}
void Window_SelectSlider::SetIndex(int nindex) {
	index = min(nindex, item_max - 1);
	if (active && help_window != NULL) {
		UpdateHelp();
	}
	UpdatePicture();
    UpdateText();
}
int Window_SelectSlider::GetRowMax() const {
	return (item_max - 1);
}

void Window_SelectSlider::DrawLine(std::string line) {
    contents->TextDraw(Player::menu_offset_x + 70, 16 * curr_row + 2, Font::ColorDefault, line);
}

void Window_SelectSlider::UpdateText() {
    contents->ClearRect(Rect(Player::menu_offset_x + 70, 0, contents->GetWidth() - 0, 96));
    curr_row = 0;
    std::string line = "";
    for (int i = 0; i < discriptions[index].size(); i++) {
        if (discriptions[index][i] == '\n') {
            DrawLine(line);
            curr_row++;
            line = "";
        } else {
            line.push_back(discriptions[index][i]);
        }
    }
    if (!line.empty()) DrawLine(line);
}
/*
int Window_SelectSlider::GetTopRow() const {
	return oy / menu_item_height;
}
void Window_SelectSlider::SetTopRow(int row) {
	if (row < 0) row = 0;
	if (row > GetRowMax() - 1) row = GetRowMax() - 1;
	SetOy(row * menu_item_height);
}
int Window_SelectSlider::GetPageRowMax() const {
	return (height - border_y * 2) / menu_item_height;
}
int Window_SelectSlider::GetPageItemMax() {
	return GetPageRowMax() * column_max;
}
*/
Rect Window_SelectSlider::GetItemRect(int index) {
	Rect rect = Rect();
	rect.width = (width - 16);
	rect.x = (index % column_max * (rect.width + 16));
	rect.height = 100-4;
	rect.y = index * 100 + 100 / 8;
	return rect;
}

Window_Help* Window_SelectSlider::GetHelpWindow() {
	return help_window;
}

void Window_SelectSlider::SetHelpWindow(Window_Help* nhelp_window) {
	help_window = nhelp_window;
	if (active && help_window != NULL) {
		UpdateHelp();
	}
}

void Window_SelectSlider::UpdateHelp() {
	if (UpdateHelpFn && help_window != nullptr) {
		UpdateHelpFn(*help_window, index);
	}
}
/*
// Update Cursor Rect
void Window_SelectSlider::UpdateCursorRect() {
	int cursor_width = 0;
	int x = 0;
	if (index < 0) {
		SetCursorRect(Rect());
		return;
	}
	int row = index;
	if (row < GetTopRow()) {
		SetTopRow(row);
	} else if (row > GetTopRow() + (GetPageRowMax() - 1)) {
		SetTopRow(row - (GetPageRowMax() - 1));
	}

	cursor_width = (width - 16) + 8;
	x = (index % column_max * (cursor_width + 8)) - 4;

	int y = index * 100 - oy;
	SetCursorRect(Rect(x, y, cursor_width, 100));
}
*/
/*
void Window_SelectSlider::UpdateArrows() {
	bool show_up_arrow = (GetTopRow() > 0);
	bool show_down_arrow = (GetTopRow() < (GetRowMax() - GetPageRowMax()));

	if (show_up_arrow || show_down_arrow) {
		arrow_frame = (arrow_frame + 1) % (arrow_animation_frames * 2);
	}
	bool arrow_visible = (arrow_frame < arrow_animation_frames);
	SetUpArrow(show_up_arrow && arrow_visible);
	SetDownArrow(show_down_arrow && arrow_visible);
}
*/

void Window_SelectSlider::UpdatePicture() {
	picture_window->Set(pictures[index]);
}

// Update
void Window_SelectSlider::Update() {
	Window_Base::Update();
	if (!IsMovementActive()) {
		UpdateText();
    	UpdatePicture();
	}
}

void Window_SelectSlider::SetPictureWindow(Window_Picture* npicture_window) {
	picture_window = npicture_window;
}

void Window_SelectSlider::ReplaceCommands(std::vector<std::string> in_commands) {
	commands = std::move(in_commands);
	index = 0;
	item_max = commands.size();
	const int num_contents = item_max > 0 ? item_max : 1;
	SetContents(Bitmap::Create(this->width - 16, num_contents * 100));
}