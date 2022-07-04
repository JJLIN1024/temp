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
    editorRefreshScreen(&E);
  }
}

int main(int argc, char **argv) {
  if (argc >= 2) {
    log_info("Usage: kilo <filename>(optional)");
  }

  initEditor(&E);
  enableRawMode(&E.orig_termios);

  /* listen for Window Size change */
  signal(SIGWINCH, sigwinchHandler);

  while (1) {
    editorRefreshScreen(&E);
    editorProcessKeypress(&E);
  }

  disableRawMode(&E.orig_termios);
  return 0;
}
