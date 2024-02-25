export module siaudio;
import hai;
import traits;

namespace siaudio {
class pimpl;
export class os_streamer {
  hai::uptr<pimpl> m_pimpl;
  float m_rate;

protected:
  os_streamer(unsigned rate = 44100);
  ~os_streamer();

  [[nodiscard]] constexpr auto frate() const noexcept { return m_rate; }

public:
  static constexpr const auto channels = 1;

  virtual void fill_buffer(float *f, unsigned num_samples) = 0;

  void start();
  void stop();
};

export class timed_streamer : siaudio::os_streamer {
  volatile unsigned m_index{};
  unsigned m_ref_t{};

  [[nodiscard]] constexpr float time(unsigned idx) const noexcept {
    return static_cast<float>(idx) / frate();
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

export class ring_buffered_stream : public siaudio::os_streamer {
  static constexpr const auto buf_len = 441000;
  hai::array<float> m_cbuf{buf_len};
  volatile unsigned m_rp = 0;
  volatile unsigned m_wp = 0;

public:
  void fill_buffer(float *f, unsigned num_samples) override {
    while (num_samples > 0 && m_rp != m_wp) {
      *f++ = m_cbuf[m_rp];
      m_rp = (m_rp + 1) % buf_len;
      num_samples--;
    }
  }

  void push_frame(float *f, unsigned num_samples) {
    while (num_samples > 0 && m_rp != m_wp + 1) {
      m_cbuf[m_wp] = *f++;
      m_wp = (m_wp + 1) % buf_len;
      num_samples--;
    }
  }

  void flush() {
    m_rp = 0;
    m_wp = 0;
    for (auto &f : m_cbuf)
      f = 0;
  }
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
