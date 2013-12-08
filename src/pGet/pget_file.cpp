#include "pget_file.hpp"

boost::mutex PgetFile::m_mutex;

/**
 * @brief PgetFile constructor
 * @param uri
 */
PgetFile::PgetFile(const Uri& uri) {
  m_uri = uri;
  curl_global_init(CURL_GLOBAL_ALL);
}


/**
 * @brief PgetFile destructor
 */
PgetFile::~PgetFile() {
  curl_global_cleanup();
}


/**
 * @brief Write the file
 * @param buffer
 * @param size
 * @param nmemb
 * @param stream
 * @return 
 */
size_t PgetFile::write(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct PgetFile::File *out = (struct PgetFile::File *)stream;
  boost::mutex::scoped_lock lock(m_mutex);

  if(out && !out->data) {
    out->data = fopen(out->name, "wb");
    if(!out->data)
      return -1;
  }

  return fwrite(buffer, size, nmemb, out->data);
}

/**
 * @brief Get the uri
 * @return 
 */
Uri PgetFile::uri() const {
  return m_uri;
}


/**
 * @brief download the file in threads
 * @param size
 */
void PgetFile::download(const int split_size) {
  int error;
  pthread_t tid[split_size];

  const int size = file_size();
  const int chunk = size/split_size;
  int currentChunk = chunk + 2;

  // First thread args
  PgetFile::ThreadArguments *arg1 = new PgetFile::ThreadArguments();
  arg1->classRef = this;
  arg1->fileName = m_uri.filename();
  arg1->chunk = "0-" + std::to_string(currentChunk);

  std::cout << "Launch tread: 0" << std::endl;
  error = pthread_create(&tid[0], NULL, PgetFile::download_chunk, arg1);
  if(0 != error)
    std::cout << "Couldn't run thread number 0 errno : " << error << std::endl;

  // Run last threads
  for(int i = 1; i < split_size; i++) {
    const std::string range = std::to_string(currentChunk + 1) + "-" + std::to_string(currentChunk + chunk);

    PgetFile::ThreadArguments *arg = new PgetFile::ThreadArguments();
    arg->classRef = this;
    arg->fileName = m_uri.filename();
    arg->chunk = range;

    std::cout << "Launch tread: " << i << std::endl;
    error = pthread_create(&tid[i], NULL, PgetFile::download_chunk, arg);

    if(0 != error) {
      std::cout << "Couldn't run thread number " << i << " errno : " << error << std::endl;
    }

    currentChunk = currentChunk + chunk;
  }

  // Waiting for threads end
  for(int i = 0; i < split_size; i++) {
    error = pthread_join(tid[i], NULL);
    std::cout << "Thread " << i << " terminated" << std::endl;
  }
}


/**
 * @brief Download the file
 */
void *PgetFile::download_chunk(void *args) {
  PgetFile::ThreadArguments *fileArgs = (PgetFile::ThreadArguments*) args;
  std::cout << "Filename: " << fileArgs->fileName.c_str() << ", Chunk: " << fileArgs->chunk.c_str() << std::endl;

  CURL *curl;
  struct PgetFile::File file = {
    fileArgs->fileName.c_str(),
    NULL,
  };
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ((PgetFile *) fileArgs->classRef)->uri().url().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
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
double PgetFile::file_size() const {
  double fileSize { 0.0 };
  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, m_uri.url().c_str());
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

  return fileSize;
}
