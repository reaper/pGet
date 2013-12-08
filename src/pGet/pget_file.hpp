#ifndef __PGET_FILE_H_INCLUDED__
#define __PGET_FILE_H_INCLUDED__

#include <curl/curl.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <boost/regex.hpp>
#include <boost/thread/mutex.hpp>

#include "uri.hpp"

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

    PgetFile(const Uri& uri);
    ~PgetFile();
    static boost::mutex m_mutex;

    double file_size() const;
    void download(const int splitSize);
    Uri uri() const;

  private:
    Uri m_uri;
    static size_t write(void *buffer, size_t size, size_t nmemb, void *stream);
    static void *download_chunk(void *args);
};

#endif // __PGET_FILE_H_INCLUDED__ 
