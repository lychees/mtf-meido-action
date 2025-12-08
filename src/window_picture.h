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

#ifndef EP_WINDOW_PICTURE_H
#define EP_WINDOW_PICTURE_H

// Headers
#include <string>
#include "window_base.h"
#include "async_handler.h"

/**
 * Window Face Class.
 */
class Window_Picture :	public Window_Base {
public:
	/**
	 * Constructor.
	 */
	Window_Picture(int ix, int iy, int iwidth, int iheight);

	/**
	 * Renders the current face on the window.
	 */
	void Refresh();

	void Set(const std::string picture_name);

    void DrawPicture(std::string_view face_name, int cx, int cy, bool flip = false);

protected:
	std::string picture_name = "";
    std::vector<FileRequestBinding> picture_request_ids;

    void OnPictureReady(FileRequestResult* result, int cx, int cy, bool flip);
};

#endif