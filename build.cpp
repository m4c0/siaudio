#include "ecow.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");

  auto m = all->add_unit<per_feat<mod>>("siaudio");
  m->for_feature(android_ndk);
  m->for_feature(objective_c);
  m->for_feature(windows_api);
  m->for_feature(webassembly);

  return run_main(all, argc, argv);
}
