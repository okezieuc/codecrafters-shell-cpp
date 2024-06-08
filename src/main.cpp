#include <iostream>
#include <vector>
#include <algorithm>

enum CommandType
{
  Builtin,
  Executable,
  Nonexistent,
};

struct FullCommandType
{
  CommandType type;
  std::string executable_location;
};

std::vector<std::string> parse_command_to_string_vector(std::string command);
FullCommandType command_to_full_command_type(std::string command);

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> command_vector = parse_command_to_string_vector(input);

    if (command_vector.size() == 0)
    {
      continue;
    }

    FullCommandType fct = command_to_full_command_type(command_vector[0]);

    // handle builtin commands
    if (fct.type == Builtin)
    {
      if (command_vector[0] == "exit")
      {
        int exit_code = std::stoi(command_vector[1]);
        return exit_code;
      }

      if (command_vector[0] == "echo")
      {
        for (int i = 1; i < command_vector.size(); i++)
        {
          // print a space before every item that is not the first
          if (i != 1)
          {
            std::cout << " ";
          }

          std::cout << command_vector[i];
        }

        std::cout << "\n";
        continue;
      }

      if(command_vector[0] == "type") {
        if(command_vector.size() < 2) {
          continue;
        }

        std::string command_name = command_vector[1];
        FullCommandType command_type = command_to_full_command_type(command_name);

        switch (command_type.type)
        {
        case Builtin:
          std::cout << command_name << " is a shell builtin\n";
          break;
        case Executable:
          std::cout << command_name << " is " << command_type.executable_location << "\n";
          break;
        case Nonexistent:
          std::cout << command_name << " not found\n";
          break;
        default:
          break;
        }

        continue;
      }

      continue;
    }

    std::cout << input << ": command not found\n";
  }
}

std::vector<std::string> parse_command_to_string_vector(std::string command)
{
  std::vector<std::string> args;
  std::string arg_acc = "";

  for (char c : command)
  {
    if (c == ' ')
    {
      args.push_back(arg_acc);
      arg_acc = "";
    }
    else
    {
      arg_acc += c;
    }
  }

  if (arg_acc != "")
  {
    args.push_back(arg_acc);
  }

  return args;
}

// returns the full command type of a command (without arguments)
FullCommandType command_to_full_command_type(std::string command)
{
  std::vector<std::string> builtin_commands = {"exit", "echo", "type"};

  // handle builtin commands
  if (std::find(builtin_commands.begin(), builtin_commands.end(), command) != builtin_commands.end())
  {
    FullCommandType fct;
    fct.type = CommandType::Builtin;

    return fct;
  }

  // nonexistent types
  FullCommandType fct;
  fct.type = CommandType::Nonexistent;
  return fct;
}