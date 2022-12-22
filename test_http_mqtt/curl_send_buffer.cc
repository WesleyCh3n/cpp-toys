#include <iostream>

#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char *argv[]) {
  cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
  std::vector<uchar> vec_img;
  cv::imencode(".png", img, vec_img);
  std::cout << vec_img.size() << '\n';

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Error from init\n";
    return 1;
  }

  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

  curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:3000/upload");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,
                   img.total() * img.elemSize());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, img.data);

  // curl_mime *form = curl_mime_init(curl);
  // curl_mimepart *field = curl_mime_addpart(form);
  //
  // curl_mime_name(field, "filename");
  // curl_mime_data(field, img.data, img.total() * img.elemSize());

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return 0;
}
