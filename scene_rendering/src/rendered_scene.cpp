#include "scene_rendering/rendered_scene.h"
#include <hdf5_file.h>


RenderedScene::RenderedScene(){}

RenderedScene::RenderedScene(std::string hdf5_file_name) {

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
  return camera_matrix_.clone();
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