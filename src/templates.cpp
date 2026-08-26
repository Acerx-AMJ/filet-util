#include "templates.hpp"

#include "files.hpp"
#include "localization.hpp"
#include <iostream>

using namespace std::string_literals;

void handle_template(const std::string& template_name)
{
   std::cout << '\n' << get_text("creating_template"s, template_name) << '\n';

   if (template_name == "hello_world"s)
      template_hello_world();
   else if (template_name == "repl"s)
      template_repl();
   else if (template_name == "args"s)
      template_args();
   else
   {
      std::cerr << get_text("unsupported_template"s, template_name) << '\n';
      std::exit(-1);
   }
   std::cout << get_text("created_template"s, template_name) << '\n';
}

void template_hello_world()
{
   std::cout << get_text("project_complexity"s);
   std::string multiple;
   std::getline(std::cin, multiple);

   std::string content = "#include <iostream>\n\nint main()\n{\n   std::cout << \""s + get_text("hello_world") + "!\\n\";\n   return 0;\n}\n"s;

   if (!multiple.empty())
   {
      std::cout << '\n' << get_text("creating_file"s, "main.cpp"s) << '\n';
      write_file("main.cpp"s, content);
      return;
   }

   std::cout << get_text("use_clangd_or_cmake"s);
   std::string build_type;
   std::getline(std::cin, build_type);

   std::cout << '\n' << get_text("creating_file"s, "src/main.cpp"s) << '\n';
   write_file("src/main.cpp"s, content);
   create_rec_directories("include"s);

   if (build_type.empty())
   {
      handle_file(".clangd"s);
      
      std::cout << get_text("input_executable_name"s);
      std::string executable;
      std::getline(std::cin >> std::ws, executable);

      std::cout << '\n' << get_text("creating_file"s, "build.sh"s) << '\n';
      handle_build_sh("build.sh"s, executable, true);
   }
   else
   {
      create_rec_directories("build"s);
      handle_file("CMakeLists.txt"s);
   }
}

void template_repl()
{
   std::cout << get_text("use_clangd_or_cmake"s);
   std::string build_type;
   std::getline(std::cin, build_type);

   std::string catcher_hpp_content = "#ifndef CATCHER_HPP\n#define CATCHER_HPP\n\n#include <vector>\n\nclass Catcher\n{\n   std::vector<const char*> errors;\npublic:\n\n   void insert(const char* error);\n   bool ok() const;\n   bool display();\n};\n\n#endif\n"s;
   std::string catcher_cpp_content = "#include \"catcher.hpp\"\n\n#include <iostream>\n\nvoid Catcher::insert(const char* error)\n{\n   errors.push_back(error);\n}\n\nbool Catcher::ok() const\n{\n   return errors.empty();\n}\n\nbool Catcher::display()\n{\n   if (ok()) return true;\n\n   std::cout << '\\n' << errors.size() << \" Error(s) occurred:\\n\";\n   for (const auto& error: errors)\n      std::cout << error << '\\n';\n\n   std::cout << '\\n';\n   errors.clear();\n   return false;\n}\n"s;
   std::string commands_hpp_content = "#ifndef COMMANDS_HPP\n#define COMMANDS_HPP\n\n#include \"catcher.hpp\"\n#include <string>\n\nclass CommandHandler\n{\n   Catcher& catcher;\n\n   void handle_help();\n   void handle_quit();\n   void handle_invalid_command();\n\npublic:\n   CommandHandler(Catcher& catcher);\n   void handle_command(const std::string& command);\n};\n\n#endif\n"s;
   std::string commands_cpp_content = "#include \"commands.hpp\"\n\n#include <iostream>\n\nusing namespace std::string_literals;\n\nCommandHandler::CommandHandler(Catcher& catcher)\n   : catcher(catcher) {}\n\nvoid CommandHandler::handle_command(const std::string& command)\n{\n   if (command == \"help\"s)\n      handle_help();\n   else if (command == \"quit\"s)\n      handle_quit();\n   else\n      handle_invalid_command();\n}\n\nvoid CommandHandler::handle_help()\n{\n   std::cout << \"help  Show list of commands.\\n\";\n   std::cout << \"quit  Quit the REPL.\\n\";\n}\n\nvoid CommandHandler::handle_quit()\n{\n   std::exit(0);\n}\n\nvoid CommandHandler::handle_invalid_command()\n{\n   catcher.insert(\"Invalid command, see 'help' for a list of commands.\");\n}\n"s;
   std::string main_cpp_content = "#include \"commands.hpp\"\n#include <iostream>\n\nint main()\n{\n   Catcher catcher;\n   CommandHandler command_handler (catcher);\n\n   while (true)\n   {\n      std::cout << \" > \";\n      std::string command;\n      std::getline(std::cin >> std::ws, command);\n\n      command_handler.handle_command(command);\n      catcher.display();\n   }\n   return 0;\n}\n"s;

   std::cout << '\n' << get_text("creating_file"s, "include/catcher.hpp"s) << '\n';
   write_file("include/catcher.hpp"s, catcher_hpp_content);

   std::cout << '\n' << get_text("creating_file"s, "src/catcher.cpp"s) << '\n';
   write_file("src/catcher.cpp"s, catcher_cpp_content);

   std::cout << '\n' << get_text("creating_file"s, "include/commands.hpp"s) << '\n';
   write_file("include/commands.hpp"s, commands_hpp_content);

   std::cout << '\n' << get_text("creating_file"s, "src/commands.cpp"s) << '\n';
   write_file("src/commands.cpp"s, commands_cpp_content);

   std::cout << '\n' << get_text("creating_file"s, "src/main.cpp"s) << '\n';
   write_file("src/main.cpp"s, main_cpp_content);

   if (build_type.empty())
   {
      handle_file(".clangd"s);
      
      std::cout << get_text("input_executable_name"s);
      std::string executable;
      std::getline(std::cin >> std::ws, executable);

      std::cout << '\n' << get_text("creating_file"s, "build.sh"s) << '\n';
      handle_build_sh("build.sh"s, executable, true);
   }
   else
   {
      create_rec_directories("build"s);
      handle_file("CMakeLists.txt"s);
   }
}

