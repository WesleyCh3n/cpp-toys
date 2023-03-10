#include <curl/curl.h>
#include <iostream>

int main() {
  CURL *curl;
  CURLcode res;

  static const char *postthis = "moo mooo moo moo";

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:3000");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);

    /* if we do not provide POSTFIELDSIZE, libcurl will strlen() by
       itself */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}
