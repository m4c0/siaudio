#include "../hai/build.hpp"
#include "ecow.hpp"

auto siaudio() {
  using namespace ecow;

  auto m = unit::create<per_feat<mod>>("siaudio");

  auto &wasm = m->for_feature(webassembly);
  wasm.add_wsdep("hai", hai());
  wasm.add_wsdep("traits", traits());
  wasm.add_impl("wasm");

  auto &droid = m->for_feature(android_ndk);
  droid.add_wsdep("hai", hai());
  droid.add_wsdep("traits", traits());
  droid.add_impl("android");
  droid.add_unit<>("android_audio");
  droid.add_system_library("aaudio");

  auto &objc = m->for_feature(objective_c);
  objc.add_wsdep("hai", hai());
  objc.add_wsdep("traits", traits());
  objc.add_impl("apple");
  objc.add_framework("AudioToolbox");

  auto &win = m->for_feature(windows_api);
  win.add_wsdep("hai", hai());
  win.add_wsdep("traits", traits());
  win.add_impl("windows");
  win.add_system_library("ole32");

  return m;
}
