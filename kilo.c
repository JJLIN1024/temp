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

  char c;
  while (read(STDIN_FILENO, &c, 1) != -1 && c != 'q') {
    printf("%c\n", c);
  }

  disableRawMode(&E.orig_termios);
  return 0;
}
