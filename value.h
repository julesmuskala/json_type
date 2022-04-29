#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

#ifndef JSON_TYPE__VALUE_H_
#define JSON_TYPE__VALUE_H_


namespace json {

// JSON uses unicode strings
// TODO finish comment
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
typedef typename std::wstring u_string;
#else
typedef typename std::string u_string;
#endif

enum class ValueType {
  kString,
  kNumber,
  kObject,
  kArray,
  kTrue,
  kFalse,
  kNull,
};

class Value {
 protected:
  ValueType type_;

 public:
  explicit Value(ValueType type)
      : type_(type) {}
  Value(const double value)
      : type_(ValueType::kNumber) {}
  Value(const int value)
      : type_(ValueType::kNumber) {}
  Value(const u_string&& string)
      : type_(ValueType::kString) {}
  Value(const char* string)
      : type_(ValueType::kString) {}
  Value(const bool value) {
    if (value) {
      type_ = ValueType::kTrue;
      return;
    }
    type_ = ValueType::kFalse;
  }
  Value(std::nullptr_t value)
      : type_(ValueType::kNull) {}
  Value(const Value& value) = default;
  Value(Value&& value) = default;
};

class StringValue : public Value {
  u_string literal_;
 public:
  StringValue(const u_string&& string)
      : Value(ValueType::kString), literal_(string) {}
  StringValue(const char* string)
      : Value(ValueType::kString), literal_(string) {}
};

class NumberValue : public Value {
  double literal_;
 public:
  NumberValue(const double value)
      : Value(ValueType::kNumber), literal_(value) {}
};

class ObjectValue : public Value {

};

class ArrayValue : public Value {
  std::vector<Value> literal_;
 public:
  ArrayValue(const std::vector<Value>&& value)
      : Value(ValueType::kArray), literal_(value) {}
  ArrayValue(const Value* value)
      : Value(ValueType::kArray), literal_{value} {}
  ArrayValue(const std::initializer_list<Value>&& value)
      : Value(ValueType::kArray), literal_{value} {}
};

}


#endif //JSON_TYPE__VALUE_H_
