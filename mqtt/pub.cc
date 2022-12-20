#include <iostream>
#include <mqtt/client.h>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: pub <ip> <user> <password>\n";
    return 1;
  }
  mqtt::client client("tcp://" + std::string(argv[1]) + ":1883", "test_client");
  mqtt::connect_options connOpts;
  connOpts.set_keep_alive_interval(20);
  connOpts.set_clean_session(true);
  connOpts.set_user_name(argv[2]);
  connOpts.set_password(argv[3]);

  try {
    client.connect(connOpts);
    std::string TOPIC = "hello";
    std::string PAYLOAD = "Hello World!";
    auto msg = mqtt::make_message(TOPIC, PAYLOAD);
    msg->set_qos(1);
    std::cout << "...start publish\n";
    client.publish(msg);
    client.disconnect();
    std::cout << "...finish\n";
  } catch (const mqtt::exception &exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }
  return 0;
}
