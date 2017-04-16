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

  // obtain valid points in the obj frame
  auto points = trackSingleObject.getValidPointsAtPose();
  auto obj_inverse_transform = trackSingleObject.getObjectPose().inverseTransform();
  std::vector<pose::TranslationRotation3D> points_obj_frame;
  for (auto point : points) {
    points_obj_frame.push_back(obj_inverse_transform*point);
  }

  while (true) {

    trackSingleObject.randomlyPerturbXUniform(35);
    auto mask       = trackSingleObject.getDefaultObjectMask();
    auto new_mask   = trackSingleObject.getObjectMask();
    auto flow_image = trackSingleObject.getFlow();

    cv::Mat output(flow_image.size(), flow_image.type(), cv::Scalar(0));

    cv::Mat I(flow_image.size(), flow_image.type(), cv::Scalar(0));
    cv::bitwise_and(flow_image,I,flow_image, 1-mask);

    cv::cvtColor(new_mask,new_mask,CV_GRAY2BGRA);
    cv::hconcat(flow_image, new_mask, flow_image);

    auto object_pose = trackSingleObject.getObjectPose();
    object_pose.showCompact();

    std::vector<cv::Point> pixel_points_camera;
    for (auto point : points_obj_frame) {
      auto point_camera_frame = trackSingleObject.getCameraPose().inverseTransform()*
                                (object_pose*point);

      float T[3];
      point_camera_frame.getT(T);
      int x = T[0] / T[2] * trackSingleObject.fx_ + trackSingleObject.cx_;
      int y = T[1] / T[2] * trackSingleObject.fy_ + trackSingleObject.cy_;

      pixel_points_camera.push_back(cv::Point(x,y));
      cv::circle(output, cv::Point(x,y),0.5,cv::Scalar(255,255,255));
    }

    cv::hconcat(flow_image, output, output);

    cv::namedWindow("texture_image", cv::WINDOW_AUTOSIZE);
    cv::imshow("texture_image", output);
    cv::waitKey(0);
  }
#pragma clang diagnostic pop

  return 0;
}