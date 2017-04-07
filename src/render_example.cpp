//
// Created by seasponge on 07.04.17.
//

#include "scene_rendering/rendered_scene.h"

int main(int argc, char const *argv[]) {

  std::string h5filename{"/home/seasponge/Workspace/"
               "random-trees-with-simtrack/"
               "simtrack-data-generator/"
               "tests/test_data/obj_file_name.h5"};
  RenderedScene myscene_(h5filename);
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Display window", myscene_.getRenderedTexture() );   // Show our image inside it.

  cv::waitKey(0);                       // Wait for a keystroke in the window

  return 0;
}