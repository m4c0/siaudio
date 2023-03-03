export module siaudio;
import hai;

namespace siaudio {
class pimpl;
export class os_streamer {
  hai::uptr<pimpl> m_pimpl;

protected:
  os_streamer();
  ~os_streamer();

public:
  static constexpr const auto channels = 1;
  static constexpr const auto rate = 44100;

  virtual void fill_buffer(float *f, unsigned num_samples) {}
};

export template <typename Producer> class streamer : os_streamer {
  Producer m_prod;

public:
  explicit streamer(Producer &&p) : os_streamer{}, m_prod{p} {}

  void fill_buffer(float *f, unsigned num_samples) override {
    m_prod(f, num_samples);
  }
};

} // namespace siaudio
