// this is an example app with minimal callbacks

#include "pntr_fenster.h"

// this is used to track offset for byte-beat
static uint32_t u = 0;

struct fenster app_init(pntr_image* screen) {
  struct fenster app = {
    .title = "pntr_fenster example",
    .width = screen->width,
    .height = screen->height,
    .buf = &screen->data->value
  };
  return app;
}

void app_update(pntr_image* screen, int64_t now) {
  (void) now;

  pntr_draw_circle_fill(screen, 200, 200, 160, PNTR_RED);
}

void app_get_audio(float* audio, int n) {
   u++;

  for (int i = 0; i < n; i++) {
     u++;

    // silent
    // audio[i] = 0;

    // static
    // audio[i] = (rand() & 0xff)/256.f;

    // byte-beat
    int x = u * 80 / 441;
    audio[i] = ((((x >> 10) & 42) * x) & 0xff) / 256.f;
  }
}

void app_cleanup() {}
