module siaudio;

extern "C" void siaudio_start(siaudio::pimpl *);

namespace siaudio {
class pimpl {
  static constexpr const unsigned desired_buf_size = os_streamer::rate / 10;

  os_streamer *m_str;
  unsigned m_channels = os_streamer::channels;
  unsigned m_rate = os_streamer::rate;
  unsigned m_buf_size = desired_buf_size;
  float m_buffer[desired_buf_size]{};

public:
  pimpl(os_streamer *s) : m_str{s} { siaudio_start(this); }

  void fill_buffer() { m_str->fill_buffer(m_buffer, m_buf_size); }
};

os_streamer::os_streamer() : m_pimpl{new pimpl{this}} {}
os_streamer::~os_streamer() = default;
} // namespace siaudio

extern "C" void siaudio_fill_buffer(siaudio::pimpl *p) { p->fill_buffer(); }
