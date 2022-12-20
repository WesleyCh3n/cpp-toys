#include <cppcodec/base64_rfc4648.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: hello_world <image path>\n";
    return -1;
  }
  cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);

  std::vector<unsigned char> out;
  cv::imencode(".png", img, out);
  std::cout << "encode len: " << out.size() << '\n';
  std::basic_string<char> img_encode = cppcodec::base64_rfc4648::encode(out);
  std::cout << "encode len: " << img_encode.length() << '\n';

  std::vector<unsigned char> in = cppcodec::base64_rfc4648::decode(img_encode);
  cv::Mat img_ = cv::imdecode(in, -1);
  cv::imshow("Decode img", img_);
  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
