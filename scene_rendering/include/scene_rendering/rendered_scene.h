#pragma once

#include <string>
#include <memory>

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <ogre_context.h>
#include <ogre_multi_render_target.h>
#include <multiple_rigid_models_ogre.h>


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

  Ogre::Matrix4 getProjectionMatrix() const;

private:
  std::string obj_name_;
  cv::Mat camera_matrix_;
  int width_, height_;

  double fx_;
  double fy_;
  double cx_;
  double cy_;
  float near_plane_;
  float far_plane_;
  float zoom_x_;
  float zoom_y_;
  Ogre::Matrix4 projection_matrix_;
  render::OgreContext ogre_context_;
  std::unique_ptr<render::RigidObject> rigid_object_;
};
