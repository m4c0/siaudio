import siaudio;

int main() {
  volatile auto gens = 0;
  auto s = siaudio::streamer{[&](float *data, unsigned samples) {
    float mult = (gens < 1000) ? gens / 1000.0f : 1.0f;
    gens = gens + samples;
    for (unsigned i = 0; i < samples; i++)
      data[i] = mult * ((i / 20) % 2) - 0.5f;
  }};
  while (gens < 44100) {
  }
}
