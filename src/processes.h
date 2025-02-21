#pragma once

void call_tray(void* process_p, bool force) {
  process* pp = process_p;
  uint64_t ns = 0;
  if (!force && !should_fire(pp, &ns))
    return;

  struct {
    int twh;
    int tww;
  }* o = pp->state;

  const bool once = force;
  if (once) {
    sgr8(CC(255), 0);
    sgr8(CC(26), 1);
    cup(o->twh, 1);
    for (int col = 1; col <= o->tww; col++)
      printf(" ");
    cup(o->twh, 1);
    printf("Enter q to quit");
  }

  sgr8(CC(255), 0);
  sgr8(CC(26), 1);
  cup(o->twh, o->tww - 8);
  print_clock(1);
}

void call_clock(void* process_p, bool force) {
  process* pp = process_p;
  uint64_t ns = 0;
  if (!force && !should_fire(pp, &ns))
    return;

  struct {
    int y;
    int x;
    double r;
    const mat* circle;
    mat* arrows;
  }* o = pp->state;

  draw_mu8(o->circle, o->y, o->x);
  memset(o->arrows->o, 0, o->arrows->w * o->arrows->h * sizeof(uint8_t));
  map_clock_arrows(o->arrows, o->r);
  enum { TransparentColor = 0 };
  draw_mu8_with_alpha(o->arrows, o->circle, o->y, o->x, TransparentColor);
  print_clock_numbers(o->r, o->y, o->x);
}
