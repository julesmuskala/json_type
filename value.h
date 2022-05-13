#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#ifndef JSON_TYPE__VALUE_H_
#define JSON_TYPE__VALUE_H_

namespace json {

enum class ValueType {
  kObject,
  kArray,
  kString,
  kNumber,
  kTrue,
  kFalse,
  kNull,
};

class Value {
 protected:
  ValueType type_;

 public:
  explicit Value(const ValueType type) noexcept: type_(type) {}
  ValueType GetType() noexcept { return type_; }
};

class StringValue : public Value {
  std::string literal_;
 public:
  explicit StringValue(std::string &v) noexcept
      : Value(ValueType::kString), literal_(std::move(v)) {}
  explicit StringValue(const char *v) noexcept
      : Value(ValueType::kString), literal_(std::string(v)) {}
  operator std::string() const { return literal_; }
};

class NumberValue : public Value {
  double literal_;
 public:
  explicit NumberValue(const double v) noexcept
      : Value(ValueType::kNumber), literal_(v) {}
  operator double() const { return literal_; }
};

class ObjectValue : public Value {
  typedef typename std::unordered_map<std::string, Value *> member_map;
  member_map members_;
 public:
  void insert(const std::string &&k, Value* v) { members_.insert({k, v}); }
  ObjectValue()
      : Value(ValueType::kObject), members_() {}
  explicit ObjectValue(member_map &v)
      : Value(ValueType::kObject), members_(std::move(v)) {}
  ~ObjectValue() noexcept {
    for (auto &[k, v] : members_) {
      delete v;
    }
  }
};

class ArrayValue : public Value {
  typedef typename std::vector<Value *> element_vector;
  element_vector elements_;
 public:
  void push_back(Value *v) { elements_.push_back(v); }
  ArrayValue()
      : Value(ValueType::kArray), elements_() {}
  explicit ArrayValue(element_vector &v)
      : Value(ValueType::kArray), elements_(std::move(v)) {}
  ~ArrayValue() noexcept {
    for (auto v : elements_) {
      delete v;
    }
  }
};

}

#endif //JSON_TYPE__VALUE_H_
