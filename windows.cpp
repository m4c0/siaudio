module;
#include <wrl/client.h>
#include <xaudio2.h>

module siaudio;
import hai;
import hay;

namespace siaudio {
void destroyer(IXAudio2Voice * ptr) { ptr->DestroyVoice(); }
template<typename X> using voice = hay<X *, nullptr, destroyer>;

class streamer : public IXAudio2VoiceCallback {
  Microsoft::WRL::ComPtr<IXAudio2> m_xa2{};
  voice<IXAudio2MasteringVoice> m_main_voice { nullptr };
  voice<IXAudio2SourceVoice   > m_src_voice  { nullptr };
  hai::array<float> m_buffer;

public:
  streamer(unsigned rate) : m_buffer{rate} {
    HRESULT hr{};
    if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
      return;
    }

    if (FAILED(hr = XAudio2Create(m_xa2.GetAddressOf(), 0,
                                  XAUDIO2_DEFAULT_PROCESSOR))) {
      return;
    }

    if (FAILED(hr = m_xa2->CreateMasteringVoice(m_main_voice))) {
      return;
    }

    constexpr const auto channels = 1;
    constexpr const auto bits_per_sample = 32; // IEEE_FORMAT
    constexpr const auto alignment = (channels * bits_per_sample) / 8;
    WAVEFORMATEX wfx{};
    wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    wfx.nChannels = channels;
    wfx.nSamplesPerSec = rate;
    wfx.nAvgBytesPerSec = rate * sizeof(float);
    wfx.nBlockAlign = alignment;
    wfx.wBitsPerSample = bits_per_sample;
    wfx.cbSize = 0;

    constexpr const auto ratio = XAUDIO2_DEFAULT_FREQ_RATIO;
    if (FAILED(hr = m_xa2->CreateSourceVoice(m_src_voice, &wfx, 0, ratio, this))) {
      return;
    }

    if (FAILED(hr = m_src_voice->Start())) {
      return;
    }
  }
  virtual ~streamer() = default;

  void OnBufferEnd(void *pBufferContext) noexcept override {}
  void OnBufferStart(void *pBufferContext) noexcept override {}
  void OnLoopEnd(void *pBufferContext) noexcept override {}
  void OnStreamEnd() noexcept override {}
  void OnVoiceError(void *pBufferContext, HRESULT Error) noexcept override {}
  void OnVoiceProcessingPassEnd() noexcept override {}
  void OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override {
    if (BytesRequired > m_buffer.size() * sizeof(float))
      BytesRequired = m_buffer.size() * sizeof(float);

    siaudio::fn(m_buffer.begin(), BytesRequired / sizeof(float));

    XAUDIO2_BUFFER buf{
        .AudioBytes = BytesRequired,
        .pAudioData = reinterpret_cast<BYTE *>(m_buffer.begin()), // NOLINT
    };
    m_src_voice->SubmitSourceBuffer(&buf);
  }

  void start() { m_src_voice->Start(); }
  void stop() { m_src_voice->Stop(); }
};

streamer_t create(unsigned rate) { return streamer_t{new streamer{rate}}; }
void deleter::operator()(streamer *s) { delete s; }
} // namespace siaudio
