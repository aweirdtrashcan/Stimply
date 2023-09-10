#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <alloca.h>

void Logger::SLOG(const char* message, ...)
{
    int messageLength = strlen(message);
    static constexpr unsigned char MAX_VARARG_SIZE = 100;
    // plus 1 for the null terminator.
    char* outputMessage = static_cast<char*>(alloca(messageLength + MAX_VARARG_SIZE + 1));
    memset(outputMessage, 0, messageLength + MAX_VARARG_SIZE + 1);
    va_list arg_ptr = {};
    va_start(arg_ptr, message);
    vsnprintf(outputMessage, messageLength + MAX_VARARG_SIZE, message, arg_ptr);
    va_end(arg_ptr);
    outputMessage[messageLength + MAX_VARARG_SIZE] = 0;
    printf("%s\n", outputMessage);
}