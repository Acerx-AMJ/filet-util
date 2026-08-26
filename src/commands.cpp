#include "commands.hpp"

#include "files.hpp"
#include "localization.hpp"
#include "templates.hpp"
#include <filesystem>
#include <iostream>
#include <string>

CommandHandler::CommandHandler(int argc, char** argv)
   : argc(argc), argv(argv) {}

void CommandHandler::handle_args()
{
   ensure_args_bigger_or_equal(2);
   std::string command = argv[1];

   if (is_valid_locale(command))
   {
      load_translations(command);
      ensure_args_bigger_or_equal(3);
      command = argv[2];
      locale_added = true;
   }
   else load_translations("-en"s);

   if (command == "-h")
      handle_help();
   else if (command == "-v")
      handle_version();
   else if (command == "-i")
      handle_info();
   else if (command == "-t")
      handle_templates();
   else if (command == "-f")
      handle_files();
   else if (command == "-d")
      handle_directories();
   else if (command == "-r")
      handle_deletion();
   else if (command == "-c")
      handle_class();
   else if (command == "-w")
      handle_write();
   else
   {
      std::cerr << get_text("invalid_arg"s) << '\n';
      std::exit(-1);
   }
}

// Info commands

void CommandHandler::handle_help()
{
   ensure_args_equal(2);
   std::cout << get_text("help_h"s) << '\n';
   std::cout << get_text("help_v"s) << '\n';
   std::cout << get_text("help_i"s) << '\n';
   std::cout << get_text("help_t"s) << '\n';
   std::cout << get_text("help_f"s) << '\n';
   std::cout << get_text("help_d"s) << '\n';
   std::cout << get_text("help_r"s) << '\n';
   std::cout << get_text("help_c"s) << '\n';
   std::cout << get_text("help_w"s) << '\n';
}

void CommandHandler::handle_version()
{
   ensure_args_equal(2);
   std::cout << get_text("version"s, "1.6.4"s) << '\n';
   std::cout << get_text("version_owner"s) << '\n';
}

void CommandHandler::handle_info()
{
   ensure_args_equal(2);
   std::cout << get_text("info_usage"s) << '\n';
   std::cout << get_text("info_filet"s) << '\n';

   std::cout << '\n' << get_text("info_arguments"s) << '\n';
   handle_help();

   std::cout << '\n' << get_text("info_examples"s) << '\n';
   std::cout << get_text("info_example1"s) << '\n';
   std::cout << get_text("info_example2"s) << '\n';
   std::cout << get_text("info_example3"s) << '\n';
   std::cout << get_text("info_example4"s) << '\n';

   std::cout << '\n' << get_text("info_templates"s) << '\n';
   std::cout << "hello_world, repl, args.\n";

   std::cout << '\n' << get_text("info_files"s) << '\n';
   std::cout << "CMakeLists.txt; main.cpp; .gitignore; .clangd; install.sh; build.sh; *.sh; *.hpp; *.cpp.\n";

   std::cout << '\n' << get_text("info_locales"s) << '\n';
   std::cout << "en (" << get_text("info_locale_en"s) << "), lv (" << get_text("info_locale_lv"s) << ").\n\n";

   handle_version();
}

// Action command

void CommandHandler::handle_templates()
{
   ensure_args_equal(3);
   handle_template(argv[2 + locale_added]);
}

void CommandHandler::handle_files()
{
   ensure_args_bigger_or_equal(3);

   for (int i = 2 + locale_added; i < argc; ++i)
      handle_file(std::filesystem::path(argv[i]));
}

void CommandHandler::handle_directories()
{
   ensure_args_bigger_or_equal(3);

   for (int i = 2 + locale_added; i < argc; ++i)
      create_rec_directories(std::filesystem::path(argv[i]));
}

void CommandHandler::handle_deletion()
{
   ensure_args_bigger_or_equal(3);

   for (int i = 2 + locale_added; i < argc; ++i)
      remove_file(std::filesystem::path(argv[i]));
}

void CommandHandler::handle_class()
{
   ensure_args_bigger_or_equal(3);

   for (int i = 2 + locale_added; i < argc; ++i)
   {
      std::string name = argv[i];

      std::filesystem::path hpp_path = std::filesystem::path("include") / (name + ".hpp");
      std::filesystem::path cpp_path = std::filesystem::path("src") / (name + ".cpp");

      std::cout << '\n' << get_text("creating_file"s, hpp_path.string()) << '\n';
      handle_hpp_ext(hpp_path);
      std::cout << '\n' << get_text("creating_file"s, cpp_path.string()) << '\n';
      handle_cpp_ext(cpp_path);
   }
}

void CommandHandler::handle_write()
{
   ensure_args_bigger_or_equal(3);

   for (int i = 2 + locale_added; i < argc; ++i)
      user_write_file(std::filesystem::path(argv[i]));
}

// Arg functions

void CommandHandler::ensure_args_equal(int expected)
{
   if (argc != expected + locale_added)
   {
      std::cerr << get_text("expected_args_equal"s, std::to_string(expected + locale_added - 1), std::to_string(argc - 1)) << '\n';
      std::exit(-1); 
   }
}

void CommandHandler::ensure_args_bigger_or_equal(int minimum)
{
   if (argc < minimum + locale_added)
   {
      std::cerr << get_text("expected_args_bigger_or_equal"s, std::to_string(minimum + locale_added - 1), std::to_string(argc - 1)) << '\n';
      std::exit(-1);
   }
}
