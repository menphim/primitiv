#ifndef PRIMITIV_ERROR_H_
#define PRIMITIV_ERROR_H_

#include <sstream>
#include <stdexcept>
#include <string>

namespace primitiv {

/**
 * A library specific exception.
 */
class Error : public std::exception {
  Error() = delete;

public:
  Error(const std::string &file, unsigned line, const std::string &message)
  : file_(file), line_(line), msg_(message) {
    std::stringstream ss;
    ss << file_ << ": " << line_ << ": " << msg_;
    full_msg_ = ss.str();
  }

  const char *what() const noexcept override { return full_msg_.c_str(); }

private:
  std::string file_;
  unsigned line_;
  std::string msg_;
  std::string full_msg_;
};

}  // namespace primitiv

#define THROW_ERROR(cmds) { \
  std::stringstream ss; \
  ss << cmds; \
  throw primitiv::Error(__FILE__, __LINE__, ss.str()); \
}

#endif  // PRIMITIV_ERROR_H_
