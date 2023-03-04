#include <aaudio/AAudio.h>

import hai;
import siaudio;

static aaudio_data_callback_result_t callback(AAudioStream * /*stream*/,
                                              void *user, void *audio,
                                              int32_t number_frames) {
  auto *str = static_cast<siaudio::os_streamer *>(user);
  auto *f_data = static_cast<float *>(audio);
  auto f_len = static_cast<unsigned>(number_frames);
  str->fill_buffer(f_data, f_len);
  return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

extern "C" AAudioStream *siaudio_setup_aas(siaudio::os_streamer *str) {
  AAudioStreamBuilder *builder{};
  AAudio_createStreamBuilder(&builder);
  // hai::holder<AAudioStreamBuilder, AAudioStreamBuilder_delete>
  // guard{builder};

  AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_EXCLUSIVE);
  AAudioStreamBuilder_setPerformanceMode(builder,
                                         AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
  AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_FLOAT);
  AAudioStreamBuilder_setChannelCount(builder, siaudio::os_streamer::channels);
  AAudioStreamBuilder_setSampleRate(builder, siaudio::os_streamer::rate);
  AAudioStreamBuilder_setDataCallback(builder, &callback, str);

  AAudioStream *stream{};
  AAudioStreamBuilder_openStream(builder, &stream);
  AAudioStream_requestStart(stream);
  return stream;
}
extern "C" void siaudio_teardown_aas(AAudioStream *aas) {
  AAudioStream_close(aas);
}
