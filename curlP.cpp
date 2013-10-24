#include <stdio.h>
#include <iostream>
#include <string>
#include <Poco/URI.h>
#include <curl/curl.h>
 
struct FtpFile {
  const char *filename;
  FILE *stream;
};
 
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && !out->stream) {
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1;
  }
  return fwrite(buffer, size, nmemb, out->stream);
}

/**
 * @brief Download a file
 * @param url
 */
static void download_file(const std::string url) {
  CURL *curl;
  CURLcode res;
  struct FtpFile ftpfile={
    "curl.tar.gz",
    NULL
  };
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    res = curl_easy_perform(curl);
 
    curl_easy_cleanup(curl);
 
    if(CURLE_OK != res) {
      fprintf(stderr, "curl told us %d\n", res);
    }
  }
 
  if(ftpfile.stream)
    fclose(ftpfile.stream);
 
  curl_global_cleanup();
}

/**
 * @brief Get a file size
 * @param url
 * @return double
 */
static double get_file_size(const std::string url) {
  double fileSize = 0.0;
  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    res = curl_easy_perform(curl);

    if(CURLE_OK == res) {
      res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
      if((CURLE_OK == res) && (fileSize > 0.0))
        return fileSize;
    }
  }
 
  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return 0.0;
}
 
int main(void)
{
  Poco::URI uri("ftp://ftp.gwdg.de/pub/misc/mysql/Downloads/MySQL-5.6/MySQL-5.6.13-1.el6.src.rpm");
  fprintf(stdout, "\nScheme: %s\n", uri.getScheme().c_str());
  fprintf(stdout, "Host: %s\n", uri.getHost().c_str());
  fprintf(stdout, "Port: %d\n", uri.getPort());
  fprintf(stdout, "Path: %s\n\n", uri.getPathEtc().c_str());

  printf("filesize: %0.0f bytes\n", get_file_size(uri.toString()));
  download_file(uri.toString());
  return 0;
}
