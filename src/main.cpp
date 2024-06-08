#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <filesystem>

enum CommandType
{
  Builtin,
  Executable,
  Nonexistent,
};

struct FullCommandType
{
  CommandType type;
  std::string executable_path;
};

std::vector<std::string> parse_command_to_string_vector(std::string command);
FullCommandType command_to_full_command_type(std::string command);
std::string find_command_executable_path(std::string command);
std::string find_command_in_path(std::string command, std::string path);

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

      if (command_vector[0] == "type")
      {
        if (command_vector.size() < 2)
        {
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
          std::cout << command_name << " is " << command_type.executable_path << "\n";
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

  // check if the command is found in path
  std::string exec_path = find_command_executable_path(command);
  if (exec_path != "")
  {
    FullCommandType fct;
    fct.type = Executable;
    fct.executable_path = exec_path;
    return fct;
  }

  // nonexistent types
  FullCommandType fct;
  fct.type = CommandType::Nonexistent;
  return fct;
}

std::string find_command_executable_path(std::string command)
{
  char *path = getenv("PATH");

  if (path == NULL)
  {
    return "";
  }

  std::string path_acc = "";

  // accumulate values in path_acc
  // and search whenever the directory is complete
  char *p = path;
  while (*p != '\0')
  {
    // search for end of paths
    if (*p == ':')
    {
      std::string exec_path = find_command_in_path(command, path_acc);

      if (exec_path != "")
      {
        return exec_path;
      }

      path_acc = "";
    }
    else
    {
      path_acc += *p;
    }
    p++;
  }

  // handle the last path in the string
  std::string exec_path = find_command_in_path(command, path_acc);

  if (exec_path != "")
  {
    return exec_path;
  }

  return "";
}

// checks for a command in the directory path
std::string find_command_in_path(std::string command, std::string path)
{
  for (const auto &entry : std::filesystem::directory_iterator(path))
  {
    if (entry.path() == (path + "/" + command))
    {
      return entry.path();
    }
  }
  return "";
}