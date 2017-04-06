#include "scene_rendering/rendered_scene.h"
#include <hdf5_file.h>

#include <iostream>


RenderedScene::RenderedScene(){}

RenderedScene::RenderedScene(std::string hdf5_file_name) {

  std::cout << "reading " << hdf5_file_name << std::endl;
  util::HDF5File in_file(hdf5_file_name);
  if (!in_file.checkVariableExists("obj_file_name"))
    std::cerr << "File name doesn't exist in " << hdf5_file_name << std::endl;

  std::string obj_name = in_file.readScalar<std::string>("obj_file_name");
  std::cout << obj_name << " is the object anem" << std::endl;
  setOBJName(obj_name);

}

void RenderedScene::setOBJName(std::string obj_name) {
  obj_name_ = obj_name;
}

std::string RenderedScene::getOBJName() const {
  return obj_name_;
}
