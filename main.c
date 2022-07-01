#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dbg.h"
#include "editor.h"
#include "terminal.h"

int main(int argc, char **argv) {
  if (argc >= 2) {
    log_info("Usage: kilo <filename>(optional)");
    exit(1);
  }

  editorConfig E;
  initEditor(&E);
  enableRawMode(&E.orig_termios);

  while (1) {
    char c = '\0';
    if ((read(STDIN_FILENO, &c, 1) == -1) && (errno != EAGAIN)) {
      log_err("read from input fail");
      exit(1);
    }
    printf("%c\n", c);
    if (c == 'q') break;
  }

  disableRawMode(&E.orig_termios);
  return 0;
}
