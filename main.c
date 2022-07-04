#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dbg.h"
#include "editor.h"
#include "terminal.h"

editorConfig E;
static void sigwinchHandler(int sig) {
  if (SIGWINCH == sig) {
    getWindowSize(&E);
    editorRefreshScreen(&E);
  }
}

int main(int argc, char **argv) {
  if (argc >= 2) {
    log_info("Usage: kilo <filename>(optional)");
  }

  enableRawMode(&E.orig_termios);
  initEditor(&E);

  /* listen for Window Size change */
  signal(SIGWINCH, sigwinchHandler);

  while (1) {
    editorRefreshScreen(&E);
    editorProcessKeypress(&E);
  }

  disableRawMode(&E.orig_termios);
  return 0;
}
