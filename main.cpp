#include <iostream>

#include "json.h"

int main() {
  // TODO substr not working correctly
  json::JSON json_1(R"({"test": "ggg", "fff": 11.1, "aa": {"bb": "pp", "sss": [111, false, null]}})");
  auto s = json_1.Stringify(4);
//  auto* number_value = (json::NumberValue *) json_1.value_;
//  auto string_value = (json::StringValue *) json_1.value_;
  std::cout << s << std::endl;
  int i = 0;
}
