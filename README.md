# **POSIX-SHELL**

Implemented a fully-functional POSIX shell that can create and manage processes, handle built-in commands (cd, ls, echo, pwd, pinfo, search) and support advanced features including I/O redirection, pipelines, and background process execution. Also implemented interactive shell features like command history, autocomplete functionality, and signal handling for process control.

The following are the specific requirements and my approach:
## 1. Display the shell:
Implemented the function get_system_info() to retrieve username, hostname and current working directory.

### sys-calls/functions used:

    getpwuid(), gethostname(), getcwd()

## 2. echo, pwd, cd
Implemented functions builtin_echo() and builtin_cd() to fulfill the requirements.
### system-calls/functions used:

    getcwd(), chdir()

## 3. ls
- Implemented the function builtin_ls() to list all the files in a directory.
- Handled the flags -l and -a
### used functions:
    opendir(), readdir(), closedir(), lstat()

## 4. Background and Foreground processes
- To implement foreground and background processes, used the fork() to create a child.
- Used execvp() sys call to execute the given command in the child process.
- If the process is foreground, the parent process waits for the child to terminate.
- If it is a background process, the parent simply returns to the shell.
### Sys-calls/functions used:

    fork(), execvp(), waitpid()

## 5. Pinfo
- Implemented builtin_pinfo() to retrieve the required information of a process
- If pid is not given, it prints the info of the shell process.
### File status path:

    /proc/pid/stat

## 6. search
- Implemented a builtin_search() function to search for a file recursively in the current directory.

### used sys-calls/functions:

    opendir(), readdir(), closedir(), lstat()

## 7. I/O redirection
- Parsed the commands to find >, < and >> and updated the input/output files accordingly.
### sys-calls/functions used:

    dup2(), open(), read(), write()

## 8. Piping
- parsed the input command and seperated sub-commands using | as delimiter.
- Set the input and output paths of every command respectively.

### sys-calls/functions used:

    dup2(), open(), read(), write()

## 9. I/O redirection with piping
- if parse the input and seperate commands with | as delimiter and <,> and >>. This will implicitly handle the piping with i/o redirection

### sys-calls/functions used:

    dup2(), open(), read(), write()

## 10. Handle control signals
- handled the ctrlD, ctrlC and ctrlZ signals by the user to fulfill the requirements.

### sys-calls/functions used:

    signal()

## 11. Autocomplete for commands and all the files/directories under the current directory.
- Handled the tab stroke approriately and implemented the auto completion logic by finding the commands at system path variable and files in the current directory.

### sys-calls/functions used:

    getenv(), opendir(), readdir(), closedir(), free()

## 12. History
- Implemented a builtin_history() function to store and read history from a text file.
- Handled the up and down arrow keys to display the previous and next used commands respectively.

### sys-calls/functions used:

    open(), close(), read()

## Additional requirements
- Parsed the input and seperated the commands using ; as the delimiter and then executed the commands iteratively.