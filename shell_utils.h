#ifndef _shell_UTILS_H
#define _shell_UTILS_H

#include <iostream>
#include <climits>
#include <pwd.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <dirent.h>

using namespace std;

typedef struct {
    char username[LOGIN_NAME_MAX];
    char hostname[HOST_NAME_MAX];
    char cwd[PATH_MAX];
} sysInfo_t;

typedef struct {
    vector<string> args;
    bool background;
} command_t;

sysInfo_t get_system_info();
vector<string> parse_input(stringstream& ss);
int handle_tab_stroke(int count, int key);
int builtin_echo(vector<string>& args);
int builtin_execute(vector<string>& args, const string& home_dir, const string& pwd, string& last_wd);
int builtin_ls(vector<string>& args, const string& home_dir, const string& pwd, string& last_wd);

#endif
