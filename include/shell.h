// engine.h
#ifndef SHELL_H 
#define SHELL_H

#include <deque>
#include <string>
#include <vector>

class Shell {
  private:
    std::deque<std::string> cmd_history;
    int last_status;

    // helper methods
    int exec_command(const std::vector<std::string>& v);
    bool run_builtin(const std::vector<std::string>& tokens);
    std::vector<std::string> parse_command(const std::string& cmd);
  public:
    Shell();
    ~Shell();
    void run();
};

#endif // SHELL_H
