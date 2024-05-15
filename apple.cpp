module;
#include <AudioToolbox/AudioComponent.h>
#include <AudioToolbox/AudioOutputUnit.h>
#include <AudioToolbox/AudioUnitProperties.h>

#if TARGET_OS_OSX
#define COMPONENT_SUBTYPE kAudioUnitSubType_DefaultOutput;
#else
#define COMPONENT_SUBTYPE kAudioUnitSubType_RemoteIO;
#endif

module siaudio;

namespace siaudio {
class streamer {
  AudioComponentInstance m_tone_unit;

  static AudioComponentInstance create_tone_unit() {
    AudioComponentDescription acd;
    acd.componentType = kAudioUnitType_Output;
    acd.componentSubType = COMPONENT_SUBTYPE;
    acd.componentManufacturer = kAudioUnitManufacturer_Apple;
    acd.componentFlags = 0;
    acd.componentFlagsMask = 0;

    AudioComponent ac = AudioComponentFindNext(nullptr, &acd);
    if (ac == nullptr)
      return nullptr;

    AudioComponentInstance tone_unit{};
    if (AudioComponentInstanceNew(ac, &tone_unit) != noErr)
      return nullptr;

    return tone_unit;
  }

  static OSStatus render(void * /*ref*/, AudioUnitRenderActionFlags * /*flags*/,
                         const AudioTimeStamp * /*timestamp*/,
                         UInt32 /*bus_number*/, UInt32 number_frames,
                         AudioBufferList *data) {
    auto *f_data = static_cast<float *>(data->mBuffers[0].mData);
    siaudio::fn(f_data, number_frames);
    return noErr;
  }

  bool set_render_callback() {
    AURenderCallbackStruct rcs;
    rcs.inputProc = render;
    // rcs.inputProcRefCon = s;
    return noErr == AudioUnitSetProperty(
                        m_tone_unit, kAudioUnitProperty_SetRenderCallback,
                        kAudioUnitScope_Input, 0, &rcs, sizeof(rcs));
  }

  bool set_format(unsigned rate) {
    constexpr const auto bits_per_byte = 8;

    AudioStreamBasicDescription sbd;
    sbd.mSampleRate = rate;
    sbd.mFormatID = kAudioFormatLinearPCM;
    sbd.mFormatFlags =
        static_cast<unsigned>(kAudioFormatFlagsNativeFloatPacked) |
        kAudioFormatFlagIsNonInterleaved;
    sbd.mBytesPerPacket = sizeof(float);
    sbd.mFramesPerPacket = 1;
    sbd.mBytesPerFrame = sizeof(float) / 1;
    sbd.mChannelsPerFrame = 1;
    sbd.mBitsPerChannel = sizeof(float) * bits_per_byte;
    return noErr ==
           AudioUnitSetProperty(m_tone_unit, kAudioUnitProperty_StreamFormat,
                                kAudioUnitScope_Input, 0, &sbd, sizeof(sbd));
  }

  bool init() { return AudioUnitInitialize(m_tone_unit) == noErr; }

public:
  streamer(unsigned rate) : m_tone_unit{create_tone_unit()} {
    if (!set_render_callback())
      return;
    if (!set_format(rate))
      return;
    if (!init())
      return;

    AudioOutputUnitStart(m_tone_unit);
  }
  ~streamer() {
    if (m_tone_unit == nullptr)
      return;

    AudioOutputUnitStop(m_tone_unit);
    AudioUnitUninitialize(m_tone_unit);
    AudioComponentInstanceDispose(m_tone_unit);
  }

  void start() { AudioOutputUnitStart(m_tone_unit); }
  void stop() { AudioOutputUnitStop(m_tone_unit); }
};

streamer_t create(unsigned rate) { return streamer_t{new streamer{rate}}; }
void deleter::operator()(streamer *s) { delete s; }
} // namespace siaudio
