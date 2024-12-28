#ifndef HTTP_MODULE
#define HTTP_MODULE

#include <WiFi.h>
#include <HTTPClient.h>

class HTTP
{
  // Private member variable to store the HTTPClient object
  private:
    HTTPClient http;

  // Public method to send a POST request to a given URL with the provided data
  public:
    bool post(const char* url, const char* data) {
      http.begin(url);
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(data);
      http.end();
      return httpCode == 200;
    }
};
#endif