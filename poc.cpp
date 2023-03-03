import siaudio;

int main() {
  auto gens = 0;
  auto s = siaudio::streamer{
      [&](float *data, unsigned samples) { gens += samples; }};
}
