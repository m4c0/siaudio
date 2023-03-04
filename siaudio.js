function (obj) {
  var streamer;
  var audioContext;

  function audio_init() {
    const AudioContext = window.AudioContext || window.webkitAudioContext;
    audioContext = new AudioContext();
  }

  function play() {
    if (!audioContext) audio_init();

    const mem_buf = obj.instance.exports.memory.buffer;

    const param_ptr = obj.instance.exports.siaudio_fill_buffer(streamer);
    const params = new Uint32Array(mem_buf, param_ptr, 4 * 4);

    const channels = params[0];
    const rate = params[1];
    const buf_size = params[2];
    const buf_ptr = params[3];

    const audioBuffer = audioContext.createBuffer(channels, buf_size, rate);

    const view = new Float32Array(mem_buf, buf_ptr, buf_size);
    audioBuffer.getChannelData(0).set(view);

    const src = audioContext.createBufferSource();
    src.buffer = audioBuffer;
    src.connect(audioContext.destination);
    src.start();

    src.onended = play;
  }

  play();

  // TODO: write down why this is necessary and how it impacts audio timestamps
  document.body.onclick = () => {
    if (audioContext != null) audioContext.resume();
  };
}
