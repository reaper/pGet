#include <stdio.h>
#include <iostream>
#include <string>
#include <Poco/URI.h>
#include <curl/curl.h>

#include "curl/curl_file.hpp"
 
int main(void)
{
  Poco::URI uri("ftp://ftp.gwdg.de/pub/misc/mysql/Downloads/MySQL-5.6/MySQL-5.6.13-1.el6.src.rpm");
  fprintf(stdout, "\nScheme: %s\n", uri.getScheme().c_str());
  fprintf(stdout, "Host: %s\n", uri.getHost().c_str());
  fprintf(stdout, "Port: %d\n", uri.getPort());
  fprintf(stdout, "Path: %s\n\n", uri.getPathEtc().c_str());

  CurlFile *curlFile = new CurlFile(uri);
  printf("filesize: %0.0f bytes\n", curlFile->get_file_size());
  // curlFile->download();

  delete curlFile;
  return 0;
}
