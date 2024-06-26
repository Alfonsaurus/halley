/*****************************************************************\
           __
          / /
		 / /                     __  __
		/ /______    _______    / / / / ________   __       __
	   / ______  \  /_____  \  / / / / / _____  | / /      / /
	  / /      | / _______| / / / / / / /____/ / / /      / /
	 / /      / / / _____  / / / / / / _______/ / /      / /
	/ /      / / / /____/ / / / / / / |______  / |______/ /
   /_/      /_/ |________/ / / / /  \_______/  \_______  /
                          /_/ /_/                     / /
			                                         / /
		       High Level Game Framework            /_/

  ---------------------------------------------------------------

  Copyright (c) 2007-2011 - Rodrigo Braz Monteiro.
  This file is subject to the terms of halley_license.txt.

\*****************************************************************/

#pragma once

#include "halley/text/halleystring.h"
#include "halley/file/path.h"
#include "halley/core/api/system_api.h"

namespace Halley {
	class ComputerData {
	public:
		String computerName;
		String userName;
		String cpuName;
		String gpuName;
		String osName;
		long long RAM = 0;
	};

	class OS {
	public:
		virtual ~OS() {}
		static OS& get();
		static void setInstance(OS* os);
		static OS* createOS();

		virtual void createLogConsole(String name);
		virtual void initializeConsole();
		virtual void displayError(const std::string& cs);

		virtual void onWindowCreated(void* windowHandle);

		virtual ComputerData getComputerData();
		virtual String getUserDataDir();
		virtual String getCurrentWorkingDir();
		virtual String getEnvironmentVariable(const String& name);
		virtual String makeDataPath(String appDataPath, String userProvidedPath);
		virtual Path parseProgramPath(const String& commandLine);
		virtual void createDirectories(const Path& path);
		virtual void atomicWriteFile(const Path& path, const Bytes& data, std::optional<Path> backupOldVersionPath = {});
		virtual std::vector<Path> enumerateDirectory(const Path& path);

		virtual void setConsoleColor(int foreground, int background);
		virtual int runCommand(String command);

		virtual std::shared_ptr<IClipboard> getClipboard();

		virtual void openURL(const String& url);

	private:
		static OS* osInstance;
	};
}
