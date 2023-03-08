#include "scene.h"
#include "window.h"
#include "window_base.h"
#include "window_help.h"
#include "window_selectable.h"
#include "font.h"
#include "input.h"

namespace Game_Multiplayer
{
	class SettingsItem {
		public:

		std::string name;
		std::string text_right;
		std::string help;

		Font::SystemColor color;
		Font::SystemColor color_right;

		virtual void HandleAction(Input::InputButton action) = 0;
		virtual ~SettingsItem() = default;
	};

	class Window_Multiplayer : public Window_Selectable {
		public:
		Window_Multiplayer(int ix, int iy, int iwidth, int iheight);
		void Update() override;
		void Refresh();

		void Action(Input::InputButton action);

		private:
		void DrawItems();
		void DrawItem(int index);
		std::vector<std::unique_ptr<SettingsItem>> items;
	};

	class Scene_MultiplayerSettings : public Scene {
		public:

		Scene_MultiplayerSettings();
		void Start() override;
		void Update() override;

		private:
	
		std::unique_ptr<Window_Multiplayer> settings_window;
		std::unique_ptr<Window_Help> help_window;
	};

	class ActionOption : public SettingsItem {
		public:
		void HandleAction(Input::InputButton action) override;
		ActionOption(const std::string& name, const std::string& help, bool (*onAction) (SettingsItem* item, Input::InputButton action));

		private:
		bool (*onAction) (SettingsItem* item, Input::InputButton action);
	};

	class SwitchOption : public SettingsItem {
		public:
		void HandleAction(Input::InputButton action) override;
		SwitchOption(const std::string& name, const std::string& help, bool* switch_ref);

		private:
		bool* switch_ref;
	};

	class RangeOption : public SettingsItem {
		public:
		void HandleAction(Input::InputButton action) override;
		RangeOption(const std::string& name, const std::string& help, int* range, int min, int max, int step = 1);

		private:
		int min, max, step;
		int* range;
	};
} // namespace Game_Multiplayer
