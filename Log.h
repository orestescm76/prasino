#pragma once
#include <string>
#include <fstream>

namespace PAG
{
	static enum class msgType
	{
		INFO,
		WARNING,
		ERROR,
		OK
	};

	class Log
	{
	private:
		static Log* instance;
		Log();
		~Log();
		std::ofstream file;
	public:
		static Log* getInstance();
		//Prints a meesage to the console, for constants
		void printMessage(msgType _msgType, std::string msg);
	};
}

