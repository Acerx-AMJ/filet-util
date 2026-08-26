#include "files.hpp"

#include "localization.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std::string_literals;

void handle_file(const Path& path)
{
   std::cout << '\n' << get_text("creating_file"s, path.string()) << '\n';

   if (path.filename() == "CMakeLists.txt"s)
   {
      std::cout << get_text("input_project_name"s);
      std::string project_name;
      std::getline(std::cin >> std::ws, project_name);

      std::cout << get_text("input_standard"s);
      std::string standard;
      std::getline(std::cin >> std::ws, standard);

      handle_cmakelists_txt(path, project_name, standard);
   }
   else if (path.filename() == "main.cpp"s)
   {
      std::cout << get_text("main_cpp_args"s);
      std::string args;
      std::getline(std::cin, args);

      handle_main_cpp(path, !args.empty());
   }
   else if (path.filename() == ".gitignore"s)
      handle_gitignore(path);
   else if (path.filename() == ".clangd"s)
      handle_clangd(path);
   else if (path.filename() == "install.sh"s)
   {
      std::cout << get_text("input_executable_name"s);
      std::string executable;
      std::getline(std::cin >> std::ws, executable);

      handle_install_sh(path, executable);
   }
   else if (path.filename() == "build.sh"s)
   {
      std::cout << get_text("input_executable_name"s);
      std::string executable;
      std::getline(std::cin >> std::ws, executable);

      std::cout << get_text("project_complexity"s);
      std::string complexity;
      std::getline(std::cin, complexity);

      handle_build_sh(path, executable, complexity.empty());
   }
   else if (path.extension() == ".sh"s)
      handle_sh_ext(path);
   else if (path.extension() == ".hpp"s)
      handle_hpp_ext(path);
   else if (path.extension() == ".cpp"s)
      handle_cpp_ext(path);
   else
   {
      std::cout << get_text("unsupported_file"s, path.string()) << '\n';
      create_file(path);
   }
}

void write_file(const Path& path, const std::string& content)
{
   if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
   {
      std::cout << '\n' << get_text("replace_file"s, path.string());
      std::string input;
      std::getline(std::cin, input);

      if (input.empty())
         return;
   }

   // To silence errors when creating no directories
   try { std::filesystem::create_directories(path.parent_path()); }
   catch (...) {}

   std::ofstream file (path.string());

   if (!file)
   {
      std::cerr << get_text("could_not_create_file"s, path.string()) << '\n';
      std::exit(-1);
   }

   file << content;
   std::cout << get_text("wrote_file"s, std::to_string(content.size()), path.string()) << '\n';
}

void create_file(const Path& path)
{
   if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
   {
      std::cout << '\n' << get_text("replace_file"s, path.string());
      std::string input;
      std::getline(std::cin, input);

      if (input.empty())
         return;
   }

   // To silence errors when creating no directories
   try { std::filesystem::create_directories(path.parent_path()); }
   catch (...) {}

   std::ofstream file (path.string());

   if (!file)
   {
      std::cerr << get_text("could_not_create_file"s, path.string()) << '\n';
      std::exit(-1);
   }

   std::cout << get_text("created_file"s, path.string());
}

// Specific file functions

void handle_cmakelists_txt(const Path& path, const std::string& project_name, const std::string& standard)
{
   std::string content = "cmake_minimum_required(VERSION 3.10)\nproject("s + project_name + " VERSION 1.0.0)\n\nset(CMAKE_CXX_STANDARD "s + standard + ")\nset("s + "CMAKE_CXX_STANDARD_REQUIRED ON)\nset(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n"s + "\ninclude_directories(${PROJECT_SOURCE_DIR}/include)\nfile(GLOB SOURCES"s + " ${PROJECT_SOURCE_DIR}/src/**.cpp)\n\nadd_executable(${PROJECT_NAME}"s + "${SOURCES})\nset_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)\n"s;
   write_file(path, content);
}

void handle_main_cpp(const Path& path, bool args)
{
   std::string content = "#include <iostream>\n\nint main("s + (args ? "int argc, char* argv[]"s : ""s) + ")\n{\n   std::cout << \""s + get_text("hello_world"s) + "!\\n\";\n   return 0;\n}\n"s;
   write_file(path, content);
}

void handle_gitignore(const Path& path)
{
   std::string content = "/build/\n/.cache/\n/.vscode/\n/.clangd/\n/.gitignore/\n";
   write_file(path, content);
}

void handle_clangd(const Path& path)
{
   std::string content = "CompileFlags:\n   Add:\n      - -I"s;

   if (std::filesystem::exists("include"s) && std::filesystem::is_directory("include"s))
      content += std::filesystem::absolute("include"s).string() + '\n';
   else
   {
      std::cout << get_text("input_include"s);
      std::string folder;
      std::getline(std::cin >> std::ws, folder);
      content += std::filesystem::absolute(folder).string() + '\n';
   }
   write_file(path, content);
}

