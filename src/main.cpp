#include "commands.hpp"

int main(int argc, char* argv[])
{
   CommandHandler command_handler (argc, argv);
   command_handler.handle_args();
   return 0;
}
