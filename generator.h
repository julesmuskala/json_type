#include <sstream>
#include <string>

#include "value.h"

#ifndef JSON_TYPE__GENERATOR_H_
#define JSON_TYPE__GENERATOR_H_

namespace json {

class Generator {
  int indent_amount_;
  int current_indent_;
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
      InsertNewline();
      IncrementIndent();
      auto i = array_value->begin();
      auto end = array_value->end();
      do {
        InsertIndent();
        GenerateValue(*i);
        ++i;
      } while ((i != end) && InsertComma());
      DecrementIndent();
      InsertNewline();
      InsertIndent();
    }

    ss_ << ']';
  }
  void GenerateObject(Value *value) {
    ss_ << '{';

    auto obj_value = (ObjectValue *) value;
    bool is_obj_empty = obj_value->empty();
    if (!is_obj_empty) {
      InsertNewline();
      IncrementIndent();
      auto i = obj_value->begin();
      auto end = obj_value->end();
      do {
        auto [k, v] = *i;
        InsertIndent();
        GenerateString(k);
        ss_ << ':';
        InsertWhitespace();
        GenerateValue(v);
        ++i;
      } while ((i != end) && InsertComma());
      DecrementIndent();
      InsertNewline();
      InsertIndent();
    }

    ss_ << '}';
  }

  bool inline InsertComma() {
    ss_ << ',';
    InsertNewline();
    return true;
  }

  void inline InsertNewline() {
    if (indent_amount_ < 0) return;
    ss_ << '\n';
  }

  void inline IncrementIndent() {
    if (indent_amount_ < 0) return;
    current_indent_ += indent_amount_;
  }

  void inline DecrementIndent() {
    if (indent_amount_ < 0) return;
    current_indent_ -= indent_amount_;
  }

  void inline InsertIndent() {
    if (indent_amount_ < 0) return;
    for (int i = current_indent_; i > 0; i--) {
      ss_ << ' ';
    }
  }

  void inline InsertWhitespace() {
    if (indent_amount_ < 0) return;
    ss_ << ' ';
  }

 public:
  explicit Generator(Value *value)
      : value_(value), indent_amount_(-1), current_indent_(0) {}
  explicit Generator(Value *value, int indent_amount)
      : value_(value), indent_amount_(indent_amount), current_indent_(0) {}
  std::string generate() {
    GenerateValue(value_);
    return ss_.str();
  }
};

}

#endif //JSON_TYPE__GENERATOR_H_
