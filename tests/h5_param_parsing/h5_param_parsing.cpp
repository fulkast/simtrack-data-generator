#include "gtest/gtest.h"
#include <cmath>
#include <string>
#include "scene_rendering/rendered_scene.h"


std::string getOBJName() {
  std::string hdf5_file_name =
    "/home/seasponge/Workspace/random-trees-with-simtrack/simtrack-data-generator/scripts/obj_file_name.h5";
  RenderedScene myscene(hdf5_file_name);
  return myscene.getOBJName();
}

TEST(h5ParamParse, FileName) {

  EXPECT_EQ ( 0, getOBJName().compare("pringles"));

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
