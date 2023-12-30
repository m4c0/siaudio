#pragma leco tool
import siaudio;

#ifdef WASM
#define EXPORT __attribute__((export_name("blip")))
#else
#define EXPORT
#endif

class audio : public siaudio::os_streamer {
  volatile unsigned m_gens{};

public:
  void fill_buffer(float *data, unsigned samples) override {
    float mult = (m_gens < 1000) ? m_gens / 1000.0f : 1.0f;
    for (unsigned i = 0; i < samples; ++i) {
      data[i] = mult * ((m_gens / 20) % 2) - 0.5f;
      m_gens = m_gens + 1;
    }
  }
  unsigned blip() const noexcept { return m_gens; }
};

extern "C" unsigned EXPORT blip() {
  static audio s{};
  return s.blip();
}

int main() {
  while (blip() < 44100) {
  }
}
