
#include "terminal.h"

#include <stdlib.h>
#include <unistd.h>

#include "dbg.h"
#include "editor.h"

void enableRawMode(struct termios *orig_termios) {
  struct termios raw = *orig_termios;
  if (tcgetattr(STDIN_FILENO, &raw) == -1) log_err("tcgetattr");

  raw.c_lflag &= ~(ECHO);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) log_err("tcsetattr");
}

void disableRawMode(struct termios *orig_termios) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios) == -1)
    log_err("tcsetattr");
}