#pragma once
#include <string>
#include <halley/data_structures/vector.h>
#include <memory>
#include <functional>
#include <map>

namespace Halley
{
	class CommandLineTool
	{
	public:
		virtual ~CommandLineTool() {}

		int runRaw(int argc, char** argv);
		virtual int run(Vector<std::string> args) = 0;
	};

	class CommandLineTools
	{
	public:
		CommandLineTools();

		Vector<std::string> getToolNames();
		std::unique_ptr<CommandLineTool> getTool(std::string name);

	private:
		using ToolFactory = std::function<std::unique_ptr<CommandLineTool>()>;
		std::map<std::string, ToolFactory> factories;
	};
}