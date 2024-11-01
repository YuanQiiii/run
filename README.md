# Run

## Overview
This utility is a command-line tool designed to execute various commands by reading their configurations from a specified file. It supports listing some available commands and their types, as well as executing a specific command based on user input.

## Features
- Convert UTF-8 encoded strings to UTF-16 for compatibility with Windows APIs.
- Execute commands using the Windows PowerShell Start-Process cmdlet.
- List all commands and their associated types from a configuration file.
- Map commands to their respective types and file paths for easy execution.
- Support multiple commands in one input.

## Usage
To use this utility, follow these steps:

1. **Place the Configuration File**: Ensure that the configuration file `paths.txt` is located at `C:\run\paths.txt`. The file should contain lines in the format `command>type>"path"`, where:
   - `command` is the name of the command.
   - `type` is the type of the command (e.g., executable, script).
   - `path` is the full path to the executable or script file.

2. **Compile the Program**: Compile the provided C++ source code using a suitable compiler that supports Windows APIs.

3. **Set the Environment Variables**: Make it easier to use the tool in every terminal.

4. **Run the Program**: Execute the compiled program from the command line. You can either list all commands or run a specific command.

### Listing Commands
To list all commands and their types, run the program with the `ls` argument:
```
run ls
```
This will output all commands and their types as defined in the `paths.txt` file.

### Executing a Command
To execute a command, run the program with the name of the command as an argument:
```
run <command_name>
run <command_name1> <command_name2> <command_name3> <command_name4>
```
Replace `<command_name>` with the actual name of the command you wish to execute. The program will look up the command in the `paths.txt` file, find the associated path and type, and execute it using PowerShell.

## Requirements
- Windows operating system with PowerShell installed.
- A C++ compiler that supports Windows APIs (e.g., MSVC, GCC with MinGW).

## Limitations
- The utility assumes that the `paths.txt` file is correctly formatted and located at the specified path.
- The utility does not handle errors in command execution beyond displaying a "not found" message if the command is not in the configuration file.
- The utility uses the `_wsystem` function, which may have limitations on the complexity of the commands it can execute.

## License
This software is provided under the [MIT License](https://opensource.org/licenses/MIT). Feel free to modify and distribute the code as long as you include the original copyright and license notice in any copy or modification of the software.

## Reference
For environment variable setting ,you can read this blog.
> https://www.cnblogs.com/xy14/p/12371671.html

## Contact
For any questions or issues regarding the utility, please contact the developer at [shihuaidexianyu@gmail.com](mailto:your_email@example.com).
