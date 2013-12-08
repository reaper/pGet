#ifndef __PGET_FILE_H_INCLUDED__
#define __PGET_FILE_H_INCLUDED__

#include <curl/curl.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <boost/regex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/network/uri.hpp>

class PgetFile {

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

  PgetFile(const boost::network::uri::uri uri);
  ~PgetFile();
  static boost::mutex m_mutex;

  void download(const int splitSize);
  boost::network::uri::uri getURI() const;

private:
  boost::network::uri::uri m_uri;
  std::string searchFileName() const;
  double getFileSize() const;
  static size_t fileWrite(void *buffer, size_t size, size_t nmemb, void *stream);
  static void *downloadChunk(void *args);

};

#endif // __PGET_FILE_H_INCLUDED__ 
