#pragma leco tool
import siaudio;
import sitime;

static constexpr const auto rate = 44100;

static unsigned m_gens{};
static void fill_buffer(float *data, unsigned samples) {
  float mult = (m_gens < 1000) ? m_gens / 1000.0f : 1.0f;
  for (unsigned i = 0; i < samples; ++i) {
    data[i] = 0.5f * (mult * ((m_gens / 20) % 2) - 0.5f);
    m_gens = m_gens + 1;
  }
}
static void fill_buffer_2(float *data, unsigned samples) {
  for (unsigned i = 0; i < samples; ++i) {
    data[i] = 0.5f * (((m_gens / 10) % 2) - 0.5f);
    m_gens = m_gens + 1;
  }
}

int main() {
  siaudio::filler(fill_buffer);
  siaudio::rate(rate);

  sitime::stopwatch time{};

  while (time.millis() < 1000) {
    // Wait
  }

  siaudio::rate(0); // Mute
  while (time.millis() < 2000) {
    // Wait
  }

  siaudio::filler(fill_buffer_2);
  siaudio::rate(rate);
  while (time.millis() < 3000) {
    // Wait
  }
}
