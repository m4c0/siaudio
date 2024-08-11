!function() {
  function worker() {
    registerProcessor("siaudio", class extends AudioWorkletProcessor {
      constructor(...args) {
        super(...args);
        this.port.onmessage = this.onmessage.bind(this);
      }
      onmessage(e) {
        this.shared_data = e.data;
      }
      process(inputs, outputs, parameters) {
        this.port.postMessage({});
        if (this.shared_data) outputs[0][0].set(this.shared_data);
        return true;
      }
    });
  }
  const blob = new Blob(['(', worker, ')()'], { type: 'text/javascript' });
  const url = URL.createObjectURL(blob);

  function start(ptr, sz) {
    const cpp_buffer = new Float32Array(leco_exports.memory.buffer, ptr, sz);

    const sab = crossOriginIsolated ? new SharedArrayBuffer(4 * sz) : new ArrayBuffer(4 * sz);
    const sab_view = new Float32Array(sab);

    // AudioContext can only be played on a user interaction
    document.body.addEventListener("click", () => {
      const AudioContext = window.AudioContext || window.webkitAudioContext;
      const audioContext = new AudioContext();
      audioContext.audioWorklet.addModule(url).then(function() {
        const node = new AudioWorkletNode(audioContext, "siaudio");
        node.connect(audioContext.destination);
        node.port.onmessage = function() {
          leco_exports.siaudio_fill_buffer();
          sab_view.set(cpp_buffer);
          node.port.postMessage(sab_view);
        };
      });

      audioContext.resume();
    });
  }
  leco_imports.siaudio = { start };
}();
