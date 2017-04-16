//
// Created by seasponge on 12.04.17.
//

#include "../include/tracker_interface/track_single_object.h"

#include <utility_kernels_pose.h>

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

  {
    std::vector<int> size;
    std::vector<double> data;
    in_file.readArray("camera_matrix", data, size);
    if ((size.at(0) != 3) || (size.at(1) != 4))
      throw std::runtime_error("Expecting 4x3 camera_matrix");
    camera_matrix_ = cv::Mat(3, 4, CV_64FC1, data.data()).clone();
  }

  cx_ = camera_matrix_.at<double>(0, 2);
  cy_ = camera_matrix_.at<double>(1, 2);
  fx_ = camera_matrix_.at<double>(0, 0);
  fy_ = camera_matrix_.at<double>(1, 1);

  std::vector<interface::MultiRigidTracker::ObjectInfo> object_info;
  object_info.push_back(interface::MultiRigidTracker::ObjectInfo(
      "dummy_label", obj_name));

  image_width_  = in_file.readScalar<int>("width");
  image_height_ = in_file.readScalar<int>("height");
  flow_parameters_.consistent_ = false;


  multi_rigid_tracker_ptr_ =
    interface::MultiRigidTracker::Ptr(new interface::MultiRigidTracker(
      image_width_, image_height_,
      camera_matrix_, object_info,
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

cv::Mat TrackSingleObject::getDefaultObjectMask() const {
  return default_obj_mask_;
}

cv::Mat TrackSingleObject::getObjectMask() const {
  return multi_rigid_tracker_ptr_->getObjMask();
}

cv::Mat TrackSingleObject::getDepth() const {
  util::Device1D<float> z_data(image_width_*image_height_);
  auto z_buffer = multi_rigid_tracker_ptr_->getZBuffer();
  pose::convertZbufferToZ(z_data.data(), z_buffer,
                          image_width_, image_height_,
                          cx_, cy_,
                          pose_parameters_.near_plane_, pose_parameters_.far_plane_);

  std::vector<float> h_out(image_height_*image_width_);
  z_data.copyTo(h_out);

  cv::Mat result(image_height_, image_width_, CV_32FC1, h_out.data());
  return result;
}

float TrackSingleObject::randomlyPerturbXUniform(float mag) {

  setDefaultPose();
  updateBackgroundDefault();

  double rand_scalar = ( double(std::rand())/double(std::numeric_limits<int>::max()) - 0.5) * mag;
  auto poses = multi_rigid_tracker_ptr_->getPoses();
  auto pose = &poses[0];
  pose->rotateX( rand_scalar );
  multi_rigid_tracker_ptr_->setPoses(poses);
  updateBackgroundDefault();

  return rand_scalar;
}

void TrackSingleObject::setDefaultPose() {
  auto poses = multi_rigid_tracker_ptr_->getPoses();
  auto pose = &poses[0];

  double T[3] = {0.,0,0.5};
  double R[3] = {0,1.57,0};
  pose->setR(R);
  pose->setT(T);

  multi_rigid_tracker_ptr_->setPoses(poses);
  default_obj_mask_ = getObjectMask();
}

void TrackSingleObject::brightenBackground(int increment) {
  std::transform(background_data_.begin(), background_data_.end(),
    background_data_.begin(), [increment] (int &x) { return (x + std::rand()) % 255; });
  std::cout << background_data_[0] << " is the current background brightness" << std::endl;
  updateBackgroundDefault();
}

std::vector<pose::TranslationRotation3D> TrackSingleObject::getValidPointsAtPose()  {

  std::vector<pose::TranslationRotation3D> result;

  // Update the buffer of the optical flow twice to represent a static scene
  setDefaultPose();
  updateBackgroundDefault();
  setDefaultPose();
  updateBackgroundDefault();

  auto mask  = getDefaultObjectMask();
  auto depth = getDepth().clone();

  // get produced ar flow
  auto image = getFlow();
  // simplify flow
  cv::cvtColor(image,image, CV_BGRA2GRAY);

  // zero out points that are not on the object
  cv::Mat I(image.size(), image.type(), cv::Scalar(0));
  cv::bitwise_and(image,I,image, 1-mask);

  // pick out regions with texture i.e. valid flow
  I = image;
  int channels = I.channels();
  int nRows = I.rows;
  int nCols = I.cols * channels;
  if (I.isContinuous())
  {
    nCols *= nRows;
    nRows = 1;
  }
  int i,j;
  uchar* p;
  for( i = 0; i < nRows; ++i)
  {
    p = I.ptr<uchar>(i);
    for ( j = 0; j < nCols; ++j)
    {
      int col = j % I.cols;
      int row = j / I.cols;
      // probe for valid flow
      if (p[j] != 0 && !isnan(p[j]) && (p[j] != 255)) {

        float z = depth.at<float>(row,col);
        float x = float(col-cx_) / fx_ * z;
        float y = float(row-cy_) / fy_ * z;
        auto point = pose::TranslationRotation3D({x,y,z},{0,0,0});
        result.push_back(point);
      }
    }
  }

  return result;
}

void TrackSingleObject::updateBackgroundImage(cv::Mat new_background) {
  multi_rigid_tracker_ptr_->updatePoses(new_background);
}

void TrackSingleObject::updateBackgroundDefault() {
  cv::Mat new_background(image_height_, image_width_,
                         CV_8UC1, background_data_.data() );
  updateBackgroundImage(new_background);
}