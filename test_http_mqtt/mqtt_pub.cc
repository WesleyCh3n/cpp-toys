#include <iostream>
#include <vector>

#include <cppcodec/base64_rfc4648.hpp>
#include <mqtt/client.h>
#include <opencv2/opencv.hpp>

#include "../Timer.h"

std::string encode_image(char *path) {
  cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
  std::vector<unsigned char> out;
  cv::imencode(".png", img, out);
  std::basic_string<char> encoded = cppcodec::base64_rfc4648::encode(out);
  return encoded;
}

bool mqtt_pub(std::string ip, std::string user, std::string passwd,
              std::string topic, std::string payload) {
  mqtt::client client("tcp://" + ip + ":1883", "test_client");
  mqtt::connect_options connOpts;
  connOpts.set_keep_alive_interval(20);
  connOpts.set_clean_session(true);
  connOpts.set_user_name(user);
  connOpts.set_password(passwd);

  try {
    client.connect(connOpts);
    auto msg = mqtt::make_message(topic, payload);
    msg->set_qos(1);
    std::cout << "...start publish\n";
    {
      Timer t;
      client.publish(msg);
    }
    client.disconnect();
    std::cout << "...finish\n";
  } catch (const mqtt::exception &exc) {
    std::cerr << exc.what() << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 5) {
    std::cerr << "Usage: mqtt_pub <image path> <ip> <user> <password>\n";
    return 1;
  }
  auto payload = encode_image(argv[1]);
  mqtt_pub(argv[2], argv[3], argv[4], "image", payload);
  return 0;
}
