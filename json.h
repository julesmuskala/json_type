#include "parser.h"
#include "value.h"

#ifndef JSON_TYPE__JSON_H_
#define JSON_TYPE__JSON_H_

namespace json {

class JSON {
 public:
  Value *value_;
//  JSON() noexcept: value_(new Value(ValueType::kNull)) {}
  explicit JSON(const std::string &s) {
    Parser parser(s);
    value_ = parser.Parse();
  }

//  static std::string Stringify(const JSON &json) {}
//  std::string Stringify() { return JSON::Stringify(*this); }
  ~JSON() noexcept {
    delete value_;
  }
};

}

#endif //JSON_TYPE__JSON_H_
