#pragma once

#include <string>

class RenderedScene {
public:
  RenderedScene();

  RenderedScene(std::string hdf5_file_name);

  void setOBJName(std::string obj_name);

  std::string getOBJName() const;

private:
  std::string obj_name_;

};
