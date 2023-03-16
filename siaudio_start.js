function (pimpl) {
  function worker() {
    registerProcessor("siaudio", class extends AudioWorkletProcessor {
      constructor(...args) {
        super(...args);
        this.port.onmessage = this.onmessage.bind(this);
      }
      onmessage(e) {
        this.sab_view = e.data;
      }
      process(inputs, outputs, parameters) {
        this.port.postMessage({});
        if (this.sab_view) outputs[0][0].set(this.sab_view);
        return true;
      }
    });
  }
  const blob = new Blob(['(', worker, ')()'], { type: 'text/javascript' });
  const url = URL.createObjectURL(blob);

  const AudioContext = window.AudioContext || window.webkitAudioContext;
  const audioContext = new AudioContext();
  audioContext.audioWorklet.addModule(url).then(function() {
    const node = new AudioWorkletNode(audioContext, "siaudio");
    node.connect(audioContext.destination);
    node.port.onmessage = function() {
      ecow_exports.siaudio_fill_buffer(pimpl);
      sab_view.set(view);
      node.port.postMessage(sab_view);
    };
  });

  const pimpl_size = 4 * 4;
  const params = new Uint32Array(ecow_buffer, pimpl, 4);
  const channels = params[1];
  const rate = params[2];
  const buf_size = params[3];
  const view = new Float32Array(ecow_buffer, pimpl + pimpl_size, buf_size);

  const sab = new SharedArrayBuffer(buf_size * 4);
  const sab_view = new Float32Array(sab);

  // AudioContext can only be played on a user interaction
  document.body.onclick = audioContext.resume.bind(audioContext);
}
