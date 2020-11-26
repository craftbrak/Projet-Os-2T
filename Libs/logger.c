#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Settings.h"
#include "voiture.h"
#include "sharedmem.h"
#include "logger.h"

char* _getTime() {
    time_t rawTime;
    struct tm* timeInfo;
    char* result;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    result = asctime(timeInfo);
    result[strlen(result)-1] = 0;

    return result;
}

int _message(SharedInfo shared, char* header, char* msg) {
    FILE* logFile = fopen("fcc.log", "a");
    if (!logFile) {
        perror("Log file");
        return 0;
    }

    if (!getSemaphore(0, shared)) {
        fclose(logFile);
        return 0;
    }

    fprintf(logFile, "[%s] %s - %s\n", _getTime(), header, msg);

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