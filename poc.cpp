#pragma leco tool
import siaudio;

#ifdef WASM
#define EXPORT __attribute__((export_name("blip")))
#else
#define EXPORT
#endif

extern "C" unsigned EXPORT blip() {
  static volatile unsigned gens = 0;
  static auto s = siaudio::streamer{[&](float *data, unsigned samples) {
    float mult = (gens < 1000) ? gens / 1000.0f : 1.0f;
    for (unsigned i = 0; i < samples; ++i) {
      data[i] = mult * ((gens / 20) % 2) - 0.5f;
      gens = gens + 1;
    }
  }};
  return gens;
}

int main() {
  while (blip() < 44100) {
  }
}
