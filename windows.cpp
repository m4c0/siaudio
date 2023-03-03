module;
#include <wrl/client.h>
#include <xaudio2.h>

module siaudio;

namespace siaudio {
struct destroyer {
  void operator()(IXAudio2Voice *ptr) { ptr->DestroyVoice(); }
};

class pimpl : public IXAudio2VoiceCallback {
  Microsoft::WRL::ComPtr<IXAudio2> m_xa2{};
  hai::uptr<IXAudio2MasteringVoice, destroyer> m_main_voice{};
  hai::uptr<IXAudio2SourceVoice, destroyer> m_src_voice{};
  os_streamer *m_owner;

public:
  pimpl(os_streamer *o) : m_owner{o} {
    HRESULT hr{};
    if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
      return;
    }

    if (FAILED(hr = XAudio2Create(m_xa2.GetAddressOf(), 0,
                                  XAUDIO2_DEFAULT_PROCESSOR))) {
      return;
    }

    if (FAILED(hr = m_xa2->CreateMasteringVoice(&*m_main_voice))) {
      return;
    }

    constexpr const auto channels = os_streamer::channels;
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
    if (FAILED(hr = m_xa2->CreateSourceVoice(&*m_src_voice, &wfx, 0, ratio,
                                             this))) {
      return;
    }

    if (FAILED(hr = sv->Start())) {
      return;
    }
  }

  void OnBufferEnd(void *pBufferContext) noexcept override {}
  void OnBufferStart(void *pBufferContext) noexcept override {}
  void OnLoopEnd(void *pBufferContext) noexcept override {}
  void OnStreamEnd() noexcept override {}
  void OnVoiceError(void *pBufferContext, HRESULT Error) noexcept override {}
  void OnVoiceProcessingPassEnd() noexcept override {}
  void OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override {
    m_buffer.resize(BytesRequired / sizeof(float));
    m_owner->fill_buffer(m_buffer, X);

    XAUDIO2_BUFFER buf{
        .AudioBytes = BytesRequired,
        .pAudioData = reinterpret_cast<BYTE *>(m_buffer.data()), // NOLINT
    };
    m_src_voice->SubmitSourceBuffer(&buf);
  }
};

os_streamer::os_streamer() : m_pimpl{new pimpl{this}} {}
os_streamer::~os_streamer() = default;
} // namespace siaudio
