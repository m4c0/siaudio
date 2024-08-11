export module siaudio;
import hai;

namespace siaudio {
export void filler(void (*)(float *, unsigned));
export void rate(unsigned);

extern void (*fn)(float *, unsigned);

class streamer;
struct deleter {
  void operator()(streamer *);
};
using streamer_t = hai::holder<streamer, deleter>;
streamer_t create(unsigned rate);
} // namespace siaudio

#pragma leco add_impl impl

#ifdef LECO_TARGET_ANDROID
#pragma leco add_impl android android_audio
#pragma leco add_library aaudio
#elif LECO_TARGET_APPLE
#pragma leco add_impl apple
#pragma leco add_framework AudioToolbox
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#pragma leco add_library ole32
#elif LECO_TARGET_LINUX
#pragma leco add_impl linux
#elif LECO_TARGET_WASM
#pragma leco add_impl wasm
#endif
