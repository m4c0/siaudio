export module siaudio;
import hai;
import traits;

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

  void start();
  void stop();
};

export template <typename Producer> class streamer : os_streamer {
  Producer m_prod;

public:
  explicit streamer(Producer &&p) : os_streamer{}, m_prod{traits::move(p)} {}

  [[nodiscard]] constexpr const auto &producer() const noexcept {
    return m_prod;
  }
  [[nodiscard]] constexpr auto &producer() noexcept { return m_prod; }

  void fill_buffer(float *f, unsigned num_samples) override {
    m_prod(f, num_samples);
  }
};

} // namespace siaudio

#ifdef LECO_TARGET_ANDROID
#pragma leco add_impl android android_audio
#pragma leco add_system_ilbrary aaudio
#elif LECO_TARGET_APPLE
#pragma leco add_impl apple
#pragma leco add_framework AudioToolbox
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#pragma leco add_system_library ole32
#endif
