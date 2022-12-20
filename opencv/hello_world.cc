#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: hello_world <image path>\n";
    return -1;
  }
  cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);

  cv::imshow("Hellow World", img);
  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
