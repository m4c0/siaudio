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
        if (this.shared_data) outputs[0][0].set(this.shared_data);
        this.port.postMessage({ currentFrame });
        return true;
      }
    });
  }
  const blob = new Blob(['(', worker, ')()'], { type: 'text/javascript' });
  const url = URL.createObjectURL(blob);
  var audioContext = null;

  function start(ptr, sz) {
    const cpp_buffer = new Float32Array(leco_exports.memory.buffer, ptr, sz);

    const sab = new SharedArrayBuffer(4 * sz);
    const sab_view = new Float32Array(sab);

    // AudioContext can only be played on a user interaction
    document.body.addEventListener("click", () => {
      if (audioContext) return;
      
      const AudioContext = window.AudioContext || window.webkitAudioContext;
      audioContext = new AudioContext({ sampleRate: 44100 });
      audioContext.audioWorklet.addModule(url).then(function() {
        const node = new AudioWorkletNode(audioContext, "siaudio");
        node.port.postMessage(sab_view);
        node.connect(audioContext.destination);
        node.port.onmessage = function() {
          leco_exports.siaudio_fill_buffer();
          sab_view.set(cpp_buffer);
        };
      });

      audioContext.resume();
    });
  }
  leco_imports.siaudio = { start };
}();
