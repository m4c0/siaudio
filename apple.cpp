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
class pimpl {
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

public:
  pimpl() : m_tone_unit{create_tone_unit()} {}
  ~pimpl() {
    if (m_tone_unit == nullptr)
      return;

    AudioOutputUnitStop(m_tone_unit);
    AudioUnitUninitialize(m_tone_unit);
    AudioComponentInstanceDispose(m_tone_unit);
  }
};

os_streamer::os_streamer() : m_pimpl{new pimpl{}} {}
os_streamer::~os_streamer() = default;
} // namespace siaudio
