#include "curl/curl_file.hpp"
 
int main(void)
{
  Poco::URI uri("ftp://ftp.gwdg.de/pub/misc/mysql/Downloads/MySQL-5.5/mysql-5.5.33-linux2.6-i686.tar.gz");
  fprintf(stdout, "\nScheme: %s\n", uri.getScheme().c_str());
  fprintf(stdout, "Host: %s\n", uri.getHost().c_str());
  fprintf(stdout, "Port: %d\n", uri.getPort());
  fprintf(stdout, "Path: %s\n\n", uri.getPathEtc().c_str());

  CurlFile *curlFile = new CurlFile(uri);
  // printf("filesize: %0.0f bytes\n", curlFile->get_file_size());

  int size = 5;
  curlFile->download(size);
  delete curlFile;
  return 0;
}
