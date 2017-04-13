//
// Created by seasponge on 12.04.17.
//

#ifndef SIMTRACK_DATA_GENERATOR_TRACK_SINGLE_OBJECT_H
#define SIMTRACK_DATA_GENERATOR_TRACK_SINGLE_OBJECT_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <hdf5_file.h>
#include <device_2d.h>
#include <utilities.h>
#include <multi_rigid_tracker.h>

class TrackSingleObject {
public:
  TrackSingleObject(std::string hdf5_file_name);

  cv::Mat getTexture() const;

  cv::Mat getFlow() const;

  cv::Mat getObjectMask() const;

  // given the object is a the current_pose, return a vector of valid
  // feature points in pixel coordinates
  std::vector<cv::Point2i> getValidPointsAtPose(pose::TranslationRotation3D
                                                current_pose) ;

  void brightenBackground(int);

  void randomlyPerturbXUniform(float);

private:

  vision::D_OpticalAndARFlow::Parameters flow_parameters_;
  pose::D_MultipleRigidPoses::Parameters pose_parameters_;
  int image_height_;
  int image_width_;

  interface::MultiRigidTracker::Ptr multi_rigid_tracker_ptr_;

  std::vector<int> background_data_;

};



#endif //SIMTRACK_DATA_GENERATOR_TRACK_SINGLE_OBJECT_H
