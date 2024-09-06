module siaudio;

static siaudio::streamer_t &instance() {
  static siaudio::streamer_t i{};
  return i;
}

static void dummy_fn(float * p, unsigned n) {
  for (auto i = 0; i < n; i++) *p++ = 0;
}
siaudio::fn_t siaudio::fn = dummy_fn;

void siaudio::filler(siaudio::fn_t fn) { siaudio::fn = fn; }
void siaudio::rate(unsigned r) {
  instance() = r == 0 ? streamer_t{} : siaudio::create(r);
}
