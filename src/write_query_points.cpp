//
// Created by seasponge on 09.05.17.
//

#include "tracker_interface/track_single_object.h"
#include <algorithm>


int main()
{
  std::string h5filename{"/home/seasponge/Workspace/"
                           "random-trees-with-simtrack/"
                           "simtrack-data-generator/"
                           "tests/test_data/obj_file_name.h5"};
  TrackSingleObject trackSingleObject(h5filename);

  // obtain valid points in the obj frame
  auto points = trackSingleObject.getValidPointsAtPose();
  auto obj_inverse_transform = trackSingleObject.getObjectPose().inverseTransform();
  std::vector<pose::TranslationRotation3D> points_obj_frame;
  for (auto point : points) {
    points_obj_frame.push_back(obj_inverse_transform*point);
  }

  std::string out_file_name = "/home/seasponge/Workspace/random-trees-with-simtrack/simtrack-data-generator/data/query_points_3d/ikea_mug_points.h5";

  util::HDF5File out_file(out_file_name);

  {
    std::vector<int> size{ points_obj_frame.size(), 3 };
    std::vector<double> data;
    data.reserve(points_obj_frame.size() * 3);
    for (auto point : points_obj_frame) {
      double buffer[3];
      point.getT(buffer);
      data.push_back(buffer[0]);
      data.push_back(buffer[1]);
      data.push_back(buffer[2]);
    }

    // the order here is ROW-major, traversing the rows first.
    out_file.writeArray("t", data, size);
  }

  return 0;
}