#include "shell.h"

using namespace std;

string last_wd;
string history_file="shell_history.txt";

void shell_loop() {
    char* input_line;
    vector<string> commands;
    sysInfo_t sys_info=get_system_info();
    string home_dir(sys_info.cwd);
    last_wd=home_dir;
    string cwd;
    int status=1;
    read_history(history_file.c_str());
    rl_attempted_completion_function = handle_tab_stroke;
    signal(SIGINT, SIG_IGN);
    
    do {
        sys_info=get_system_info();
        if(home_dir==sys_info.cwd) cwd="~";
        else {
            cwd=sys_info.cwd;
            size_t last_slash = cwd.find_last_of('/');
            cwd=(last_slash == string::npos) ? "~/"+cwd : "~/"+cwd.substr(last_slash + 1);
        } 
        string username=sys_info.username;
        string empty="";
        string shell_display=LEFT_C+empty+"<"+username+"@"+sys_info.hostname+":"+RIGHT_C+cwd+RESET+">";
        input_line=readline(shell_display.c_str());
        if(input_line==nullptr) {
            break;
        }
        if(input_line && *input_line) {
            add_history(input_line);
            append_history(1, history_file.c_str());
        }
        string input_string(input_line);
        if(!input_string.empty()) {
            vector<string> command_strings = parse_line(input_string);
            for(string cmd_str: command_strings) {
                vector<command_t> commands = parse_pipe(cmd_str);
                size_t count=commands.size();
                if(count>1) {
                    sys_info=get_system_info();
                    status=execute_pipe(commands, home_dir, sys_info.cwd, last_wd);
                }
                else {
                    sys_info=get_system_info();
                    status=builtin_execute(commands[0], home_dir, sys_info.cwd, last_wd);
                }
            }
        }
        else status=1;
        free(input_line);
    } while(status);
    write_history(history_file.c_str());
}

int main() {
    shell_loop();

    return 0;
}