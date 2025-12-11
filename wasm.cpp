module siaudio;

extern "C" void __attribute__((import_module("siaudio"), import_name("start")))
siaudio_start(void *, unsigned);

// TODO: receive this from JS. "128" is the expected standard, but who knows?
static constexpr const unsigned js_window_size = 128;
static constexpr const unsigned js_window_count = 4; // Enough to allow some JS events
static constexpr const unsigned desired_buf_size = js_window_size * js_window_count;
static float buffer[desired_buf_size] {};

namespace siaudio {
streamer_t create(unsigned rate) { 
  siaudio_start(buffer, desired_buf_size);
  return streamer_t{nullptr}; 
}
void deleter::operator()(streamer *s) {}
} // namespace siaudio

extern "C" void __attribute__((export_name("siaudio_fill_buffer")))
siaudio_fill_buffer() {
  unsigned n = desired_buf_size;
  siaudio::fn(buffer, n);
}
