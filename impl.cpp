module siaudio;

static siaudio::streamer_t &instance() {
  static siaudio::streamer_t i{};
  return i;
}

void (*siaudio::fn)(float *, unsigned) = [](float *, unsigned) {};

void siaudio::filler(void (*fn)(float *, unsigned)) { siaudio::fn = fn; }
void siaudio::rate(unsigned r) { instance() = siaudio::create(r); }
