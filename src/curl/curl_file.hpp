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

  struct Args {
    void *classRef;
    std::string fileName;
    std::string chunk;
  };

  CurlFile(const Poco::URI uri);
  ~CurlFile();

  const void downloadChunk(const std::string& fileName, const std::string& chunk);
  const double get_file_size();
  static void *runChunkDownload(void *args) {
    CurlFile::Args *arg = (CurlFile::Args*) args;
    ((CurlFile *) arg->classRef)->downloadChunk(arg->fileName, arg->chunk);
    return NULL;
  }

private:
  Poco::URI m_uri;
  static size_t fileWrite(void *buffer, size_t size, size_t nmemb, void *stream);
};

#endif // __CURL_FILE_H_INCLUDED__ 
