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
#include "bitmap.h"
#include "game_actors.h"
#include "window_picture.h"
#include "cache.h"

Window_Picture::Window_Picture(int ix, int iy, int iwidth, int iheight, double izoom_x, double izoom_y) :
	Window_Base(ix, iy, iwidth, iheight) {
	zoom_x = izoom_x;
	zoom_y = izoom_y;
	SetContents(Bitmap::Create(this->width - 8, this->height - 8));
}

void Window_Picture::OnPictureReady(FileRequestResult* result, int cx, int cy, bool flip) {
	BitmapRef picture = Cache::Picture(result->file, true);

	const int sw = picture->GetWidth();
	const int sh = picture->GetHeight();

	Rect src_rect(
		0,
		0,
		sw,
		sh
		);

	if (flip) {
		contents->FlipBlit(cx, cy, *picture, src_rect, true, false, Opacity::Opaque());
	}
	else {
		contents->ZoomOpacityBlit(cx, cy, cx, cy, *picture, src_rect, zoom_x, zoom_y, 255);
	}
}

// All these functions assume that the input is valid

void Window_Picture::DrawPicture(std::string_view picture_name, int cx, int cy, bool flip) {
	if (picture_name.empty()) { return; }

	FileRequestAsync* request = AsyncHandler::RequestFile("Picture", picture_name);
	request->SetGraphicFile(true);
	picture_request_ids.push_back(request->Bind(&Window_Picture::OnPictureReady, this, cx, cy, flip));
	request->Start();
}

void Window_Picture::Refresh() {
	contents->Clear();
	// Actor data is guaranteed to be valid
	DrawPicture(picture_name, 0, 0);
}

void Window_Picture::Set(const std::string name) {
	picture_name = name;
	Refresh();
}
/*
std::string Window_Picture::Get() {
	return picture_name;
}
*/