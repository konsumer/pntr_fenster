// this is an example shell using pntr+fenster

#define PNTR_PIXELFORMAT_RGBA
#define PNTR_IMPLEMENTATION
#include "pntr.h"
#include "fenster.h"
#include "fenster_audio.h"
#include "cmixer.h"

// implement these
struct fenster app_init(pntr_image* screen);
void app_update(pntr_image* screen, int64_t now);
void app_get_audio(float* audio, int n);
void app_cleanup();

// wrappers more like pntr_app
cm_Source* pntr_load_sound(const char* fileName) {
  unsigned int bytesRead = 0;
  unsigned char* bytes = pntr_load_file(fileName, &bytesRead);
  return cm_new_source_from_mem(bytes, bytesRead);
}


int run() {
  pntr_image* screen = pntr_new_image(400, 400);
  cm_init(44100);

  struct fenster app = app_init(screen);
  struct fenster_audio fa = {0};
  float audio[FENSTER_AUDIO_BUFSZ] = {0};
  int16_t ua[FENSTER_AUDIO_BUFSZ*2] = {0};
  
  fenster_open(&app);
  fenster_audio_open(&fa);

  int64_t now = fenster_time();
  int64_t time;

  while (fenster_loop(&app) == 0) {
    app_update(screen, now);
    int n = fenster_audio_available(&fa);
    if (n > 0) {
      cm_process((void*) ua, n*2);
      
for (int i = 0; i < n; i++) {
    // Convert from [-32768, 32767] to [-1.0, 1.0]
    float left = ua[i * 2] / 32768.0f;
    float right = ua[i * 2 + 1] / 32768.0f;
    
    // Average the channels
    audio[i] = (left + right) * 0.5f;
}
      
      app_get_audio(audio, n);
      fenster_audio_write(&fa, audio, n);
    }
    
    // maintain 60 FPS
    time = fenster_time();
    if (time - now < 1000 / 60) {
      fenster_sleep(time - now);
    }
    now = time;
  }

  app_cleanup();
  pntr_unload_image(screen);
  fenster_audio_close(&fa);
  fenster_close(&app);

  return 0;
}

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
  (void)hInstance, (void)hPrevInstance, (void)pCmdLine, (void)nCmdShow;
  return run();
}
#else
int main() { return run(); }
#endif