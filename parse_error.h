#include <stdexcept>
#include <string>

#ifndef JSON_TYPE__PARSE_ERROR_H_
#define JSON_TYPE__PARSE_ERROR_H_

namespace json {

class ParseError : public std::logic_error {
 public:
  ParseError(const std::size_t lnIdx, const std::size_t colIdx, const std::string& message)
  : std::logic_error("At line " + std::to_string(lnIdx + 1) + ", column " + std::to_string(colIdx + 1) + ": " + message) {}
};

}

#endif //JSON_TYPE__PARSE_ERROR_H_
