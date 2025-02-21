// THIS FILE WAS CREATED AUTOMATICALLY BY /home/user/4/Game/utils/compose

#pragma once

// HEADERS FROM ansi.h

#include <stdio.h>

// HEADERS FROM conio.h

#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

// USER TYPES HEADER

#include "types.h"

// SIGNATURES FROM ansi.h

// Cursor position, 1-based
void cup(int row, int col);

// Erase in line
void el2();

// Cursor visibility
void dectcem(int show);

// Alternative screen buffer
void csi1049(int enable);

// Select Graphic Rendition
void sgr(int param);

void sgr24(uint32_t rgb, int bg);

void sgr8(uint8_t a, int bg);

// SIGNATURES FROM common.h

int sgrparams(const char* s);

int sgrcolors(const char* s, int is_bg, int is_bright);

void printbytes(const unsigned char* a, size_t n);

const char* bppname(unsigned int bpp);

void* push(void* a, size_t* n, const void* valuep, size_t size);

const void* find(const void* a, size_t n, const void* vp, size_t vsz);

imap index_gimp_image(const void* gimp_image_p);

const char* find_with_ctype(const char* s, int (*f)(int), int negate);

// Searches for isalnum in *sp, sets *sp to first NOT isalnum or NULL. Returns
// index of param found (only first character is tested), or any digit found.
// Neither returns -1.
int param_alphanum(const char* sp[static 1],
                   const char** params,
                   size_t paramlen);

int strarrchr(char c, const char** list, size_t n);

uint64_t nanoseconds();

uint64_t remain_u64(uint64_t* a, uint64_t mod);

int32_t remain_i32(int32_t* a, int32_t mod);

void nanodelay(uint64_t ns);

void setsgr24(uint32_t fg, uint32_t bg);

int print_clock(bool with_seconds);

const char* redirect_stdout();

void restore_stdout();

void winsize(int* row, int* col);

void draw_mu8(const mat* mu8, int y, int x);

bool is_inside(const mat* a, size_t y, size_t x);

uint8_t get_u8(const mat* mu8, size_t y, size_t x, uint8_t fallback);

void draw_mu8_with_alpha(const mat* fore,
                         const mat* back,
                         int row,
                         int col,
                         uint8_t alpha);

void put_u8(mat* mu8, size_t y, size_t x, uint8_t v);

void map_dda(mat* mu8,
             double y1,
             double x1,
             double y2,
             double x2,
             uint8_t color);

void map_circle(mat* mu8, double y, double x, double r, uint8_t color);

void init_mu8(mat* out, size_t h, size_t w);

bool should_fire(process* pp, uint64_t* nsp);

void kill_mat(mat* a);

void map_clock_arrows(mat* mu8, double r);

void print_clock_numbers(double r, int row, int col);

// SIGNATURES FROM conio.h

void reset_terminal_mode(struct termios* orig_termios);

void set_conio_terminal_mode(struct termios* orig_termios);

int kbhit();

int getch();

// SIGNATURES FROM processes.h

void call_tray(void* process_p, bool force);

void call_clock(void* process_p, bool force);

// USER HEADERS

#include "ansi.h"
#include "common.h"
#include "conio.h"
#include "processes.h"