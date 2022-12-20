#include <curl/curl.h>
#include <iostream>

int main(int argc, char *argv[]) {
  CURL *curl = curl_easy_init();
  CURLcode res;

  if (curl == nullptr) {
    std::cerr << "curl init failed!\n";
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:3000");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  /* Perform the request, res will get the return code */
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res)
              << '\n';
  } else {
    std::cout << "curl_easy_perform() success\n";
  }
  curl_easy_cleanup(curl);
  return 0;
}
