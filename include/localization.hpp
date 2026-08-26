#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#include <string>

using namespace std::string_literals;

bool is_valid_locale(const std::string& locale);
void load_translations(const std::string& locale);

std::string get_text(const std::string& key, const std::string& arg1 = ""s, const std::string& arg2 = ""s);

#endif
