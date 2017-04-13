//
// Created by seasponge on 12.04.17.
//

#include "../include/tracker_interface/track_single_object.h"

TrackSingleObject::TrackSingleObject(std::string hdf5_file_name) {

  // Load parameters
  std::cout << "reading " << hdf5_file_name << std::endl;
  std::cout << "\n\n";
  util::HDF5File in_file(hdf5_file_name);
  if (!in_file.checkVariableExists("obj_file_name"))
    std::cerr << "OBJ File name doesn't exist in " << hdf5_file_name << std::endl;
  if (!in_file.checkVariableExists("width"))
    std::cerr << "Image Width doesn't exist in " << hdf5_file_name << std::endl;
  if (!in_file.checkVariableExists("height"))
    std::cerr << "Image Height doesn't exist in " << hdf5_file_name << std::endl;
  if (!in_file.checkVariableExists("camera_matrix"))
    std::cerr << "Camera Matrix doesn't exist in " << hdf5_file_name << std::endl;

  std::string obj_name = in_file.readScalar<std::string>("obj_file_name");
  std::cout << obj_name << " is the object name" << std::endl;

  cv::Mat camera_matrix;
  {
    std::vector<int> size;
    std::vector<double> data;
    in_file.readArray("camera_matrix", data, size);
    if ((size.at(0) != 3) || (size.at(1) != 4))
      throw std::runtime_error("Expecting 4x3 camera_matrix");
    camera_matrix = cv::Mat(3, 4, CV_64FC1, data.data()).clone();
  }

  std::vector<interface::MultiRigidTracker::ObjectInfo> object_info;
  object_info.push_back(interface::MultiRigidTracker::ObjectInfo(
      "dummy_label", obj_name));

  image_width_  = in_file.readScalar<int>("width");
  image_height_ = in_file.readScalar<int>("height");
  flow_parameters_.consistent_ = true;
//  pose_parameters_.w_ar_flow_  = 0.;
//  pose_parameters_.w_disp_     = 0.;
//  pose_parameters_.w_flow_     = 0.;

  multi_rigid_tracker_ptr_ =
    interface::MultiRigidTracker::Ptr(new interface::MultiRigidTracker(
      image_width_, image_height_,
      camera_matrix, object_info,
      flow_parameters_, pose_parameters_ ));

  background_data_ = std::vector<int>(image_height_ * image_width_, 255);
  cv::Mat image(image_height_, image_width_, CV_8UC1, background_data_.data());

  multi_rigid_tracker_ptr_->updatePoses(image);
}

cv::Mat TrackSingleObject::getTexture() const {
  return multi_rigid_tracker_ptr_->generateOutputImage(
    interface::MultiRigidTracker::OutputImageType::
    model_appearance_blended);
}

cv::Mat TrackSingleObject::getFlow() const {
  return multi_rigid_tracker_ptr_->generateOutputImage(
    interface::MultiRigidTracker::OutputImageType::
    optical_flow_x);
}

cv::Mat TrackSingleObject::getObjMask() const {
  return mu
}

void TrackSingleObject::randomlyPerturbXUniform(float mag) {

  auto poses = multi_rigid_tracker_ptr_->getPoses();
  auto pose = &poses[0];

  double T[3] = {0,0,0.5};
  double R[3] = {0,1,0};
  pose->setR(R);
  pose->setT(T);

  multi_rigid_tracker_ptr_->setPoses(poses);
  cv::Mat new_background(image_height_, image_width_,
                         CV_8UC1, background_data_.data() );
  multi_rigid_tracker_ptr_->updatePoses(new_background);

  double rand_scalar = ( double(std::rand())/double(std::numeric_limits<int>::max()) - 0.5) * mag;
  std::cout << "rand scalar: " << rand_scalar << std::endl;
  pose->rotateX( rand_scalar);
  multi_rigid_tracker_ptr_->setPoses(poses);
  multi_rigid_tracker_ptr_->updatePoses(new_background);
}

void TrackSingleObject::brightenBackground(int increment) {
  std::transform(background_data_.begin(), background_data_.end(),
    background_data_.begin(), [increment] (int &x) { return (x + std::rand()) % 255; });
  std::cout << background_data_[0] << " is the current background brightness" << std::endl;
  cv::Mat new_background(image_height_, image_width_,
                         CV_8UC1, background_data_.data() );
  multi_rigid_tracker_ptr_->updatePoses(new_background);
}

std::vector<cv::Point2i> TrackSingleObject::getValidPointsAtPose(pose::TranslationRotation3D current_pose)  {
  std::vector<pose::TranslationRotation3D> poses(1,current_pose);
  multi_rigid_tracker_ptr_->setPoses(poses);

  cv::Mat new_background(image_height_, image_width_, CV_8UC1, background_data_.data() );
  multi_rigid_tracker_ptr_->updatePoses(new_background);



}