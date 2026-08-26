#ifndef COMMANDS_HPP
#define COMMANDS_HPP

class CommandHandler
{
   bool locale_added = false;
   int argc = 0;
   char** argv;

   // Info commands

   void handle_help();
   void handle_version();
   void handle_info();
   
   // Action commands

   void handle_templates();
   void handle_files();
   void handle_directories();
   void handle_deletion();
   void handle_class();
   void handle_write();

   // Arg functions
   
   void ensure_args_equal(int expected);
   void ensure_args_bigger_or_equal(int minimum);

public:
   CommandHandler(int argc, char** argv);

   void handle_args();
};

#endif
