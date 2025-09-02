#include "shell.h"

using namespace std;

stack<string> dir_st;
string last_wd;

void shell_loop() {
    char* input_line;
    vector<string> commands;
    sysInfo_t sys_info=get_system_info();
    string home_dir(sys_info.cwd);
    last_wd=home_dir;
    string cwd;
    int status=1;
    rl_bind_key('\t', handle_tab_stroke);
    
    do {
        sys_info=get_system_info();
        if(home_dir==sys_info.cwd) cwd="~";
        else cwd=sys_info.cwd;
        string username=sys_info.username;
        string shell_display="<"+username+"@"+sys_info.hostname+":"+cwd+">";
        input_line=readline(shell_display.c_str());
        if(input_line==nullptr) {
            exit(0);
        }
        string input_string(input_line);
        if(!input_string.empty()) {
            vector<command_t> commands = parse_line(input_string);
            for(command_t command: commands) {
                sys_info=get_system_info();
                status=builtin_execute(command, home_dir, sys_info.cwd, last_wd);
            }
        }
        else status=1;
        
    }while(status);
}

int main() {
    shell_loop();

    return 0;
}