#include <iostream>
#include <windows.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include <format> // Requires C++20

class Logger {
public:
    enum LogLevel {
        INFO,
        WARNING,
        ERRORR,
        SUCCESS
    };

    // Variadic log function to accept format strings
    template<typename... Args>
    static void Log(const std::string& formatStr, LogLevel level, Args... args) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Get current time
        std::string timeStr = getCurrentTime();

        const char* levelStr = logLevelToString(level);
        WORD color = getLogLevelColor(level);

        // Format the message
        std::string message = std::vformat(formatStr, std::make_format_args(args...));

        // Print the log message with colors
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
        std::cout << "[";

        SetConsoleTextAttribute(hConsole, color);
        std::cout << timeStr;

        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
        std::cout << "] [";

        SetConsoleTextAttribute(hConsole, color);
        std::cout << levelStr;

        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
        std::cout << "] " << message << std::endl;

        // Reset console color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

private:
    static std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now);

        char timeBuffer[20];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &localTime);
        return std::string(timeBuffer);
    }

    static const char* logLevelToString(LogLevel level) {
        static const std::unordered_map<LogLevel, const char*> logLevelMap = {
            { INFO, "INFO" },
            { WARNING, "WARNING" },
            { ERRORR, "ERROR" },
            { SUCCESS, "SUCCESS" }
        };

        auto it = logLevelMap.find(level);
        return (it != logLevelMap.end()) ? it->second : "UNKNOWN";
    }

    static WORD getLogLevelColor(LogLevel level) {
        switch (level) {
        case INFO: return FOREGROUND_BLUE;
        case WARNING: return FOREGROUND_RED | FOREGROUND_GREEN;
        case ERRORR: return FOREGROUND_RED;
        case SUCCESS: return FOREGROUND_GREEN;
        default: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }
};