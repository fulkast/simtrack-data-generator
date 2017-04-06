#include "scene_rendering/rendered_scene.h"

int main(int argc, char const *argv[]) {

  // load object mesh and pose info
  // load camera in
  std::string hdf5_file_name =
    "/home/seasponge/Workspace/random-trees-with-simtrack/simtrack-data-generator/scripts/obj_file_name.h5";
  RenderedScene myscene(hdf5_file_name);
  return 0;
}
