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
    trackSingleObject.randomlyPerturbXUniform(35);
    auto mask  = trackSingleObject.getDefaultObjectMask();
    auto new_pose = trackSingleObject.getObjectMask();

    auto image = trackSingleObject.getFlow();

    cv::Mat I(image.size(), image.type(), cv::Scalar(0));
    cv::bitwise_and(image,I,image, 1-mask);

    cv::cvtColor(new_pose,new_pose,CV_GRAY2BGRA);
    cv::hconcat(image, new_pose, image);

    auto points = trackSingleObject.getValidPointsAtPose();

    for (auto point : points) {
      std::cout << point << std::endl;
    }



//    cv::fillConvexPoly(I, points, cv::Scalar(225,255,255));

    cv::namedWindow("texture_image", cv::WINDOW_AUTOSIZE);
    cv::imshow("texture_image", image);
    cv::waitKey(0);
  }
#pragma clang diagnostic pop

  return 0;
}