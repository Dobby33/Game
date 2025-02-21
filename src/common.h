#pragma once

int sgrparams(const char* s) {
  const struct {
    int param;
    const char* name;
  } list[] = {
      {0, "normal"},    {1, "saturated"}, {2, "faint"},   {3, "italic"},
      {4, "underline"}, {5, "blink"},     {7, "reverse"}, {9, "crossed"},
      {21, "doubly"},   {53, "overline"},
  };
  const size_t n = sizeof(list) / sizeof(list[0]);
  for (size_t i = 0; i < n; ++i)
    if (list[i].name[0] == s[0])
      return list[i].param;
  return -1;
}

int sgrcolors(const char* s, int is_bg, int is_bright) {
  const char* list[] = {
      "black", "red", "green", "yellow", "navy", "magenta", "cyan", "white",
  };
  enum {
    fg = 30,
    bg = 40,
    brightfg = 90,
    brightbg = 102,
  };
  const int m = ((const int[]){fg, bg, brightfg,
                               brightbg})[(!!is_bg) | ((!!is_bright) << 1)];
  const size_t n = sizeof(list) / sizeof(list[0]);
  for (size_t i = 0; i < n; ++i)
    if (list[i][0] == s[0])
      return i + m;
  return -1;
}

void printbytes(const unsigned char* a, size_t n) {
  for (size_t i = 0; i < n; ++i)
    printf("%i,", a[i]);
}

const char* bppname(unsigned int bpp) {
  switch (bpp) {
    case 1:
      return "index256";
    case 2:
      return "RGB16";
    case 3:
      return "RGB";
    case 4:
      return "RGBA";
  }
  return 0;
}

void* push(void* a, size_t* n, const void* valuep, size_t size) {
  const size_t i = *n;
  a = realloc(a, size * (i + 1));
  memmove((uint8_t*)a + i * size, valuep, size);
  (*n)++;
  return a;
}

const void* find(const void* a, size_t n, const void* vp, size_t vsz) {
  for (size_t i = 0; i < n; ++i) {
    if (!memcmp(a, vp, vsz))
      return a;
    a = (uint8_t*)a + vsz;
  }
  return 0;
}

imap index_gimp_image(const void* gimp_image_p) {
  const struct {
    unsigned int w;
    unsigned int h;
    unsigned int bpp;
    const unsigned char* bytes;
  }* a = gimp_image_p;
  size_t cn = 0;
  const void** cs = 0;
  size_t in = 0;
  size_t* is = 0;
  for (size_t y = 0; y < a->h; ++y) {
    for (size_t x = 0; x < a->w; ++x) {
      const unsigned char* cp = &a->bytes[in * a->bpp];
      size_t iv = cn;
      for (size_t i = 0; i < cn; ++i) {
        if (memcmp(cs[i], cp, a->bpp) == 0) {
          iv = i;
          break;
        }
      }
      if (iv == cn)
        cs = push(cs, &cn, &cp, sizeof(cp));
      is = push(is, &in, &iv, sizeof(iv));
    }
  }
  return (imap){cn, cs, in, is};
}

const char* find_with_ctype(const char* s, int (*f)(int), int negate) {
  for (;;) {
    if ((!!f(*s)) == (!!negate))
      return s;
    if (*s == 0)
      return 0;
    s++;
  }
  return 0;
}

// Searches for isalnum in *sp, sets *sp to first NOT isalnum or NULL. Returns
// index of param found (only first character is tested), or any digit found.
// Neither returns -1.
int param_alphanum(const char* sp[static 1],
                   const char** params,
                   size_t paramlen) {
  int v = -1;
  if ((*sp = find_with_ctype(*sp, isalnum, 0))) {
    if (isdigit(**sp))
      v = atoi(*sp);
    else if (isalpha(**sp))
      for (size_t pi = 0; pi < paramlen; ++pi)
        if (params[pi][0] == **sp) {
          v = pi;
          break;
        }
    *sp = find_with_ctype(*sp, isalnum, 1);
  }
  return v;
}

