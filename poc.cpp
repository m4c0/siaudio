import siaudio;

int main() {
  volatile auto gens = 0;
  auto s = siaudio::streamer{[&](float *data, unsigned samples) {
    gens = gens + samples;
    for (unsigned i = 0; i < samples; i++)
      data[i] = ((i / 20) % 2) - 0.5f;
  }};
  while (gens < 44100) {
  }
}
