#include <stdexcept>

#include "parser.h"
#include "generator.h"
#include "value.h"

#ifndef JSON_TYPE__JSON_H_
#define JSON_TYPE__JSON_H_

namespace json {

class JSON {
  Value *value_;
  ValueType base_type_;

  bool delete_value_;

 public:
  explicit JSON(const std::string &s) {
    Parser parser(s);
    value_ = parser.Parse();
    base_type_ = value_->Type();
    delete_value_ = true;
  }
  explicit JSON(Value *ptr) noexcept
      : value_(ptr), base_type_(ptr->Type()), delete_value_(false) {}

  explicit JSON(Value &value) noexcept
      : value_(&value), base_type_(value.Type()), delete_value_(false) {}

  [[nodiscard]] std::string Stringify() const noexcept {
    Generator generator(value_);
    return generator.generate();
  }
  [[nodiscard]] std::string Stringify(int indent_amount) const noexcept {
    Generator generator(value_, indent_amount);
    return generator.generate();
  }

  Value *GetValue() noexcept {
    return value_;
  }
  ValueType GetType() noexcept {
    return base_type_;
  }

  ~JSON() noexcept {
    if (delete_value_) {
      delete value_;
    }
  }
};

}

#endif //JSON_TYPE__JSON_H_
