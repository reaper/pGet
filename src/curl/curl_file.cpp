#include "curl_file.hpp"

boost::mutex CurlFile::m_mutex;

/**
 * @brief CurlFile constructor
 * @param uri
 */
CurlFile::CurlFile(const boost::network::uri::uri uri) {
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
  struct CurlFile::File *out = (struct CurlFile::File *)stream;
  boost::mutex::scoped_lock lock(m_mutex);

  if(out && !out->data) {
    out->data = fopen(out->name, "wb");
    if(!out->data)
      return -1;
  }

  return fwrite(buffer, size, nmemb, out->data);
}

/**
 * @brief get URI
 * @return Poco::URI
 */
boost::network::uri::uri CurlFile::getURI() const {
  return m_uri;
}


/**
 * @brief download the file in threads
 * @param size
 */
void CurlFile::download(int splitSize) {
  int error;
  pthread_t tid[splitSize];

  const int fileSize = getFileSize();
  const int chunk = fileSize/splitSize;
  int currentChunk = chunk + 2;
  std::string fileName = searchFileName();

  CurlFile::ThreadArguments *arg1 = new CurlFile::ThreadArguments();
  arg1->classRef = this;
  arg1->fileName = fileName;
  arg1->chunk = "0-" + std::to_string(currentChunk);

  error = pthread_create(&tid[0], NULL, CurlFile::downloadChunk, arg1);
  if(0 != error)
    fprintf(stderr, "Couldn't run thread number %d, errno %d\n", 0, error);

  for(int i = 1; i < splitSize; i++) {
    const std::string range = std::to_string(currentChunk + 1) + "-" + std::to_string(currentChunk + chunk);

    CurlFile::ThreadArguments *arg = new CurlFile::ThreadArguments();
    arg->classRef = this;
    arg->fileName = fileName;
    arg->chunk = range;

    fprintf(stdout, "Launch tread:%d\n", i);
    error = pthread_create(&tid[i], NULL, CurlFile::downloadChunk, arg);

    if(0 != error) {
      fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
    }

    currentChunk = currentChunk + chunk;
  }

  // Waiting for threads end
  for(int i = 0; i < splitSize; i++) {
    error = pthread_join(tid[i], NULL);
    fprintf(stderr, "Thread %d terminated\n", i);
  }
}


/**
 * @brief Download the file
 */
void *CurlFile::downloadChunk(void *args) {
  CurlFile::ThreadArguments *fileArgs = (CurlFile::ThreadArguments*) args;

  fprintf(stdout, "Filename: %s, Chunk: %s\n", fileArgs->fileName.c_str(), fileArgs->chunk.c_str());

  CURL *curl;
  struct CurlFile::File file = {
    fileArgs->fileName.c_str(),
    NULL,
  };
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ((CurlFile *) fileArgs->classRef)->getURI().string().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileWrite);
    curl_easy_setopt(curl, CURLOPT_RANGE, fileArgs->chunk.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
 
  if(file.data)
    fclose(file.data);

  return NULL;
}

/**
 * @brief Get the file size
 * @return 
 */
double CurlFile::getFileSize() const {
  double fileSize = 0.0;
  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, m_uri.string().c_str());
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

  return 0.0;
}

/**
 * @brief search fo the file name in the URI path
 * @return std::string
 */
std::string CurlFile::searchFileName() const {
  std::string fileName = m_uri.path();

  boost::regex base_regex("^.*\\/(.*)$");
  boost::smatch matches;

  if(boost::regex_match(fileName, matches, base_regex)) {
    for (int i = 1; i < matches.size(); i++) {
      std::string match (matches[i].first, matches[i].second);
      return match;
    }
  }
  
  return NULL;
}
