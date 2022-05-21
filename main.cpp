#include <iostream>

#include "json.h"

int main() {
  // TODO substr not working correctly
  json::JSON json_1(R"({"test": "ggg", "fff": 11.1})");
  auto s = json_1.Stringify();
//  auto* number_value = (json::NumberValue *) json_1.value_;
//  auto string_value = (json::StringValue *) json_1.value_;
  int i = 0;
}
