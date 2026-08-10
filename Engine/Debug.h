#pragma once
#include "Prerequisites.h"
#include <string>
#include <vector>

class Debug {
public:
	static void Log(const std::string& msg);
	static void Warning(const std::string& msg);
	static void Error(const std::string& msg);
private:
	static const std::vector<LogInfo>& getLogs();
	static void clearLogs();

	static void addEntry(LogLevel lvl, const std::string &msg);

private:
	static std::vector<LogInfo> m_logs;
private:
	friend class DebugUI;
};

