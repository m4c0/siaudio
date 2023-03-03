#include "../hai/build.hpp"
#include "ecow.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");

  auto m = all->add_unit<per_feat<mod>>("siaudio");
  m->for_feature(android_ndk).add_wsdep("hai", hai());
  m->for_feature(webassembly).add_wsdep("hai", hai());

  auto &objc = m->for_feature(objective_c);
  objc.add_wsdep("hai", hai());
  objc.add_impl("apple");
  objc.add_framework("AudioToolbox");

  auto &win = m->for_feature(windows_api);
  win.add_wsdep("hai", hai());
  win.add_impl("windows");

  auto poc = all->add_unit<app>("poc");
  poc->add_ref(m);
  poc->add_unit<>("poc");

  return run_main(all, argc, argv);
}
