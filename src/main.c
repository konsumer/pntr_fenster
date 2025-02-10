// this is an example app with minimal callbacks

#include "pntr_fenster.h"

// this is used to track offset for byte-beat
static int32_t u = 0;

cm_Source *src;

struct fenster app_init(pntr_image* screen) {
  struct fenster app = {
    .title = "pntr_fenster example",
    .width = screen->width,
    .height = screen->height,
    .buf = &screen->data->value
  };

  src = cm_new_source_from_file("loop.wav");
  cm_set_loop(src, 1);
  cm_play(src);

  return app;
}

void app_update(pntr_image* screen, int64_t now) {
  pntr_clear_background(screen, PNTR_BLACK);
  pntr_draw_circle_fill(screen, 200, 200, (now/10 % 120), PNTR_RED);
}

// read more here: https://stellartux.github.io/websynth/guide.html
static int soundgen(int t) {
  return (t>>10^t>>11)%5*((t>>14&3^t>>15&1)+1)*t%99+((3+(t>>14&3)-(t>>16&1))/3*t%99&64);
}

void app_get_audio(float* audio, int n) {
  for (int i = 0; i < n; i++) {
     u++;

    // audio is current mixer-output, do whatever you want with it

    // silent
    // audio[i] = 0;

    // static
    // audio[i] = (rand() & 0xff)/256.f;

    // byte-beat mixed with incoming
    audio[i] = (audio[i]/2.0f) + (soundgen(u/8) / 2048.0f);
  }
}

void app_cleanup() {
   cm_destroy_source(src);
}
