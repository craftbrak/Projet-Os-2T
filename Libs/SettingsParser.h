#pragma once
#include "Settings.h"

void seterror(char* fmt, ...);
char* geterror();

Settings parse_config(char* filename);