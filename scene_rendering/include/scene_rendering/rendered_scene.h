#pragma once

#include <string>

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

class RenderedScene {
public:
  RenderedScene();

  RenderedScene(std::string hdf5_file_name);

  void setOBJName(std::string obj_name);

  std::string getOBJName() const;

  void setCameraMatrix(const cv::Mat matrix);

  cv::Mat getCameraMatrix() const;

  void setWidth(int width);

  int getWidth() const ;

  void setHeight(int height);

  int getHeight() const;

private:
  std::string obj_name_;
  cv::Mat camera_matrix_;
  int width_, height_;

};
