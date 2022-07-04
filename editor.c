#include "editor.h"

#include <ctype.h>
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

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
  return 0;
}

int getWindowSize(editorConfig *E) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(&E->screenrows, &E->screencols);
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

void editorDrawRaws(editorConfig *E, abuf *ab) {
  int y;
  /* TODO: put more information into welcoming message, e.g. help, how to
   * quit..., see what vim & nvim does!! especially when window size change or
   * too small*/
  for (y = 0; y < E->screenrows; y++) {
    if (y == E->screenrows / 3) {
      char welcome[80];
      int welcomeLen =
          snprintf(welcome, sizeof(welcome), "Min Editor v%s", MIN_VERSION);
      if (welcomeLen > E->screencols) welcomeLen = E->screencols;
      int padding = (E->screencols - welcomeLen) / 2;
      if (padding) {
        abAppend(ab, "~", 1);
        padding--;
      }
      while (padding--) {
        abAppend(ab, " ", 1);
      }
      abAppend(ab, welcome, welcomeLen);
    } else {
      abAppend(ab, "~", 3);
    }

    abAppend(ab, "\x1b[K", 3); /* Erases from the current cursor position to
                              the end of the current line */
    if (y < E->screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
}

void editorRefreshScreen(editorConfig *E) {
  // use buffer to avoid continuous write(flickering effect to the editor view)
  abuf ab = ABUF_INIT;
  abAppend(&ab, "\x1b[?25l", 6); /* make cursor invisible */
  abAppend(&ab, "\x1b[H", 3);    /* move cursor to home */

  editorDrawRaws(E, &ab);

  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25h", 6); /* make cursor visible */

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}