void handle_install_sh(const Path& path, const std::string& executable)
{
   std::string content = "#!/bin/bash\nsudo mv "s + executable + " /usr/bin/"s + executable + "\n"s;

   write_file(path, content);
   if (system(("chmod +x "s + path.string()).c_str()))
      std::cout << get_text("made_executable"s, path.string()) << '\n';
}

void handle_build_sh(const Path& path, const std::string& executable, bool complex)
{
   std::string content = "#!/bin/bash\nclang++ "s + (complex ? "-Iinclude src/**"s : "main"s) + ".cpp -o "s + executable + "\n"s;

   write_file(path, content);
   if (system(("chmod +x "s + path.string()).c_str()))
      std::cout << get_text("made_executable"s, path.string()) << '\n';
}

// Extension functions

void handle_hpp_ext(const Path& path)
{
   std::filesystem::path current = path.parent_path();
   while (current.has_relative_path() && current.filename() != "include"s)
      current = current.parent_path();
   
   std::string guard;
   
   if (current.filename() == "include"s)
      guard = path.lexically_relative(current).string();
   else
      guard = path.filename();

   std::replace(guard.begin(), guard.end(), '/', '_');
   std::replace(guard.begin(), guard.end(), '.', '_');
   std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);

   std::string content = "#ifndef "s + guard + "\n#define "s + guard + "\n\n\n\n#endif\n"s;
   write_file(path, content);
}

void handle_cpp_ext(const Path& path)
{
   std::string content;
   std::string source = path.stem().string() + ".hpp"s;

   if (std::filesystem::exists(source))
      content = "#include \""s + source + "\"\n"s;
   else if (std::filesystem::exists("include/"))
   {
      for (const auto& entry : std::filesystem::recursive_directory_iterator("include/"))
      {
         if (!entry.is_regular_file() || entry.path().filename() != source)
            continue;

         std::filesystem::path current = entry.path();

         while (current.has_relative_path() && current.filename() != "include"s)
            current = current.parent_path();
         
         if (current.filename() != "include"s)
            goto manual_cpp;
         
         current = entry.path().lexically_relative(current);
         content = "#include \""s + current.string() + "\"\n"s;
      }
   }

manual_cpp:
   if (content.empty())
      content = "#include \""s + source + "\"\n"s;

   write_file(path, content);
}

void handle_sh_ext(const Path& path)
{
   std::string content = "#!/bin/bash\n\n"s;
   write_file(path, content);
   if (system(("chmod +x "s + path.string()).c_str()))
      std::cout << get_text("made_executable"s, path.string()) << '\n';
}

// Create/remove functions

void create_rec_directories(const Path& path)
{
   std::cout << '\n' << get_text("creating_dir"s, path.string()) << '\n';

   try { std::filesystem::create_directories(path); }
   catch (...)
   {
      std::cerr << get_text("could_not_create_dir"s, path.string()) << '\n';
      std::exit(-1);
   }
   std::cout << get_text("created_dir"s, path.string()) << '\n';
}

void remove_file(const Path& path)
{
   std::cout << '\n' << get_text("deleting"s, path.string()) << '\n';

   try { std::filesystem::remove_all(path); }
   catch (...)
   {
      std::cerr << get_text("could_not_delete"s, path.string()) << '\n';
      std::exit(-1);
   }
   std::cout << get_text("deleted"s, path.string()) << '\n';
}

void user_write_file(const Path& path)
{
  if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
   {
      std::cout << '\n' << get_text("replace_file"s, path.string());
      std::string input;
      std::getline(std::cin, input);

      if (input.empty())
         return;
   }

   std::cout << get_text("writing_file"s, path.string()) << '\n';
   std::string content;
   size_t line = 0;

   while (true)
   {
      // Ugly, but works until line 9999, why would anyone ever write a file longer than that in a CLI tool? <- Never assume what users might or might not do
      std::cout << std::string((line < 9 ? 3 : (line < 99 ? 2 : (line < 999 ? 1 : 0))), ' ') << ++line << " ";
      std::string temp;
      std::getline(std::cin, temp);

      if (temp == ":d"s)
      {
         std::cout << get_text("write_discarded"s, path.string()) << '\n';
         return;
      }

      if (temp == ":q"s)
         break;
      content += temp + '\n';
   }

   // To silence errors when creating no directories
   try { std::filesystem::create_directories(path.parent_path()); }
   catch (...) {}

   std::ofstream file (path.string());

   if (!file)
   {
      std::cerr << get_text("could_not_create_file"s, path.string()) << '\n';
      std::exit(-1);
   }

   file << content;
   std::cout << get_text("wrote_file"s, std::to_string(content.size()), path.string()) << '\n';
}
