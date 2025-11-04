# File Manager

This is a simple CLI based file manager application.

## Project Structure

The project is organized as follows:

- `include/`: Contains header files (`.h`) for various functionalities.
  - `file_ops.h`: Declarations for file operation functions.
  - `utils.h`: Declarations for utility functions.
- `src/`: Contains source files (`.cpp`) implementing the functionalities.
  - `file_ops.cpp`: Implementations of file operation functions.
  - `main.cpp`: The main application entry point.
- `.vscode/`: Contains VS Code specific settings.
- `file-manager.exe`: The compiled executable (after building).

## Building the Project

To build this project, you will typically use a C++ compiler like g++ or MSVC.

### Using g++ (MinGW/Cygwin on Windows, or Linux)

1.  Navigate to the project root directory in your terminal.
2.  Compile the source files:
    ```bash
    g++ -Iinclude src/*.cpp -o file-manager.exe
    ```

### Using MSVC (Visual Studio Command Prompt on Windows)

1.  Open a Developer Command Prompt for Visual Studio.
2.  Navigate to the project root directory.
3.  Compile the source files:
    ```bash
    cl /Iinclude src\*.cpp /Fe:file-manager.exe
    ```

## Running the Application

After building the project, you can run the executable from your terminal.

1.  Navigate to the project root directory.
2.  Execute the program:
    ```bash
    .\file-manager.exe
    ```