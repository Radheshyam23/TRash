#ifndef __RawMode__
#define __RawMode__

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


void die(const char *s);
void disableRawMode();
void enableRawMode();

#endif