#include "shell.h"

using namespace std;

stack<string> dir_st;
string last_wd;

void shell_loop() {
    char* input_string;
    vector<string> commands;
    sysInfo_t sys_info=get_system_info();
    string home_dir(sys_info.cwd);
    last_wd=home_dir;
    string cwd;
    int status;
    rl_bind_key('\t', handle_tab_stroke);
    
    do {
        sys_info=get_system_info();
        if(home_dir==sys_info.cwd) cwd="~";
        else cwd=sys_info.cwd;
        string username=sys_info.username;
        string shell_display="<"+username+"@"+sys_info.hostname+":"+cwd+">";
        input_string=readline(shell_display.c_str());
        if(input_string==nullptr) {
            exit(0);
        }
        // cout<<"<"<<sys_info.username<<"@"<<sys_info.hostname<<":"<<cwd<<">";
        // if(!getline(cin, input_string)) {
        //     cout<<endl;
        //     exit(0);
        // }
        if(!input_string) {
            stringstream ss(input_string);
            commands=parse_input(ss);
            status=builtin_execute(commands, home_dir, sys_info.cwd, last_wd);
        }
        else status=1;
        
    }while(status);
}

int main() {
    shell_loop();

    return 0;
}