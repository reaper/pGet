#ifndef __URI_H_INCLUDED__
#define __URI_H_INCLUDED__

#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#define REGEX "^(.*):\\/\\/(.*?):?(.*?)@?(.*?):?(\\d*?)(\\/.*\\/(.*))$"

class Uri {
  public:
    Uri(const std::string& url);
    Uri() : m_port(0) {};
    ~Uri(){};

    inline std::string url() const { return m_url; };
    inline std::string username() const { return m_username; };
    inline std::string password() const { return m_password; };
    inline std::string scheme() const { return m_scheme; };
    inline std::string host() const { return m_host; };
    inline std::string path() const { return m_path; };
    inline std::string filename() const { return m_filename; };
    inline int port() const { return m_port; };
  
  private:
    std::string m_url;
    std::string m_username;
    std::string m_password;
    std::string m_scheme;
    std::string m_host;
    std::string m_path;
    std::string m_filename;
    int m_port;
};

#endif // __URI_H_INCLUDED__ 
