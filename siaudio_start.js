function (pimpl) {
  var audioContext;

  function audio_init() {
    const AudioContext = window.AudioContext || window.webkitAudioContext;
    audioContext = new AudioContext();
  }

  function play() {
    if (!audioContext) audio_init();

    ecow_exports.siaudio_fill_buffer(pimpl);

    const pimpl_size = 4 * 4;
    const params = new Uint32Array(ecow_buffer, pimpl, 4);
    const channels = params[1];
    const rate = params[2];
    const buf_size = params[3];

    const audioBuffer = audioContext.createBuffer(channels, buf_size, rate);

    const view = new Float32Array(ecow_buffer, pimpl + pimpl_size, buf_size);
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
  }
}
