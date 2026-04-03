#include "shell.h"

#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

Shell::Shell() {
  this->last_status = 0;
}

Shell::~Shell() {
  
}

std::vector<std::string> Shell::parse_command(const std::string& str) 
{
  std::istringstream parser(str);
  std::string token;

  std::vector<std::string> tokens;
  while(parser >> token) {
    tokens.emplace_back(token);
  } 

  return tokens;
}

int Shell::exec_command(const std::vector<std::string>& v)
{
  if(v.empty()) 
    return 0;
  // setup child process
  pid_t pid = fork();

  // case child
  if(pid == 0) {
    // setup a new char* vector for c strings
    std::vector<char*> args;
    args.reserve(v.size() + 1);

    // push content of v into args & convert to c_str
    for(const auto& token : v) 
      args.emplace_back(const_cast<char*>(token.c_str()));

    // null-terminate data
    args.emplace_back(nullptr);
    // execute & exit
    execvp(args[0], args.data());
    perror("execvp");
    _exit(127);
} 

  if(pid < 0) { // case fork failure
    perror("fork");
    return 1;
  }

  // wait for child 
  int status = 0;
  while(waitpid(pid, &status, 0) == -1) {
    if(errno == EINTR)
      continue;
    perror("waitpid");
    return 1;
  }

  if(WIFEXITED(status))
    return WEXITSTATUS(status);

  if(WIFSIGNALED(status)) {
    int sig = WTERMSIG(status);
    return 128 + sig;
  }

  return 0;
}

bool Shell::run_builtin(const std::vector<std::string>& tokens) 
{
  std::string cmd = tokens.at(0);
  if(cmd == "history") {
    for(int i = 0; i < this->cmd_history.size(); i++) {
      std::cout << i << ": " << this->cmd_history.at(i) << std::endl;
    }
  } else 
    return false;
  return true;
}

void Shell::run() 
{
  while(true) {
    std::string prompt;
    std::cout << "$ " << std::flush;
    // handles getline failure (i.e.: ctrl-d out of program)
    if(!std::getline(std::cin, prompt))
      break;
    // continue if you do not type anything
    if(prompt == "") 
      continue;
    std::vector<std::string> tokens = this->parse_command(prompt);
    if(tokens.empty())
      continue;
    if(run_builtin(tokens))
      continue;
    this->last_status = this->exec_command(tokens);
    this->cmd_history.push_back(prompt);
    
    // store the most recent 20 commands
    if(this->cmd_history.size() > 20)
      this->cmd_history.pop_front();
  }
}
