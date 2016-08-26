#pragma once

#include "prec.h"
#include "tasks/editor_task_set.h"

namespace Halley {
	class HalleyEditor;
	class ConsoleWindow;

	class EditorRootStage final : public Stage
	{
	public:
		EditorRootStage(HalleyEditor& editor);
		~EditorRootStage();

		void init() override;
		void onVariableUpdate(Time time) override;
		void onRender(RenderContext& context) const override;

	private:
		HalleyEditor& editor;

		Sprite halleyLogo;
		Sprite background;
		std::unique_ptr<ConsoleWindow> console;
		std::unique_ptr<EditorTaskSet> tasks;

		void initSprites();
	};
}
