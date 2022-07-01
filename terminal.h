#include <termios.h>

#include "dbg.h"

struct termios orig_termios;

void enableRawMode(struct termios *);

void disableRawMode(struct termios *);