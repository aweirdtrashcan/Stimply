#pragma once

#define SFATAL(msg, ...) { Logger::SLOG(msg, ##__VA_ARGS__); debugBreak(); }
#define SINFO(msg, ...) { Logger::SLOG(msg, ##__VA_ARGS__); }

class Logger
{
public:
    Logger() = delete;
    Logger(Logger&) = delete;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
public:
    static void SLOG(const char* message, ...);
};