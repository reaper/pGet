#include "curl/curl_file.hpp"
 
int main(void)
{
  boost::network::uri::uri fileUri("ftp://ftp.gwdg.de/pub/misc/mysql/Downloads/MySQL-5.5/mysql-5.5.33-linux2.6-i686.tar.gz");
  fprintf(stdout, "\nScheme: %s\n", fileUri.scheme().c_str());
  fprintf(stdout, "Host: %s\n", fileUri.host().c_str());
  fprintf(stdout, "Port: %s\n", fileUri.port().c_str());
  fprintf(stdout, "Path: %s\n\n", fileUri.path().c_str());

  CurlFile *curlFile = new CurlFile(fileUri);
  // printf("filesize: %0.0f bytes\n", curlFile->get_file_size());

  int size = 5;
  curlFile->download(size);
  delete curlFile;
  return 0;
}
