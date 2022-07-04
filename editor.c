#include "editor.h"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h> /* winsize */
#include <unistd.h>

#include "dbg.h"

void initEditor(editorConfig *E) {
  check(getWindowSize(E) == -1, "getWindowSize");
}

int getWindowSize(editorConfig *E) {
  struct winsize ws;

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    E->screencols = ws.ws_col;
    E->screenrows = ws.ws_row;
    return 0;
  }
}

char editorReadKey() {
  char c;
  int rc;
  while ((rc = read(STDIN_FILENO, &c, 1)) != 1) {
    check(rc == -1 && errno != EAGAIN, "read from input fail");
  }
  return c;
}

void editorProcessKeypress() {
  char c = editorReadKey();

  switch (c) {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}

void editorDrawRaws(editorConfig *E) {
  int y;
  for (y = 0; y < E->screencols; y++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}

void editorRefreshScreen(editorConfig *E) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  editorDrawRaws(E);

  write(STDOUT_FILENO, "\x1b[H", 3);
}