module siaudio;

extern "C" void __attribute__((import_module("siaudio"), import_name("start")))
siaudio_start(void *, unsigned);

// TODO: receive this from JS. "128" is the expected standard, but who knows?
static constexpr const unsigned desired_buf_size = 128;
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
  siaudio::fn(buffer, desired_buf_size);
}
