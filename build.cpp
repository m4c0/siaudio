#include "build.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto poc = unit::create<app>("poc");
  poc->add_ref(siaudio());
  poc->add_unit<>("poc");
  poc->add_feat<export_symbol>("blip");
  return run_main(poc, argc, argv);
}
