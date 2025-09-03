#ifndef _shell_UTILS_H
#define _shell_UTILS_H

#include <iostream>
#include <climits>
#include <pwd.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fstream>
#include <limits>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>

using namespace std;

typedef struct {
    char username[LOGIN_NAME_MAX];
    char hostname[HOST_NAME_MAX];
    char cwd[PATH_MAX];
} sysInfo_t;

typedef struct {
    string command;
    vector<string> args;
    string input_file;
    string output_file;
    bool background;
    bool append_output;
} command_t;

typedef struct 
{
    pid_t pid;
    command_t command;
} pInfo_t;

sysInfo_t get_system_info();
vector<command_t> parse_input(string& input_string);
int handle_tab_stroke(int count, int key);
int builtin_echo(vector<string>& args);
int builtin_execute(command_t &cmd, const string& home_dir, const string& pwd, string& last_wd);
int builtin_ls(vector<string>& args, const string& home_dir, const string& pwd, string& last_wd);
int execute_pipe(vector<command_t> &commands, const string& home_dir, const string& pwd, string& last_wd);

vector<string> parse_line(string& line);
vector<command_t> parse_pipe(string& line);
command_t parse_single_command(string& part);

#endif
