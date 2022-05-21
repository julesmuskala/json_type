#include <cstddef>
#include <ostream>
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

// TODO Use factory function for constructing Values?

class Value {
 protected:
  ValueType type_;

 public:
  explicit Value(const ValueType type) noexcept: type_(type) {}
  ValueType Type() noexcept { return type_; }
};

typedef std::vector<Value *> element_vector;
typedef std::unordered_map<std::string, Value *> member_map;

class StringValue : public Value {
  std::string literal_;
 public:
  char at(std::size_t i) { return literal_.at(i); }
  char operator[](std::size_t i) { return literal_[i]; }
  char front() noexcept { return literal_.front(); }
  char back() noexcept { return literal_.back(); }

  auto begin() noexcept { return literal_.begin(); }
  [[nodiscard]] auto cbegin() const noexcept { return literal_.cbegin(); }
  auto end() noexcept { return literal_.end(); }
  [[nodiscard]] auto cend() const noexcept { return literal_.cend(); }
  auto rbegin() noexcept { return literal_.rbegin(); }
  [[nodiscard]] auto crbegin() const noexcept { return literal_.crbegin(); }
  auto rend() noexcept { return literal_.rend(); }
  [[nodiscard]] auto crend() const noexcept { return literal_.crend(); }

  [[nodiscard]] bool empty() const noexcept { return literal_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return literal_.size(); }
  [[nodiscard]] std::size_t length() const noexcept { return literal_.length(); }

  auto find(const std::string &s) { return literal_.find(s); }
  auto rfind(const std::string &s) { return literal_.rfind(s); }
  auto find_first_of(const std::string &s) { return literal_.find_first_of(s); }
  auto find_first_not_of(const std::string &s) { return literal_.find_first_not_of(s); }
  auto find_last_of(const std::string &s) { return literal_.find_last_of(s); }
  auto find_last_not_of(const std::string &s) { return literal_.find_last_not_of(s); }

  friend bool operator==(const StringValue &lv, const StringValue &rv) {
    return lv.literal_ == rv.literal_;
  }

  explicit StringValue(std::string &v) noexcept
      : Value(ValueType::kString), literal_(std::move(v)) {}
  explicit StringValue(const char *v) noexcept
      : Value(ValueType::kString), literal_(std::string(v)) {}
  operator std::string() const { return literal_; }
  friend std::ostream &operator<<(std::ostream &os, const StringValue &v) {
    os << v.literal_;
    return os;
  }
};

class NumberValue : public Value {
  double literal_;
 public:
  explicit NumberValue(const double v) noexcept
      : Value(ValueType::kNumber), literal_(v) {}
  operator double() const { return literal_; }
  friend std::ostream &operator<<(std::ostream &os, const NumberValue &v) {
    os << v.literal_;
    return os;
  }
};

class ObjectValue : public Value {
  member_map members_;
 public:
  auto begin() noexcept { return members_.begin(); }
  [[nodiscard]] auto cbegin() const noexcept { return members_.cbegin(); }
  auto end() noexcept { return members_.end(); }
  [[nodiscard]] auto cend() const noexcept { return members_.cend(); }

  [[nodiscard]] bool empty() const noexcept { return members_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return members_.size(); }

  void insert(std::string &k, Value *v) { members_.insert({k, v}); }

  Value *at(const std::string &s) { return members_.at(s); }
  Value *operator[](const std::string &s) { return members_[s]; }
  std::size_t count(const std::string &s) const { return members_.count(s); }
  auto find(const std::string &s) { return members_.find(s); }
  bool contains(const std::string &s) { return members_.contains(s); }
  auto equal_range(const std::string &s) { return members_.equal_range(s); }

  friend bool operator==(const ObjectValue &lv, const ObjectValue &rv) {
    return lv.members_ == rv.members_;
  }

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
  element_vector elements_;
 public:
  Value *at(std::size_t i) { return elements_.at(i); }
  Value *operator[](std::size_t i) { return elements_[i]; }
  Value *front() noexcept { return elements_.front(); }
  Value *back() noexcept { return elements_.back(); }

  auto begin() noexcept { return elements_.begin(); }
  [[nodiscard]] auto cbegin() const noexcept { return elements_.cbegin(); }
  auto end() noexcept { return elements_.end(); }
  [[nodiscard]] auto cend() const noexcept { return elements_.cend(); }
  auto rbegin() noexcept { return elements_.rbegin(); }
  [[nodiscard]] auto crbegin() const noexcept { return elements_.crbegin(); }
  auto rend() noexcept { return elements_.rend(); }
  [[nodiscard]] auto crend() const noexcept { return elements_.crend(); }

  [[nodiscard]] bool empty() const noexcept { return elements_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return elements_.size(); }

  void push_back(Value *v) { elements_.push_back(v); }
  void pop_back() { elements_.pop_back(); }

  friend bool operator==(const ArrayValue &lv, const ArrayValue &rv) {
    return lv.elements_ == rv.elements_;
  }

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
