#pragma once

typedef struct {
  size_t cn;
  const void** cs;
  size_t in;
  size_t* is;
} imap;

typedef struct {
  size_t n;
  void* o;
} vec;

typedef struct {
  size_t h;
  size_t w;
  void* o;
} mat;

typedef struct {
  uint64_t prev_ns;
  uint64_t ns_per_call;
  void (*call)(void* processp, bool force);
  void* state;
} process;
