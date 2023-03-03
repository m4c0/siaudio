export module siaudio;
import hai;

namespace siaudio {
class pimpl;
class os_streamer {
  hai::uptr<pimpl> m_pimpl;

protected:
  os_streamer();
  ~os_streamer();
};

export template <typename Producer> class streamer : os_streamer {
  Producer m_prod;

public:
  static constexpr const auto channels = 1;
  static constexpr const auto rate = 44100;

  explicit streamer(Producer &&p) : os_streamer{}, m_prod{p} {}
};

} // namespace siaudio
