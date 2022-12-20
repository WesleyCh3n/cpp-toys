#include <curl/curl.h>
#include <iostream>

class Timer {
public:
  Timer() { stTimePt_ = std::chrono::high_resolution_clock::now(); }
  ~Timer() { Stop(); }
  void Stop() {
    auto endTimePt = std::chrono::high_resolution_clock::now();
    auto start =
        std::chrono::time_point_cast<std::chrono::microseconds>(stTimePt_)
            .time_since_epoch()
            .count();
    auto end =
        std::chrono::time_point_cast<std::chrono::microseconds>(endTimePt)
            .time_since_epoch()
            .count();
    auto duration = end - start;
    std::cout << "take " << duration << "us (" << duration * 0.001 << "ms)\n";
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> stTimePt_;
};

int main(int argc, char *argv[]) {
  CURL *curl;

  CURLM *multi_handle;
  int still_running = 0;

  curl_mime *form = nullptr;
  curl_mimepart *field = nullptr;
  struct curl_slist *headerlist = nullptr;

  curl = curl_easy_init();
  multi_handle = curl_multi_init();

  if (!curl || !multi_handle) {
    std::cerr << "Error from init\n";
    return -1;
  }
  /* Create the form */
  form = curl_mime_init(curl);

  /* Fill in the file upload field */
  field = curl_mime_addpart(form);
  curl_mime_name(field, "sendfile");
  curl_mime_filedata(field, argv[1]);

  /* Fill in the filename field */
  field = curl_mime_addpart(form);
  curl_mime_name(field, "filename");
  curl_mime_data(field, argv[1], CURL_ZERO_TERMINATED);

  /* Fill in the submit field too, even if this is rarely needed */
  field = curl_mime_addpart(form);
  curl_mime_name(field, "submit");
  curl_mime_data(field, "send", CURL_ZERO_TERMINATED);

  /* initialize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append(headerlist, "Expect: ");

  /* what URL that receives this POST */
  curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:3000/upload");
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
  curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

  curl_multi_add_handle(multi_handle, curl);

  {
    Timer t;
    std::cout << "start send\n";
    do {
      CURLMcode mc = curl_multi_perform(multi_handle, &still_running);

      if (still_running) {
        std::cout << "still runing\n";
        /* wait for activity, timeout or "nothing" */
        mc = curl_multi_poll(multi_handle, nullptr, 0, 1000, nullptr);
      }

      if (mc)
        break;
    } while (still_running);
    std::cout << "finish send\n";
  }

  curl_multi_cleanup(multi_handle);
  /* always cleanup */
  curl_easy_cleanup(curl);
  /* then cleanup the form */
  curl_mime_free(form);
  /* free slist */
  curl_slist_free_all(headerlist);

  return 0;
}
