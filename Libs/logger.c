#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Settings.h"
#include "voiture.h"
#include "sharedmem.h"
#include "logger.h"

struct tm* _getTime() {
    time_t rawTime;

    time(&rawTime);
    return localtime(&rawTime);
}

int _message(SharedInfo shared, char* header, char* msg) {
    FILE* logFile = fopen("fcc.log", "a");
    char buf[32];

    if (!logFile) {
        perror("Log file");
        return 0;
    }

    if (!getSemaphore(0, shared)) {
        fclose(logFile);
        return 0;
    }

    strftime(buf, 32, "%Y-%m-%d %H:%M:%S", _getTime());

    fprintf(logFile, "[%s] %s - %s\n", buf, header, msg);

    if (!freeSemaphore(0, shared)) {
        fclose(logFile);
        return 0;
    }

    fclose(logFile);
    return 1;
}

int logWarning(SharedInfo shared, char* message) {
    return _message(shared, "WARNING", message);
}

int logError(SharedInfo shared, char* message) {
    perror(message);
    return _message(shared, "ERROR", message);
}

int logInfo(SharedInfo shared, char* message) {
    return _message(shared, "INFO", message);
}