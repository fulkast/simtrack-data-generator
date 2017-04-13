//
// Created by seasponge on 12.04.17.
//

#include "tracker_interface/track_single_object.h"

int main()
{
  std::string h5filename{"/home/seasponge/Workspace/"
                           "random-trees-with-simtrack/"
                           "simtrack-data-generator/"
                           "tests/test_data/obj_file_name.h5"};
  TrackSingleObject trackSingleObject(h5filename);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (true) {
//    trackSingleObject.brightenBackground(1);
    trackSingleObject.randomlyPerturbXUniform(35);
    auto image = trackSingleObject.getFlow();

    cv::namedWindow("texture_image", cv::WINDOW_AUTOSIZE);
    cv::imshow("texture_image", image);
    cv::waitKey(0);
  }
#pragma clang diagnostic pop

  return 0;
}