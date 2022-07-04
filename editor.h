#ifndef __editor_h__
#define __editor_h__

#include <termios.h>
#include <time.h>

#include "buffer.h"

#define CTRL_KEY(k) ((k)&0x1f)
#define MIN_VERSION "0.0.1"
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

void initEditor(editorConfig *);
int getCursorPosition(int *, int *);
int getWindowSize(editorConfig *);
char editorReadKey();
void editorProcessKeypress();
void editorDrawRaws(editorConfig *, abuf *);
void editorRefreshScreen(editorConfig *);
#endif