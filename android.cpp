module siaudio;

struct AAudioStream;
extern "C" AAudioStream *siaudio_setup_aas(siaudio::os_streamer *);
extern "C" void siaudio_teardown_aas(AAudioStream *);

namespace siaudio {
class pimpl {
  AAudioStream *m_aas{};

public:
  pimpl(os_streamer *str) : m_aas{siaudio_setup_aas(str)} {};
  ~pimpl() { siaudio_teardown_aas(m_aas); }
};

os_streamer::os_streamer() : m_pimpl{new pimpl{this}} {}
os_streamer::~os_streamer() = default;
} // namespace siaudio
