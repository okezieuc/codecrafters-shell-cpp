#include <iostream>
#include <vector>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> builtin_commands = {"exit"};

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

    // handle builtin commands
    if (std::find(builtin_commands.begin(), builtin_commands.end(), command_vector[0]) != builtin_commands.end())
    {
      if (command_vector[0] == "exit")
      {
        int exit_code = std::stoi(command_vector[1]);
        return exit_code;
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
