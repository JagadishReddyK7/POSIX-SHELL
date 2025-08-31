#include "shell_utils.h"

using namespace std;

sysInfo_t get_system_info() {
    sysInfo_t sys_info;
    struct passwd *password = getpwuid(getuid());
    if(password) {
        snprintf(sys_info.username, sizeof(sys_info.username), "%s", password->pw_name);
    }
    else {
        snprintf(sys_info.username, sizeof(sys_info.username), "%s", "unknown");
    }
    if(gethostname(sys_info.hostname, sizeof(sys_info.hostname))!=0) {
        snprintf(sys_info.hostname, sizeof(sys_info.hostname), "%s", "unknown_host");
    }
    if(getcwd(sys_info.cwd, sizeof(sys_info.cwd))==NULL) {
        snprintf(sys_info.cwd, sizeof(sys_info.cwd), "%s", "unknown_dir");
    }
    return sys_info;
}

vector<string> parse_input(stringstream& ss) {
    string token;
    vector<string> commands;
    while(ss >> token) {
        commands.push_back(token);
    }
    return commands;
}

int handle_tab_stroke(int count, int key) {

    return 0;
}

int builtin_echo(vector<string>& args) {
    int num=args.size();
    for(int i=1;i<num;i++) {
        cout<<args[i]<<" ";
    }
    cout<<endl;
    return 1;
}

int builtin_ls(vector<string>& args, const string& home_dir, const string& pwd, string& last_wd) {
    const char* path=".";
    DIR *dir=opendir(path);
    if(!dir) perror("ls failed");
    struct dirent *dir_entry;
    while(dir_entry=readdir(dir)) {
        if(dir_entry->d_name[0]!='.') {
            cout<<dir_entry->d_name<<endl;
        }
    }
    closedir(dir);
    return 1;
}

int builtin_cd(vector<string>& args, const string& home_dir, const string& pwd, string& last_wd) {
    string target_dir;
    if(args.size()>2) {
        perror("Invalid arguments");
        return 1;
    }
    if(args.size()==1 || (args.size()>1 && args[1]=="~")) {
        target_dir=home_dir;
    }
    else if(args[1]=="-") {
        target_dir=last_wd;
        cout<<target_dir<<endl;
    }
    else if(args[1]=="..") {

    }
    else {
        target_dir=args[1];
    }
    if(chdir(target_dir.c_str())!=0) {
        perror(target_dir.c_str());
    }
    else last_wd=pwd;
    return 1;
}

int builtin_execute(vector<string>& args, const string& home_dir, const string& pwd, string& last_wd) {
    if(args[0]=="echo") {
        return builtin_echo(args);
    }
    else if(args[0]=="cd") {
        return builtin_cd(args, home_dir, pwd, last_wd);
    }
    else if(args[0]=="pwd") {
        cout<<pwd<<endl;
        return 1;
    }
    else if(args[0]=="ls") {
        return builtin_ls(args, home_dir, pwd, last_wd);
    }
    else {
        cout<<args[0]<<": command not found"<<endl;
        return 1;
    }
}