int strarrchr(char c, const char** list, size_t n) {
  for (size_t i = 0; i < n; ++i)
    if (list[i][0] == c)
      return i;
  return -1;
}

uint64_t nanoseconds() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  return (uint64_t)1.e9 * spec.tv_sec + spec.tv_nsec;
}

uint64_t remain_u64(uint64_t* a, uint64_t mod) {
  uint64_t b = *a;
  *a = b % mod;
  return b / mod;
}

int32_t remain_i32(int32_t* a, int32_t mod) {
  int32_t b = *a;
  *a = b % mod;
  return b / mod;
}

void nanodelay(uint64_t ns) {
  const uint64_t s = remain_u64(&ns, 1.e9);
  nanosleep(&(const struct timespec){.tv_sec = s, .tv_nsec = ns}, 0);
}

void setsgr24(uint32_t fg, uint32_t bg) {
  enum { Omit = 0 };
  if (fg != Omit)
    sgr24(fg, 0);
  if (bg != Omit)
    sgr24(bg, 1);
}

int print_clock(bool with_seconds) {
  time_t t = time(0);
  struct tm* tm = localtime(&t);
  if (with_seconds) {
    printf("%2i:%02i:%02i", tm->tm_hour, tm->tm_min, tm->tm_sec);
    return 2 + 1 + 2 + 1 + 2;
  } else {
    printf("%2i:%02i", tm->tm_hour, tm->tm_min);
    return 2 + 1 + 2;
  }
}

const char* redirect_stdout() {
  static char s[BUFSIZ];
  freopen("/dev/null", "a", stdout);
  setbuf(stdout, s);
  return s;
}

void restore_stdout() {
  freopen("/dev/tty", "a", stdout);  // "CON" on Windows
}

void winsize(int* row, int* col) {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  *row = w.ws_row;
  *col = w.ws_col;
}

#define UPPER_HALF_BLOCK "\xe2\x96\x80"

void draw_mu8(const mat* mu8, int y, int x) {
  const size_t h = mu8->h;
  const size_t w = mu8->w;
  const uint8_t* o = mu8->o;
  enum { BlocksNumInChar = 2 };
  for (size_t i = 0; i < h; i += BlocksNumInChar) {
    for (size_t k = 0; k < w; k++) {
      int cy = y + i / BlocksNumInChar;
      int cx = x + k;
      cup(cy, cx);
      uint8_t u = i + 0 < h ? o[(i + 0) * w + k] : 0;
      uint8_t d = i + 1 < h ? o[(i + 1) * w + k] : 0;
      sgr8(u, 0);
      sgr8(d, 1);
      printf(UPPER_HALF_BLOCK);
    }
  }
}

bool is_inside(const mat* a, size_t y, size_t x) {
  return (y < a->h && x < a->w);
}

uint8_t get_u8(const mat* mu8, size_t y, size_t x, uint8_t fallback) {
  if (is_inside(mu8, y, x))
    return ((const uint8_t*)mu8->o)[(y)*mu8->w + (x)];
  return fallback;
}

void draw_mu8_with_alpha(const mat* fore,
                         const mat* back,
                         int row,
                         int col,
                         uint8_t alpha) {
  const size_t h = fore->h;
  const size_t w = fore->w;
  const uint8_t* o = fore->o;
  enum { BlocksNumInChar = 2 };
  for (size_t i = 0; i < h; i += BlocksNumInChar) {
    for (size_t k = 0; k < w; k++) {
      const int row1 = row + i / BlocksNumInChar;
      const int col1 = col + k;
      cup(row1, col1);
      const uint8_t u = i + 0 < h ? o[(i + 0) * w + k] : alpha;
      const uint8_t d = i + 1 < h ? o[(i + 1) * w + k] : alpha;
      if (u != alpha)
        sgr8(u, 0);
      else
        sgr8(get_u8(back, i + 0, k, alpha), 0);
      if (d != alpha)
        sgr8(d, 1);
      else
        sgr8(get_u8(back, i + 1, k, alpha), 1);
      printf(UPPER_HALF_BLOCK);
    }
  }
}

