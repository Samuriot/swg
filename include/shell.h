// engine.h
#ifndef SHELL_H 
#define SHELL_H

#include <deque>
#include <string>
#include <vector>

class Shell {
  private:
    void query_command();
    int exec_command();
    bool run_builtin(const std::vector<std::string>& tokens,
                     const std::deque<std::string>& history,
                     int& last_status);
  public:
    Shell();
    ~Shell();
    void run();
}

#endif // SHELL_H
