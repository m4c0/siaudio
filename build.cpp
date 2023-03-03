#include "../hai/build.hpp"
#include "ecow.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");

  auto m = all->add_unit<per_feat<mod>>("siaudio");
  m->for_feature(android_ndk).add_wsdep("hai", hai());
  m->for_feature(objective_c).add_wsdep("hai", hai());
  m->for_feature(windows_api).add_wsdep("hai", hai());
  m->for_feature(webassembly).add_wsdep("hai", hai());

  return run_main(all, argc, argv);
}
