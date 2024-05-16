module siaudio;

struct AAudioStream;
extern "C" AAudioStream *siaudio_setup_aas(siaudio::os_streamer *);
extern "C" void siaudio_teardown_aas(AAudioStream *);

namespace siaudio {
class streamer {
  AAudioStream *m_aas{};

public:
  streamer() : m_aas{siaudio_setup_aas(str)} {};
  ~streamer() { siaudio_teardown_aas(m_aas); }
};

streamer_t create(unsigned rate) { return streamer_t{new streamer{rate}}; }
void deleter::operator()(streamer *s) { delete s; }
} // namespace siaudio
