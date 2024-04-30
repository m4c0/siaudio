module siaudio;

namespace siaudio {
class pimpl {
public:
  pimpl(os_streamer *s, unsigned rate) {}
  ~pimpl() {}

  void start() {}
  void stop() {}
};

os_streamer::os_streamer(unsigned rate)
    : m_pimpl{new pimpl{this, rate}}, m_rate{static_cast<float>(rate)} {}
os_streamer::~os_streamer() = default;
void os_streamer::start() { m_pimpl->start(); }
void os_streamer::stop() { m_pimpl->stop(); }
} // namespace siaudio
