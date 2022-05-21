#include <stdexcept>

#include "parser.h"
#include "generator.h"
#include "value.h"

#ifndef JSON_TYPE__JSON_H_
#define JSON_TYPE__JSON_H_

namespace json {

class JSON {
 public:
  Value *value_;
  explicit JSON(const std::string &s) {
    Parser parser(s);
    value_ = parser.Parse();
  }

  [[nodiscard]] std::string Stringify() const {
    Generator generator(value_);
    return generator.generate();
  }
  ~JSON() noexcept {
    delete value_;
  }
};

}

#endif //JSON_TYPE__JSON_H_
