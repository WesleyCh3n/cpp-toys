#include "opencv2/imgcodecs.hpp"
#include <chrono>
#include <iostream>
#include <vector>

#include <cppcodec/base64_rfc4648.hpp>
#include <mqtt/client.h>
#include <opencv2/opencv.hpp>

bool decode_image(std::string payload) {
  std::vector<unsigned char> in = cppcodec::base64_rfc4648::decode(payload);
  cv::Mat img = cv::imdecode(in, -1);
  cv::imwrite("get_image.png", img);
  return true;
}

bool mqtt_sub(std::string ip, std::string user, std::string passwd,
              std::string topic) {
  mqtt::client client("tcp://" + ip + ":1883", "test_client_sub");
  auto connOpts = mqtt::connect_options_builder()
                      .automatic_reconnect(std::chrono::seconds(2),
                                           std::chrono::seconds(30))
                      .clean_session(false)
                      .user_name(user)
                      .password(passwd)
                      .finalize();

  try {
    std::cout << "Connecting to the MQTT server..." << std::flush;
    mqtt::connect_response rsp = client.connect(connOpts);
    std::cout << "OK\n";

    if (!rsp.is_session_present()) {
      std::cout << "Subscribing to topics..." << std::flush;

      mqtt::subscribe_options subOpts;
      mqtt::properties props1{
          {mqtt::property::SUBSCRIPTION_IDENTIFIER, 1},
      };
      client.subscribe(topic, 1, subOpts, props1);

      std::cout << "OK" << std::endl;
    } else {
      std::cout << "Session already present. Skipping subscribe." << std::endl;
    }

    while (true) {
      auto msg = client.consume_message();
      if (msg) {
        // std::cout << msg->to_string() << '\n';
        if (decode_image(msg->to_string())) {
          std::cout << "got image\n";
        }
      }
    }
    std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
    client.disconnect();
    std::cout << "OK\n";

  } catch (const mqtt::exception &exc) {
    std::cerr << exc.what() << '\n';
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: mqtt_sub <ip> <user> <password>\n";
    return 1;
  }
  mqtt_sub(argv[1], argv[2], argv[3], "image");
  return 0;
}
