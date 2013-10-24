#include "curl_file.hpp"

CurlFile::CurlFile(const Poco::URI uri) {
  m_uri = uri;
}

CurlFile::~CurlFile() {
}


struct FtpFile {
  const char *filename;
  FILE *stream;
};

size_t CurlFile::my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && !out->stream) {
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1;
  }

  return my_fwrite(buffer, size, nmemb, out->stream);
}



const void CurlFile::download() {
  struct FtpFile ftpfile={
    "curl.tar.gz",
    NULL
  };
  curl_global_init(CURL_GLOBAL_DEFAULT);
  m_curl = curl_easy_init();

  if(m_curl) {
    curl_easy_setopt(m_curl, CURLOPT_URL, m_uri.toString().c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &ftpfile);
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);

    m_res = curl_easy_perform(m_curl);

    curl_easy_cleanup(m_curl);

    if(CURLE_OK != m_res) {
      fprintf(stderr, "curl told us %d\n", m_res);
    }
  }

  

  if(ftpfile.stream)
    fclose(ftpfile.stream);

  curl_global_cleanup();
}

const double CurlFile::get_file_size() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  m_curl = curl_easy_init();
  double fileSize = 0.0;
 
  if(m_curl) {
    curl_easy_setopt(m_curl, CURLOPT_URL, m_uri.toString().c_str());
    curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1);
    m_res = curl_easy_perform(m_curl);

    if(CURLE_OK == m_res) {
      m_res = curl_easy_getinfo(m_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
      if((CURLE_OK == m_res) && (fileSize > 0.0))
        return fileSize;
    }
  }

  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
 
  return 0.0;
}
