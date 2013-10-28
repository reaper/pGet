#include <pthread.h>
#include "curl/curl_file.hpp"
 
int main(void)
{
  Poco::URI uri("http://ftp.gwdg.de/pub/misc/mysql/Downloads/MySQL-5.5/mysql-5.5.33-linux2.6-i686.tar.gz");
  fprintf(stdout, "\nScheme: %s\n", uri.getScheme().c_str());
  fprintf(stdout, "Host: %s\n", uri.getHost().c_str());
  fprintf(stdout, "Port: %d\n", uri.getPort());
  fprintf(stdout, "Path: %s\n\n", uri.getPathEtc().c_str());

  CurlFile *curlFile = new CurlFile(uri);
  // printf("filesize: %0.0f bytes\n", curlFile->get_file_size());

  int size = 5;
  int error;
  pthread_t tid[size];

  const int chunk = curlFile->get_file_size()/size;
  int tmpChunk = chunk + 2;

  struct CurlFile::Args args = {
    &curlFile,
    "0",
    "0-" + std::to_string(tmpChunk)
  };

  error = pthread_create(&tid[0], NULL, CurlFile::runChunkDownload, (void *)&args);
  if(0 != error)
    fprintf(stderr, "Couldn't run thread number %d, errno %d\n", 0, error);

  for(int i = 1; i < size; i++) {
    const std::string range = std::to_string(tmpChunk + 1) + "-" + std::to_string(tmpChunk + chunk);

    struct CurlFile::Args args = {
      &curlFile,
      std::to_string(i),
      range
    };

    error = pthread_create(&tid[i], NULL, CurlFile::runChunkDownload, (void *)&args);

    if(0 != error) {
      fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
    } else {
      fprintf(stderr, "Thread %d finished!\n", i);
    }

    tmpChunk = tmpChunk + chunk;
  }

  for(int i=0; i < size; i++) {
    error = pthread_join(tid[i], NULL);
    fprintf(stderr, "Thread %d terminated\n", i);
  }

  delete curlFile;
  return 0;
}
