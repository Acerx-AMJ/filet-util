#include "localization.hpp"

#include <pwd.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

static std::string empty = ""s;
static std::unordered_map<std::string, std::string> table;
static std::unordered_set<std::string> locales
{"-en"s, "-lv"s};

bool is_valid_locale(const std::string& locale)
{
   return locales.find(locale) != locales.end();
}

void load_translations(const std::string& locale)
{
   const char* homedir = getenv("HOME");
   if (!homedir)
      homedir = getpwuid(getuid())->pw_dir;

   std::fstream file (std::filesystem::path(homedir) / (".local/share/filet/locales/"s + locale.substr(1, locale.size() - 1) + ".txt"s));

   if (!file.is_open())
   {
      file.open(std::filesystem::path(homedir) / (".local/share/filet/locales/es.txt"s));

      if (!file.is_open())
      {
         std::cerr << "Could not find a suitable translation file.\n";
         std::exit(-1);
      }
   }

   std::string temp;
   while (std::getline(file, temp))
   {
      if (temp.empty() || temp[0] == '#')
         continue;

      size_t pos = temp.find('=');
      if (pos == temp.npos)
      {
         std::cerr << get_text("invalid_trans"s, temp) << '\n';
         std::exit(-1);
      }
      table[temp.substr(0, pos)] = temp.substr(pos + 1, temp.size() - 1);
   }
}

std::string get_text(const std::string& key, const std::string& arg1, const std::string& arg2)
{
   auto it = table.find(key);
   if (it == table.end())
      return {};
   
   std::string result = it->second;
   if (arg1.empty())
      return result;
   
   size_t pos1 = result.find("{}");
   if (pos1 == std::string::npos)
      return result;
   
   result = result.replace(pos1, 2, arg1);
   size_t pos2 = result.find("{}", pos1 + arg1.size());
   if (pos2 == std::string::npos)
      return result;
   
   return result.replace(pos2, 2, arg2);
}
