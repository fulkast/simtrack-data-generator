#include "../include/scene_rendering/rendered_scene.h"
#include <hdf5_file.h>


RenderedScene::RenderedScene(){}

RenderedScene::RenderedScene(std::string hdf5_file_name) {

  bool success = true;

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

  setOBJName(obj_name);
  setWidth(in_file.readScalar<int>("width"));
  setHeight(in_file.readScalar<int>("height"));

  cv::Mat camera_matrix;
  {
    std::vector<int> size;
    std::vector<double> data;
    in_file.readArray("camera_matrix", data, size);
    if ((size.at(0) != 3) || (size.at(1) != 4))
      throw std::runtime_error("Expecting 4x3 camera_matrix");
    camera_matrix = cv::Mat(3, 4, CV_64FC1, data.data()).clone();
  }
  setCameraMatrix(camera_matrix);

  // set camera parameters
  fx_ = camera_matrix.at<double>(0, 0);
  fy_ = camera_matrix.at<double>(1, 1);
  cx_ = camera_matrix.at<double>(0, 2);
  cy_ = camera_matrix.at<double>(1, 2);

  near_plane_ = .001f;
  far_plane_ = 10.0f;
  zoom_x_ = 1;
  zoom_y_ = 1;

  // set the projection matrix
  projection_matrix_[0][0] = 2.0 * fx_ / (double)width_ * zoom_x_;
  projection_matrix_[1][1] = 2.0 * fy_ / (double)height_ * zoom_y_;
  projection_matrix_[0][2] = 2.0 * (0.5 - cx_ / (double)width_) * zoom_x_;
  projection_matrix_[1][2] = 2.0 * (cy_ / (double)height_ - 0.5) * zoom_y_;
  projection_matrix_[2][2] =
    -(far_plane_ + near_plane_) / (far_plane_ - near_plane_);
  projection_matrix_[2][3] =
    -2.0 * far_plane_ * near_plane_ / (far_plane_ - near_plane_);
  projection_matrix_[3][2] = -1;

  // create the object
  int segment_ind = 1;
  std::string model_resource = "file://" + getOBJName();

  rigid_object_ptr_ =
    std::unique_ptr<render::RigidObject>{ new render::RigidObject(
      model_resource, ogre_context_.scene_manager_, segment_ind) };
  rigid_object_ptr_->setVisible(true);
  {
    // Set hard coded object pose
    double T[3] = {0, 0, 0.0};
    double R[3] = {0, 0, 0};
    pose::TranslationRotation3D pose;
    pose.setT(T);
    pose.setR(R);
    rigid_object_ptr_->setPose(pose.ogreTranslation(),
                          pose.ogreRotation());
  }

  // set camera pose
  Ogre::Vector3 camera_position;
  Ogre::Quaternion camera_orientation;
  {
    // Set hard coded object pose
    double T[3] = {0, 0, -1.0};
    double R[3] = {0, 0, 0};
    pose::TranslationRotation3D pose;
    pose.setT(T);
    pose.setR(R);
    camera_position = pose.ogreTranslation();
    camera_orientation = pose.ogreRotation();
    // convert vision (Z-forward) frame to ogre frame (Z-out)
    camera_orientation = camera_orientation *
      Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_X);

  }

  if (!success) return;

  ogre_multi_render_target_ptr_ = std::unique_ptr<render::OgreMultiRenderTarget> {
      new render::OgreMultiRenderTarget("scene", width_,
                                        height_, ogre_context_.scene_manager_) };

  ogre_multi_render_target_ptr_->updateCamera(camera_position, camera_orientation,
                                        projection_matrix_);
  ogre_multi_render_target_ptr_->render();

}

void RenderedScene::setOBJName(std::string obj_name) {
  obj_name_ = obj_name;
}

std::string RenderedScene::getOBJName() const {
  return obj_name_;
}

void RenderedScene::setCameraMatrix(const cv::Mat matrix) {
  camera_matrix_ = matrix.clone();
}

cv::Mat RenderedScene::getCameraMatrix() const {
  return camera_matrix_;
}

void RenderedScene::setWidth(int width) {
  width_ = width;
}

int RenderedScene::getWidth() const {
  return width_;
}

void RenderedScene::setHeight(int height) {
  height_ = height;
}

int RenderedScene::getHeight() const {
  return height_;
}

Ogre::Matrix4 RenderedScene::getProjectionMatrix() const {
  return projection_matrix_;
}

cv::Mat RenderedScene::getRenderedTexture() const {
  std::vector<cudaArray **> cuda_arrays;
  int n_arrays = 6;
  for (int i = 0; i < n_arrays; i++)
    cuda_arrays.push_back(new cudaArray *);

  std::vector<int> out_size{ height_, width_ };
  std::vector<float> h_out(height_ * width_);

  ogre_multi_render_target_ptr_->mapCudaArrays(cuda_arrays);

  cv::Mat texture = cv::Mat::zeros(height_, width_, CV_8UC4);

  cudaMemcpyFromArray(texture.data, *cuda_arrays.at(5), 0, 0,
                      width_ * height_ * sizeof(float), cudaMemcpyDeviceToHost);

  ogre_multi_render_target_ptr_->unmapCudaArrays();
  return texture;
}

cv::Mat RenderedScene::getSegmentIndexImage() const {
  std::vector<cudaArray **> cuda_arrays;
  int n_arrays = 6;
  for (int i = 0; i < n_arrays; i++)
    cuda_arrays.push_back(new cudaArray *);

  std::vector<int> out_size{ height_, width_ };
  std::vector<float> h_out(height_ * width_);

  ogre_multi_render_target_ptr_->mapCudaArrays(cuda_arrays);

  cv::Mat texture = cv::Mat::zeros(height_, width_, CV_8UC4);

  cudaMemcpyFromArray(texture.data, *cuda_arrays.at(4), 0, 0,
                      width_ * height_ * sizeof(float), cudaMemcpyDeviceToHost);

  ogre_multi_render_target_ptr_->unmapCudaArrays();
  return texture;
}

std::vector<cv::Point2d> RenderedScene::getValidFlowPoints() const {

}