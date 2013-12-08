#include "uri.hpp"

Uri::Uri(const std::string& url) : m_port(0) {
  boost::regex regex(REGEX);
  boost::smatch matches;

  if(boost::regex_search(url, matches, regex)) {
    m_url = matches[0];
    m_scheme = matches[1];
    m_username = matches[2];
    m_password = matches[3];
    m_host = matches[4];

    try {
      m_port = std::stoi(matches[5]);
    } catch (std::invalid_argument e) {
    }

    m_path = matches[6];
    m_filename = matches[7];
  }
}
