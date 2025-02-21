#pragma once

#include <stdio.h>

// Cursor position, 1-based
void cup(int row, int col) {
  printf("\033[%i;%iH", row, col);
}

// Erase in line
void el2() {
  enum {
    FromCursorToTheEnd = 0,
    FromCursorToBegining = 1,
    EntireLine = 2,
  };
  printf("\033[%iK", EntireLine);
}

// Cursor visibility
void dectcem(int show) {
  printf("\033[?25%c", show ? 'h' : 'l');
}

// Alternative screen buffer
void csi1049(int enable) {
  printf("\033[?1049%c", enable ? 'h' : 'l');
}

// Select Graphic Rendition
void sgr(int param) {
  printf("\033[%im", param);
}

void sgr24(uint32_t rgb, int bg) {
  int r = (rgb >> 0x10) & 0xff;
  int g = (rgb >> 0x08) & 0xff;
  int b = (rgb >> 0x00) & 0xff;
  printf("\033[%i;2;%i;%i;%im", bg ? 48 : 38, r, g, b);
}

void sgr8(uint8_t a, int bg) {
  printf("\033[%i;5;%im", bg ? 48 : 38, a);
}
