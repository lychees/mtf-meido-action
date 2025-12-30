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

#ifndef EP_WINDOW_SELECTSLIDER_H
#define EP_WINDOW_SELECTSLIDER_H

// Headers
#include <functional>
#include <vector>
#include "window_base.h"
#include "window_picture.h"
#include "window_help.h"
#include "font.h"

/**
 * Window Command class.
 */
class Window_SelectSlider: public Window_Base {
public:
	/**
	 * Constructor.
	 *
	 * @param commands commands to display.
	 * @param width window width, if no width is passed
	 *              the width is autocalculated.
	 * @param max_item forces a window height for max_item
	 *                 items, if no height is passed
	 *                 the height is autocalculated.
	 */
	Window_SelectSlider(std::vector<std::string> commands, std::vector<std::string> discriptions, std::vector<std::string> pictures, int x);

    void CreateContents();

	int GetIndex() const;
    int GetItemMax() const;
	void SetIndex(int nindex);
	int GetRowMax() const;

	/** Change the max item */
	void SetItemMax(int value);

	/**
	 * Returns the Item Rect used for item drawing.
	 *
	 * @param index index of item.
	 * @return Rect where the item is drawn.
	 */
	virtual Rect GetItemRect(int index);

	/**
	 * Function called by the base UpdateHelp() implementation.
	 * Passes in the Help Window and the current selected index
	 * Will not be called if the help_window is null
	 */
	std::function<void(Window_Help&, int)> UpdateHelpFn;

	Window_Help* GetHelpWindow();

	/**
	 * Assigns a help window that displays a description
	 * about the selected item.
	 *
	 * @param nhelp_window the help window.
	 */
	void SetHelpWindow(Window_Help* nhelp_window);
	//virtual void UpdateCursorRect();
	void Update() override;

	virtual void UpdateHelp();

    virtual void UpdatePicture();

    void DrawLine(std::string line);

    void UpdateText();


	/**
	 * Replace all commands with a new command set.
	 *
	 * @param commands the commands to replace with
	 * @note auto-generating width and height is not supported.
	 */
	void ReplaceCommands(std::vector<std::string> commands);

	void SetPictureWindow(Window_Picture* npicture_window);


protected:
    Window_Help* help_window = nullptr;
	int item_max = 1;
	int column_max = 1;
	int index = -1;
	int arrow_frame = 0;
    int curr_row = 0;
	std::vector<std::string> commands;
	std::vector<std::string> discriptions;
	std::vector<std::string> pictures;
    Window_Picture*  picture_window = nullptr;

    int scroll_dir = 0;
	int scroll_progress = 0;

	//virtual void DrawItem(int index, Font::SystemColor color);
};

inline void Window_SelectSlider::SetItemMax(int value) {
	item_max = value;
}

#endif
