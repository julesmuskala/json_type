#include <string>
#include <stdexcept>

#include "parse_error.h"
#include "value.h"

#ifndef JSON_TYPE__PARSER_H_
#define JSON_TYPE__PARSER_H_

namespace json {

class Parser {
  std::string source_;
  std::size_t pos_ = 0;
  std::size_t line_ = 0;
  std::size_t column_ = 0;

  Value *parse_value_;

  char inline CurrentChar() { return source_[pos_]; }
  bool inline Match(const char c) { return CurrentChar() == c; }
  bool inline MatchPrevious(const char c) { return source_[pos_ - 1] == c; }

  bool inline IsAtEnd() { return pos_ >= source_.size() + 1; }
  bool inline IsAtStrEnd() {
    if (IsAtEnd() || Match('\n')) {
      throw ParseError(line_, column_, "unterminated string literal");
    }
    if (Match('"')) {
      if (MatchPrevious('\\')) {
        return false;
      }
      return true;
    }
    return false;
  }

  void inline Advance() {
    if (IsAtEnd()) {
      throw ParseError(line_, column_, "tried advancing past terminal character");
    }
    ++pos_;
    ++column_;
  }

  void inline Expect(char c) {
    if (!Match(c)) {
      std::string msg = "expected '";
      msg.push_back(c);
      msg.push_back('\'');
      throw ParseError(line_, column_, msg);
    }
    Advance();
  }

  std::string inline String() {
    Advance();
    const auto start = pos_;

    while (!IsAtStrEnd()) Advance();

    const auto end = pos_;

    Advance(); // Account for terminating '"'

    return source_.substr(start, end - start);
  }

  Value *ParseValue() {
    switch (CurrentChar()) {
      case '{':return ParseObject();
      case '[':return ParseArray();
      case '"':return ParseString();
      case 't':
      case 'f':
      case 'n':return ParseBasicValue();
      default: {
        if (IsNumStart()) {
          return ParseNumber();
        }
      }
    }
    throw ParseError(line_, column_, "unrecognized value");
  }

  bool ExpectComma(const char end_char, const std::string &expected_message) {
    Whitespace();
    if (!Match(',')) {
      return false;
    }
    Advance();
    Whitespace();
    if (Match(end_char)) {
      throw ParseError(line_, column_, "expected " + expected_message);
    }
    return true;
  }

  bool inline ExpectNextMember() {
    return ExpectComma('}', "key-value pair");
  }

  ObjectValue *ParseObject() {
    Advance();
    Whitespace();
    if (Match('}')) {
      Advance();
      return new ObjectValue();
    }

    auto *obj = new ObjectValue();
    do {
      auto key = String();
      Whitespace();
      Expect(':');
      Whitespace();
      auto value = ParseValue();
      obj->insert(key, value);
    } while (ExpectNextMember());

    Expect('}');
    return obj;
  }

  bool inline ExpectNextElement() {
    return ExpectComma(']', "value");
  }

  ArrayValue *ParseArray() {
    Advance();
    Whitespace();
    if (Match(']')) {
      Advance();
      return new ArrayValue();
    }

    auto *array = new ArrayValue();
    do {
      array->push_back(ParseValue());
    } while (ExpectNextElement());

    Whitespace();
    Expect(']');
    return array;
  }

  StringValue *ParseString() {
    auto string = String();
    return new StringValue(string);
  }

  bool inline IsE() { return Match('e') || Match('E'); }
  bool inline IsDigit() { return CurrentChar() >= '0' && CurrentChar() <= '9'; }
  bool inline IsSign() { return CurrentChar() == '-' || CurrentChar() == '+'; }
  bool inline IsSeparator() { return CurrentChar() == '.'; }
  bool inline IsNumStart() { return (IsSign() || IsDigit() || IsSeparator() || IsE()); }

  bool inline Integer() {
    const auto start = pos_;
    while (IsDigit()) Advance();
    return start != pos_;
  }
  void inline Fraction() {
    if (IsSeparator()) {
      Advance();
      if (!Integer()) {
        throw ParseError(line_, column_, "expected digits after separator");
      }
    }
  }
  void inline Exponent() {
    if (IsE()) {
      Advance();
      if (IsSign()) {
        Advance();
      }
      if (!Integer()) {
        throw ParseError(line_, column_, "expected digits after exponent");
      }
    }
  }

  NumberValue *ParseNumber() {
    const auto start = pos_;

    if (IsSign()) {
      Advance();
    }
    Integer();
    Fraction();
    Exponent();

    const double number = std::stod(source_.substr(start, pos_ - start));
    return new NumberValue(number);
  }

  void ExpectWord(const std::string &word) {
    for (auto c : word) {
      Expect(c);
    }
  }

  Value *ParseBasicValue() {
    char start_char = CurrentChar();
    switch (start_char) {
      case 'n':ExpectWord("null");
        return new Value(ValueType::kNull);
      case 't':ExpectWord("true");
        return new Value(ValueType::kTrue);
      case 'f':ExpectWord("false");
        return new Value(ValueType::kFalse);
      default:throw ParseError(line_, column_, "unrecognized value");
    }
  }

  bool inline IsWhitespace() {
    char c = CurrentChar();
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
  }

  bool Whitespace() {
    const auto start = pos_;
    while (IsWhitespace()) {
      Advance();
      if (MatchPrevious('\n')) {
        line_++;
        column_ = 0;
      }
    }
    return start != pos_;
  }

 public:
  explicit Parser(std::string s)
      : source_(std::move(s)), parse_value_(nullptr) {}
  Value *Parse() {
    if (source_.empty()) {
      return new Value(ValueType::kNull);
    }

    Whitespace();

    if (IsAtEnd()) {
      return new Value(ValueType::kNull);
    }

    try {
      parse_value_ = ParseValue();
    } catch (const std::exception& e) {
      if (parse_value_ != nullptr) {
        delete parse_value_;
      }
      throw e;
    }

    return parse_value_;
  }
};

}

#endif //JSON_TYPE__PARSER_H_
