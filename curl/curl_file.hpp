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
  CurlFile(const Poco::URI uri);
  ~CurlFile();

  const void download();
  const double get_file_size();

private:
  Poco::URI m_uri;
  CURL *m_curl;
  CURLcode m_res;

  static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
};

#endif // __CURL_FILE_H_INCLUDED__ 
