#include "pGet/pget_file.hpp"
 
int main(void)
{
  boost::network::uri::uri fileUri("ftp://ftp.gwdg.de/pub/misc/mysql/Downloads/MySQL-5.5/mysql-5.5.33-linux2.6-i686.tar.gz");
  std::cout << "\nScheme: " << fileUri.scheme().c_str() << std::endl;
  std::cout << "Host: " << fileUri.host().c_str() << std::endl;
  std::cout << "Port: " << fileUri.port().c_str() << std::endl;
  std::cout << "Path: " << fileUri.path().c_str() << std::endl;

  PgetFile *pgetFile = new PgetFile(fileUri);
  // std::cout << "filesize: " << pgetFile->get_file_size() << std::endl;

  int size { 5 };
  pgetFile->download(size);
  delete pgetFile;
  return EXIT_SUCCESS;
}
