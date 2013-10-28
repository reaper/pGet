#include "curl_file.hpp"

/**
 * @brief CurlFile constructor
 * @param uri
 */
CurlFile::CurlFile(const Poco::URI uri) {
  m_uri = uri;
  curl_global_init(CURL_GLOBAL_ALL);
}

/**
 * @brief CurlFile destructor
 */
CurlFile::~CurlFile() {
  curl_global_cleanup();
}

/**
 * @brief fileWrite
 * @param buffer
 * @param size
 * @param nmemb
 * @param stream
 * @return 
 */
size_t CurlFile::fileWrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct CurlFile::File *out=(struct CurlFile::File *)stream;
  if(out && !out->data) {
    out->data=fopen(out->name, "wb");
    if(!out->data)
      return -1;
  }
  return fwrite(buffer, size, nmemb, out->data);
}

/**
 * @brief Download the file
 */
const void CurlFile::downloadChunk(const std::string& fileName, const std::string& chunk) {
  fprintf(stdout, "Filename: %s, Chunk: %s\n", fileName.c_str(), chunk.c_str());
  CURL *curl;
  CURLcode res;
  struct CurlFile::File file = {
    fileName.c_str(),
    NULL
  };
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, m_uri.toString().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileWrite);
    curl_easy_setopt(curl, CURLOPT_RANGE, chunk.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
 
  if(file.data)
    fclose(file.data);
}

/**
 * @brief Get the file size
 * @return 
 */
const double CurlFile::get_file_size() {
  double fileSize = 0.0;
  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, m_uri.toString().c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
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
