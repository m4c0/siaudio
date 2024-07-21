module siaudio;

static siaudio::streamer_t &instance() {
  static siaudio::streamer_t i{};
  return i;
}

void (*siaudio::fn)(float *, unsigned) = [](float *p, unsigned n) {
  for (auto i = 0; i < n; i++)
    *p++ = 0;
};

void siaudio::filler(void (*fn)(float *, unsigned)) { siaudio::fn = fn; }
void siaudio::rate(unsigned r) {
  instance() = r == 0 ? streamer_t{} : siaudio::create(r);
}
