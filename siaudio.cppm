export module siaudio;
import hai;

namespace siaudio {
struct producer {
  virtual ~producer() = default;
  virtual void fill_buffer(float *data, unsigned samples) = 0;
};

class streamer {
  hai::uptr<producer> m_producer;

public:
  static constexpr const auto channels = 1;
  static constexpr const auto rate = 44100;

  virtual ~streamer() = default;

  [[nodiscard]] constexpr const auto &producer() const noexcept {
    return m_producer;
  }
  [[nodiscard]] constexpr auto &producer() noexcept { return m_producer; }

  static hai::uptr<streamer> create();
};
} // namespace siaudio
