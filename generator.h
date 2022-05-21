#include <sstream>
#include <string>

#include "value.h"

#ifndef JSON_TYPE__GENERATOR_H_
#define JSON_TYPE__GENERATOR_H_

namespace json {

class Generator {
  Value *value_;
  std::stringstream ss_;

  void GenerateValue(Value *value) {
    switch (value->Type()) {
      case ValueType::kFalse:ss_ << "false";
        return;
      case ValueType::kTrue:ss_ << "true";
        return;
      case ValueType::kNull:ss_ << "null";
        return;
      case ValueType::kNumber:GenerateNumber(value);
        return;
      case ValueType::kString:GenerateString(value);
        return;
      case ValueType::kArray:GenerateArray(value);
        return;
      case ValueType::kObject:GenerateObject(value);
        return;
    }
  }

  void inline GenerateNumber(Value *value) {
    ss_ << *((NumberValue *) value);
  }
  void inline GenerateString(Value *value) {
    GenerateString((*(StringValue *) value));
  }
  void inline GenerateString(const std::string &string) {
    ss_ << '"';
    ss_ << string;
    ss_ << '"';
  }
  void GenerateArray(Value *value) {
    ss_ << '[';

    auto array_value = (ArrayValue *) value;
    if (!array_value->empty()) {
      auto i = array_value->begin();
      auto end = array_value->end();
      do {
        GenerateValue(*i);
        ++i;
      } while ((i != end) && InsertComma());
    }

    ss_ << ']';
  }
  void GenerateObject(Value *value) {
    ss_ << '{';

    auto obj_value = (ObjectValue *) value;
    if (!obj_value->empty()) {
      auto i = obj_value->begin();
      auto end = obj_value->end();
      do {
        auto [k, v] = *i;
        GenerateString(k);
        ss_ << ':';
        GenerateValue(v);
        ++i;
      } while ((i != end) && InsertComma());
    }

    ss_ << '}';
  }

  bool inline InsertComma() {
    ss_ << ',';
    return true;
  }

 public:
  explicit Generator(Value *value) : value_(value) {}
  std::string generate() {
    GenerateValue(value_);
    return ss_.str();
  }
};

}

#endif //JSON_TYPE__GENERATOR_H_
