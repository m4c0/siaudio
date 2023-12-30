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

export class timed_streamer : siaudio::os_streamer {
  volatile unsigned m_index{};
  unsigned m_ref_t{};

  [[nodiscard]] constexpr float time(unsigned idx) const noexcept {
    constexpr const auto frate = static_cast<float>(siaudio::os_streamer::rate);
    return static_cast<float>(idx) / frate;
  }

protected:
  void fill_buffer(float *buf, unsigned len) override {
    auto idx = m_index;
    for (auto i = 0; i < len; ++i, ++idx) {
      *buf++ = vol_at(time(idx - m_ref_t));
    }
    m_index = idx;
  }

  void reset_ref() { m_ref_t = m_index; }

  virtual constexpr float vol_at(float t) const noexcept = 0;
};
} // namespace siaudio

#ifdef LECO_TARGET_ANDROID
#pragma leco add_impl android android_audio
#pragma leco add_library aaudio
#elif LECO_TARGET_APPLE
#pragma leco add_impl apple
#pragma leco add_framework AudioToolbox
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#pragma leco add_library ole32
#endif
