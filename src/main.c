#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

#include "head.h"

struct termios Termios;

void sigabrt(int sig) {
  (void)sig;
  sgr(0);
  csi1049(0);
  reset_terminal_mode(&Termios);
}

void __asan_on_error() {
  sigabrt(0);
}

bool is_winsize_change(bool set_to_changed) {
  static bool changed = false;
  if (set_to_changed) {
    changed = true;
  } else if (changed) {
    changed = false;  // Toggle at read
    return true;
  }
  return changed;
}

void sigwinch(int sig) {
  (void)sig;
  is_winsize_change(true);
}

int main() {
  set_conio_terminal_mode(&Termios);
  setbuf(stdout, NULL);
  signal(SIGINT, SIG_IGN);
  signal(SIGABRT, sigabrt);
  signal(SIGWINCH, sigwinch);
  csi1049(1);

  int twh, tww;
  winsize(&twh, &tww);

  process tray = {.prev_ns = nanoseconds(),
                  .ns_per_call = 1.e9 / 1,
                  .call = call_tray,
                  &((struct {
                    int terminal_h;
                    int terminal_w;
                  }){
                      twh,
                      tww,
                  })};

  double r = 12.5;
  int y = 5;
  int x = 10;
  mat clock_circle;
  init_mu8(&clock_circle, ceil(r * 2), ceil(r * 2));
  map_circle(&clock_circle, 0, 0, r, CC(7));
  mat clock_arrows;
  init_mu8(&clock_arrows, clock_circle.h, clock_circle.w);
  process clock = {
      .prev_ns = nanoseconds(),
      .ns_per_call = 1.e9 / 1,
      .call = call_clock,
      &((struct {
        int row;
        int col;
        double radius;
        const mat* circle;
        mat* arrows;
      }){
          .row = y,
          .col = x,
          .radius = r,
          .circle = &clock_circle,
          .arrows = &clock_arrows,
      }),
  };

  process* processes[] = {
      &tray,
      &clock,
  };

  bool initial = true;
  uint64_t kps = 100;  // Keyboard listenings per second
  while (1) {
    dectcem(0);
    while (!kbhit()) {
      if (is_winsize_change(0)) {
        // TODO
      }
      process** o = processes;
      for (size_t t = 0; t < SZ(processes); t++)
        o[t]->call(o[t], initial);
      if (kps)
        nanodelay(1.e9 / kps);
      initial = false;
    }
    dectcem(1);
    int c = getch();
    if (c == 'q')
      break;
  }

  kill_mat(&clock_circle);
  kill_mat(&clock_arrows);

  sigabrt(0);
  return 0;
}
