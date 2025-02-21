#pragma once

#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

void reset_terminal_mode(struct termios* orig_termios) {
  tcsetattr(0, TCSANOW, orig_termios);
}

void set_conio_terminal_mode(struct termios* orig_termios) {
  // Note: Disables carriage return after line feed and enables stdout line
  // buffering.
  struct termios new_termios;
  tcgetattr(0, orig_termios);
  memcpy(&new_termios, orig_termios, sizeof(new_termios));
  cfmakeraw(&new_termios);
  tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit() {
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
  int r;
  unsigned char c;
  if ((r = read(0, &c, sizeof(c))) < 0) {
    return r;
  } else {
    return c;
  }
}
