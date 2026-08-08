#include "Debug.h"

std::vector<LogInfo> Debug::m_logs;

void Debug::Log(const std::string& msg) {
	std::string log = "[LOG] " + msg;
	addEntry(LogLevel::Info, log);
}

void Debug::Warning(const std::string& msg) {
	std::string warning = "[WARNING] " + msg;
	addEntry(LogLevel::Warning, warning);
}

void Debug::Error(const std::string& msg) {
	std::string error = "[ERROR] " + msg;
	addEntry(LogLevel::Error, error);
}

const std::vector<LogInfo>& Debug::getLogs() {
	return m_logs;
}

void Debug::clearLogs() {
	m_logs.clear();
}

void Debug::addEntry(LogLevel lvl, const std::string& msg) {
	m_logs.push_back({lvl, msg});
}
