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

bool isNumber(string& arg) {
    if(arg.empty()) return false;
    for (char c : arg) 
    {
        if (!isdigit((unsigned char)c)) {
            return false; 
        }
    }
    return true;
}

vector<string> tokenize(string &part) {
    vector<string> tokens;
    const char* delimiters = " \t\n\r";
    char* token=strtok(&part[0], delimiters);
    while(token!=nullptr) {
        tokens.push_back(string(token));
        token=strtok(nullptr, delimiters);
    }
    return tokens;
}

vector<command_t> parse_input(string& input_string) {
    string part(input_string);
    vector<command_t> commands;
    size_t pos=0;
    while((pos=input_string.find(';'))!=string::npos) {
        part=input_string.substr(0, pos);
        if(!part.empty()) {
            commands.push_back(command_t());
            commands.back().args=tokenize(part);
        }
        part.erase(0, pos+1);
    }
    if(!part.empty()) {
        commands.push_back(command_t());
        commands.back().args=tokenize(part);
    }
    for(command_t &cmd:commands) {
        if(!cmd.args.empty() && cmd.args.back()=="&") {
            cmd.args.pop_back();
        }
    }
    return commands;
}

vector<command_t> parse_line(string& line) {
    vector<command_t> commands;
    
    size_t pos = 0;
    string part;
    while ((pos = line.find(';')) != string::npos) 
    {
        part = line.substr(0, pos);
        line.erase(0, pos + 1); 
        if (part.empty()) continue;
        
        commands.push_back(parse_single_command(part));
    }
    if (!line.empty()) {
        commands.push_back(parse_single_command(line));
    }
    return commands;
}

command_t parse_single_command(string& part) {
    command_t cmd;
    cmd.command = part;
    cmd.append_output = false;
    vector<string> raw_tokens = tokenize(part);
    if (raw_tokens.empty()) return cmd;

    for (size_t i = 0; i < raw_tokens.size(); i++) 
    {
        string& token = raw_tokens[i];
        if (token == "<") {
            if (i + 1 < raw_tokens.size()) 
            {
                cmd.input_file = raw_tokens[++i];
            }
        } 
        else if (token == ">") 
        {
             if (i + 1 < raw_tokens.size()) 
             {
                cmd.output_file = raw_tokens[++i];
                cmd.append_output = false;
            }
        } else if (token == ">>") 
        {
             if (i + 1 < raw_tokens.size()) {
                cmd.output_file = raw_tokens[++i];
                cmd.append_output = true;
            }
        } 
        else if (token == "&" && i == raw_tokens.size() - 1) 
        {
            cmd.background = true;
        } 
        else 
        {
            cmd.args.push_back(token);
        }
    }
    return cmd;
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
    while((dir_entry=readdir(dir))) {
        if(dir_entry->d_name[0]!='.') {
            cout<<dir_entry->d_name<<endl;
        }
    }
    closedir(dir);
    return 1;
}

int builtin_pinfo(const pid_t &pid_in, const string& home_dir) {
    string pid_str = to_string(pid_in);
    string stat_path = "/proc/" + pid_str + "/stat";

    ifstream stat_file(stat_path);
    if (!stat_file) {
        cerr << "pinfo: process with PID " << pid_str << " not found.\n";
        return 1;
    }

    string pid, cmd, state;
    int pgrp, tpgid;
    unsigned long vsize;

    stat_file >> pid >> cmd >> state;
    stat_file.ignore(numeric_limits<streamsize>::max(), ' ');
    stat_file >> pgrp;
    for (int i = 0; i < 16; i++) stat_file >> ws;
    stat_file >> tpgid;
    for (int i = 0; i < 7; i++) stat_file >> ws;
    stat_file >> vsize;

    cout << "Process Status -- {" << state;
    if (pgrp == tpgid) cout << "+";
    cout << "}\n";

    cout << "Memory -- " << vsize << " {Virtual Memory}\n";

    string exe_path_str = "/proc/" + pid_str + "/exe";
    char exe_path[PATH_MAX] = {0};
    ssize_t len = readlink(exe_path_str.c_str(), exe_path, sizeof(exe_path) - 1);

    if (len != -1) {
        string final_path(exe_path);
        if (final_path.rfind(home_dir, 0) == 0) {
            final_path.replace(0, home_dir.size(), "~");
        }
        cout << "Executable Path -- " << final_path << "\n";
    }
	cout << endl;
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

int builtin_execute(command_t &cmd, const string& home_dir, const string& pwd, string& last_wd) {
    if(cmd.args[0]=="echo") {
        return builtin_echo(cmd.args);
    }
    else if(cmd.args[0]=="cd") {
        return builtin_cd(cmd.args, home_dir, pwd, last_wd);
    }
    else if(cmd.args[0]=="pwd") {
        cout<<pwd<<endl;
        return 1;
    }
    else if(cmd.args[0]=="ls") {
        return builtin_ls(cmd.args, home_dir, pwd, last_wd);
    }
    else if(cmd.args[0]=="pinfo") {
        if(cmd.args.size()==1) {
            pid_t pid=getpid();
            builtin_pinfo(pid, home_dir);
        }
        for(size_t i=1;i<cmd.args.size();i++) {
            if(isNumber(cmd.args[i])) {
                builtin_pinfo((pid_t)atoi(cmd.args[i].c_str()),home_dir);
            }
        }
    }
    else {
        pid_t pid=fork();
        if(pid==-1) {
            perror("Unable to fork the shell");
            return 1;
        }
        else if(pid==0) {
            vector<char*> argv;
            for(string arg:cmd.args) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(NULL);
            if(execvp(cmd.args[0].c_str(), argv.data())==-1) {
                perror("Unable to execute the command");
            }
        }
        else {
            if(cmd.background) {
                cout << "Background process started with PID: "<<pid<<endl;

            }
            else {
                int state;
                waitpid(pid, &state, WUNTRACED);
            }
        }
        return 1;
    }
    return 1;
}