module siaudio;

static constexpr const unsigned siaudio_buf_size =
    siaudio::os_streamer::rate / 10;
static float siaudio_buffer[siaudio_buf_size]{};
struct {
  unsigned channels = siaudio::os_streamer::channels;
  unsigned rate = siaudio::os_streamer::rate;
  unsigned buf_size = siaudio_buf_size;
  float *buffer = siaudio_buffer;
} siaudio_params;

extern "C" decltype(&siaudio_params)
siaudio_fill_buffer(siaudio::os_streamer *str) {
  return &siaudio_params;
}

namespace siaudio {
struct pimpl {};

os_streamer::os_streamer() {}
os_streamer::~os_streamer() {}
} // namespace siaudio
