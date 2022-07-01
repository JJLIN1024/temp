#include "termios.h"
#include "time.h"

typedef struct editorConfig {
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int screenrows;
  int screencols;
  int numrows;
  char *filename;
  struct termios orig_termios;
} editorConfig;

void initEditor(editorConfig *E);
