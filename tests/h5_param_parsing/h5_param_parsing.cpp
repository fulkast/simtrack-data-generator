#include "gtest/gtest.h"
#include <cmath>
#include <string>
#include "scene_rendering/rendered_scene.h"


class H5ParamLoadingTest : public ::testing::Test {
public:
  H5ParamLoadingTest() :
      h5filename{"/home/seasponge/Workspace/"
                     "random-trees-with-simtrack/"
                     "simtrack-data-generator/"
                     "tests/test_data/obj_file_name.h5"},
      myscene_(h5filename) {}

protected:
  std::string h5filename;
  RenderedScene myscene_;

};

TEST_F(H5ParamLoadingTest, FileNameCorrect)
{
  EXPECT_EQ("pringles", myscene_.getOBJName());
}

TEST_F(H5ParamLoadingTest, WidthCorrect)
{
  EXPECT_EQ(320, myscene_.getWidth());
}

TEST_F(H5ParamLoadingTest, HeightCorrect)
{
  EXPECT_EQ(240, myscene_.getHeight());
}

TEST_F(H5ParamLoadingTest, CameraMatrixCorrect)
{
  std::vector<double> data{ 262.5,    0., 159.75, 0.,
                               0., 262.5, 119.75, 0.,
                               0.,   0. ,     1., 0.};

  cv::Mat xor_result;
  cv::Mat correct_matrix(3,4, CV_64FC1, data.data());
  cv::Mat test_matrix = myscene_.getCameraMatrix();
  cv::bitwise_xor(correct_matrix, test_matrix, xor_result);
  EXPECT_EQ(0, cv::countNonZero(xor_result));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
