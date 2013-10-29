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
#include <pthread.h>
#include <boost/regex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

//=================================
// the actual class
class CurlFile {

public:
  struct File {
    const char *name;
    FILE *data;
  };

  struct ThreadArguments {
    void *classRef;
    std::string fileName;
    std::string chunk;
  };

  CurlFile(const Poco::URI uri);
  ~CurlFile();
  static boost::mutex m_mutex;

  void download(int splitSize);
  Poco::URI getURI() const;

private:
  Poco::URI m_uri;
  std::string searchFileName() const;
  double getFileSize() const;
  static size_t fileWrite(void *buffer, size_t size, size_t nmemb, void *stream);
  static void *downloadChunk(void *args);

};

#endif // __CURL_FILE_H_INCLUDED__ 
