#include "pGet/pget_file.hpp"
 
int main(void)
{
  Uri uri("http://cdimage.debian.org/debian-cd/7.2.0/amd64/iso-cd/debian-7.2.0-amd64-CD-1.iso");
  std::cout << "\nScheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;
  std::cout << "Path: " << uri.path() << std::endl;

  PgetFile *pgetFile = new PgetFile(uri);
  std::cout << "filesize: " << pgetFile->file_size() << std::endl;

  // int size { 5 };
  // pgetFile->download(size);
  delete pgetFile;
  return EXIT_SUCCESS;
}
