function (pimpl) {
  const AudioContext = window.AudioContext || window.webkitAudioContext;
  var audioContext = new AudioContext();

  const pimpl_size = 4 * 4;
  const params = new Uint32Array(ecow_buffer, pimpl, 4);
  const channels = params[1];
  const rate = params[2];
  const buf_size = params[3];
  const view = new Float32Array(ecow_buffer, pimpl + pimpl_size, buf_size);

  const audioBuffer = audioContext.createBuffer(channels, buf_size, rate);
  function play() {
    ecow_exports.siaudio_fill_buffer(pimpl);
    audioBuffer.copyToChannel(view, 0);

    const src = audioContext.createBufferSource();
    src.buffer = audioBuffer;
    src.connect(audioContext.destination);
    src.onended = play;
    src.start();
  }

  play();

  // TODO: write down why this is necessary and how it impacts audio timestamps
  document.body.onclick = audioContext.resume.bind(audioContext);
}
