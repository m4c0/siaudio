function (pimpl) {
  const pimpl_size = 4 * 4;
  const params = new Uint32Array(ecow_buffer, pimpl, 4);
  const channels = params[1];
  const rate = params[2];
  const buf_size = params[3];
  const view = new Float32Array(ecow_buffer, pimpl + pimpl_size, buf_size);

  const AudioContext = window.AudioContext || window.webkitAudioContext;
  var audioContext;

  // AudioContext should only be created on a user interaction
  document.body.onclick = function() {
    if (audioContext) return;

    audioContext = new AudioContext();

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
  }
}
