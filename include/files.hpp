#ifndef FILES_HPP
#define FILES_HPP

#include <filesystem>
#include <string>

using Path = std::filesystem::path;

void handle_file(const Path& path);
void write_file(const Path& path, const std::string& content);
void create_file(const Path& path);

// Specific file functions

void handle_cmakelists_txt(const Path& path, const std::string& project_name, const std::string& standard);
void handle_main_cpp(const Path& path, bool args);
void handle_gitignore(const Path& path);
void handle_clangd(const Path& path);
void handle_install_sh(const Path& path, const std::string& executable);
void handle_build_sh(const Path& path, const std::string& executable, bool complex);

// Extension functions

void handle_hpp_ext(const Path& path);
void handle_cpp_ext(const Path& path);
void handle_sh_ext(const Path& path);

// Create/remove functions

void create_rec_directories(const Path& path);
void remove_file(const Path& path);
void user_write_file(const Path& path);

#endif
