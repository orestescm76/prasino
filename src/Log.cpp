#include "pch.h"
#include "Log.h"

PAG::Log* PAG::Log::instance = nullptr;

PAG::Log::Log(): file()
{
	file.open("log.txt", std::fstream::out);
}
PAG::Log::~Log()
{
	file.close();
}

PAG::Log* PAG::Log::getInstance()
{
	if (!instance)
	{
		instance = new Log();
		instance->printMessage(msgType::INFO, "Log started");
	}
	return instance;
}

void PAG::Log::printMessage(PAG::msgType _tipoMensaje, std::string msg)
{
	switch (_tipoMensaje)
	{
	case msgType::INFO:
		std::cout << "<INFO>: " << msg << std::endl;
		file << "<INFO>: " << msg << std::endl;
		break;
	case msgType::WARNING:
		std::cout << "<WARNING>: " << msg << std::endl;
		file << "<WARNING>: " << msg << std::endl;
		break;
	case msgType::LOG_ERROR:
		std::cout << "<ERROR>: " << msg << std::endl;
		file << "<ERROR>: " << msg << std::endl;
		break;
	case msgType::OK:
		std::cout << "<OK>: " << msg << std::endl;
		file << "<OK>: " << msg << std::endl;
		break;
	}
}