void put_u8(mat* mu8, size_t y, size_t x, uint8_t v) {
  if (is_inside(mu8, y, x)) {
    ((uint8_t*)mu8->o)[y * mu8->w + x] = v;
  }
}

void map_dda(mat* mu8,
             double y1,
             double x1,
             double y2,
             double x2,
             uint8_t color) {
  double dy = (y2 - y1);
  double dx = (x2 - x1);
  double step;
  if (fabs(dx) >= fabs(dy))
    step = fabs(dx);
  else
    step = fabs(dy);
  dy = dy / step;
  dx = dx / step;
  double y = y1;
  double x = x1;
  int i = 0;
  while (i <= step) {
    put_u8(mu8, round(y), round(x), color);
    y = y + dy;
    x = x + dx;
    i++;
  }
}

#define SZ(a) (sizeof(a) / sizeof(a[0]))

void map_circle(mat* mu8, double y, double x, double r, uint8_t color) {
  size_t h = ceil(r * 2);
  size_t w = ceil(r * 2);
  for (size_t i = 0; i < h; i++) {
    for (size_t k = 0; k < w; k++) {
      if (pow(i - r, 2) + pow(k - r, 2) <= pow(r, 2)) {
        size_t i1 = round(y + i);
        size_t k1 = round(x + k);
        if (is_inside(mu8, i1, k1))
          ((uint8_t*)mu8->o)[i1 * w + k1] = color;
      }
    }
  }
}

void init_mu8(mat* out, size_t h, size_t w) {
  size_t n = h * w * sizeof(uint8_t);
  uint8_t* o = realloc(0, n);
  memset(o, 0, n);
  out->h = h;
  out->w = w;
  out->o = o;
}

bool should_fire(process* pp, uint64_t* nsp) {
  const uint64_t nowns = nanoseconds();
  const uint64_t oldns = pp->prev_ns;
  const uint64_t ns = nowns - oldns;
  if (ns >= pp->ns_per_call) {
    pp->prev_ns = nowns;
    *nsp = ns;
    return true;
  }
  return false;
}

void kill_mat(mat* a) {
  free(a->o);
  a->o = 0;
}

#define CC(color_u8) (color_u8)

#define PI (3.14159265395)

void map_clock_arrows(mat* mu8, double r) {
  struct {
    int dt;
    double t;
    double dr;
    uint8_t c;
  } arrows[] = {
      {60, (nanoseconds() / 1.e9), 1.5, CC(4)},
      {60, (nanoseconds() / 1.e9 / 60), 1.75, CC(5)},
      {12, (nanoseconds() / 1.e9 / 60 / 60), 2.00, CC(6)},
  };
  for (size_t k = 0; k < SZ(arrows); ++k) {
    int dt = arrows[k].dt;
    double t = fmod(arrows[k].t, dt);
    double a = PI * 2 / dt * t - PI / 2;
    double lr = r / arrows[k].dr;
    double ly = r + sin(a) * lr;
    double lx = r + cos(a) * lr;
    map_dda(mu8, r, r, round(ly), round(lx), arrows[k].c);
  }
}

void print_clock_numbers(double r, int row, int col) {
  const char* roman12[] = {"XII", "I",   "II",   "III", "IV", "V",
                           "VI",  "VII", "VIII", "IX",  "X",  "XI"};
  enum { BlocksNumInChar = 2 };
  for (int t = 0; t < 12; t++) {
    double a = PI * 2 / 12 * t - PI / 2;
    double rr = r / 1.25;
    double rh = rr / BlocksNumInChar;
    double rw = rr;
    double rcy = r / BlocksNumInChar;
    double rcx = r;
    const char* s = roman12[t];
    double scy = 1 / 2.0;
    double scx = strlen(s) / 2.0;
    int sy = round(row + rcy + sin(a) * rh - scy);
    int sx = round(col + rcx + cos(a) * rw - scx);
    cup(sy, sx);
    sgr8(CC(8), 0);
    sgr8(CC(7), 1);
    printf("%s", s);
  }
}
