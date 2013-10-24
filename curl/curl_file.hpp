//=================================
// include guard
#ifndef __CURL_FILE_H_INCLUDED__
#define __CURL_FILE_H_INCLUDED__

//=================================
// included dependencies
#include <Poco/URI.h>
#include <curl/curl.h>
#include <string>
#include <iostream>

//=================================
// the actual class
class CurlFile
{
public:
  struct File {
    const char *name;
    FILE *data;
  };

  CurlFile(const Poco::URI uri);
  ~CurlFile();

  const void download();
  const double get_file_size();

private:
  Poco::URI m_uri;
  static size_t fileWrite(void *buffer, size_t size, size_t nmemb, void *stream);
  static size_t throwAway(void *ptr, size_t size, size_t nmemb, void *data);
};

#endif // __CURL_FILE_H_INCLUDED__ 
