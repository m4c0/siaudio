function (obj) {
  const audioFrameCount = 10240;
  var audioContext;

  function audio_init() {
    const AudioContext = window.AudioContext || window.webkitAudioContext;
    audioContext = new AudioContext();
  }

  function play() {
    if (!audioContext) audio_init();

    const audioBuffer = audioContext.createBuffer(1, audioFrameCount, 44100);

    const view = new Float32Array(
      obj.instance.exports.memory.buffer,
      obj.instance.exports.siaudio_fill_buffer(audioFrameCount),
      audioFrameCount);
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