void template_args()
{
   std::cout << get_text("use_clangd_or_cmake"s);
   std::string build_type;
   std::getline(std::cin, build_type);

   std::string catcher_hpp_content = "#ifndef CATCHER_HPP\n#define CATCHER_HPP\n\n#include <vector>\n\nclass Catcher\n{\n   std::vector<const char*> errors;\npublic:\n\n   void error(const char* error);\n   void insert(const char* error);\n   bool ok() const;\n   void display();\n};\n\n#endif\n"s;
   std::string catcher_cpp_content = "#include \"catcher.hpp\"\n\n#include <iostream>\n\nvoid Catcher::error(const char* error)\n{\n   insert(error);\n   display();\n}\n\nvoid Catcher::insert(const char* error)\n{\n   errors.push_back(error);\n}\n\nbool Catcher::ok() const\n{\n   return errors.empty();\n}\n\nvoid Catcher::display()\n{\n   if (ok()) return;\n\n   std::cout << '\\n' << errors.size() << \" Error(s) occurred:\\n\";\n   for (const auto& error: errors)\n      std::cout << error << '\\n';\n\n   std::cout << '\\n';\n   errors.clear();\n   std::exit(-1);\n}\n"s;
   std::string commands_hpp_content = "#ifndef LANGUAGES_HPP\n#define LANGUAGES_HPP\n\n#include <string>\n\nclass Catcher;\n\nvoid handle_command(const std::string& arg1, const std::string& arg2, Catcher& catcher);\nvoid handle_help();\n\n#endif\n"s;
   std::string commands_cpp_content = "#include \"commands.hpp\"\n\n#include \"catcher.hpp\"\n#include <functional>\n#include <unordered_map>\n#include <utility>\n\nusing namespace std::string_literals;\n\nstatic std::unordered_map<std::string, std::pair<std::function<void()>, std::function<void(const std::string&, Catcher&)>>> command_map {\n   \n};\n\nvoid handle_command(const std::string& arg1, const std::string& arg2, Catcher& catcher)\n{\n   auto it = command_map.find(arg1);\n   if (it == command_map.end())\n      catcher.error(\"Command does not exist. See -h for a list of commands.\");\n\n   if (arg2 == \"-h\"s)\n      it->second.first();\n   else\n      it->second.second(arg2, catcher);\n}\n\nvoid handle_help()\n{\n   std::exit(0);\n}\n"s;
   std::string main_cpp_content = "#include \"catcher.hpp\"\n#include \"commands.hpp\"\n\nint main(int argc, char* argv[])\n{\n   Catcher catcher;\n   if (argc == 2 && std::string(argv[1]) == \"-h\")\n      handle_help();\n\n   if (argc != 3)\n      catcher.error(\"Expected 2 arguments.\");\n\n   std::string arg1 = argv[1], arg2 = argv[2];\n   handle_command(arg1, arg2, catcher);\n   catcher.display();\n}\n"s;

   std::cout << '\n' << get_text("creating_file"s, "include/catcher.hpp"s) << '\n';
   write_file("include/catcher.hpp"s, catcher_hpp_content);

   std::cout << '\n' << get_text("creating_file"s, "src/catcher.cpp"s) << '\n';
   write_file("src/catcher.cpp"s, catcher_cpp_content);

   std::cout << '\n' << get_text("creating_file"s, "include/commands.hpp"s) << '\n';
   write_file("include/commands.hpp"s, commands_hpp_content);

   std::cout << '\n' << get_text("creating_file"s, "src/commands.cpp"s) << '\n';
   write_file("src/commands.cpp"s, commands_cpp_content);

   std::cout << '\n' << get_text("creating_file"s, "src/main.cpp"s) << '\n';
   write_file("src/main.cpp"s, main_cpp_content);

   if (build_type.empty())
   {
      handle_file(".clangd"s);
      
      std::cout << get_text("input_executable_name"s);
      std::string executable;
      std::getline(std::cin >> std::ws, executable);

      std::cout << '\n' << get_text("creating_file"s, "build.sh"s) << '\n';
      handle_build_sh("build.sh"s, executable, true);
   }
   else
   {
      create_rec_directories("build"s);
      handle_file("CMakeLists.txt"s);
   }
}
