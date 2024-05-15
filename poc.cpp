#pragma leco tool
import siaudio;
import sitime;

static constexpr const auto rate = 44100;

class audio : public siaudio::os_streamer {
  unsigned m_gens{};

public:
  audio() : os_streamer{rate} {}

  void fill_buffer(float *data, unsigned samples) override {
    float mult = (m_gens < 1000) ? m_gens / 1000.0f : 1.0f;
    for (unsigned i = 0; i < samples; ++i) {
      data[i] = mult * ((m_gens / 20) % 2) - 0.5f;
      m_gens = m_gens + 1;
    }
  }
};

int main() {
  static audio s{};

  sitime::stopwatch time{};

  while (time.millis() < 1000) {
    // Wait
  }
}
