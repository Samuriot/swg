#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

void query_command();
std::vector<std::string> parse_command(const std::string& str);
int exec_command(const std::vector<std::string>& v);
bool run_builtin_command(const std::vector<std::string>& tokens,
                         const std::deque<std::string>& history,
                         int& last_status);

int main() 
{
  query_command();
  return 0;
}

// basic workflow of shell
// Step 1: Wait for user input
// Step 2: Store user input & Parse Data
// Step 3: Run the commands
void query_command() 
{
  std::deque<std::string> cmd_hist;
  int last_status = 0;
  while(true) {
    std::string prompt;
    std::cout << "$ " << std::flush;
    // handles getline failure (i.e.: ctrl-d out of program)
    if(!std::getline(std::cin, prompt))
      break;
    // continue if you do not type anything
    if(prompt == "") 
      continue;
    std::vector<std::string> tokens = parse_command(prompt);
    if(tokens.empty())
      continue;
    if(run_builtin_command(tokens, cmd_hist, last_status))
      continue;
    last_status = exec_command(tokens);
    cmd_hist.push_back(prompt);
    
    // store the most recent 20 commands
    if(cmd_hist.size() > 20)
      cmd_hist.pop_front();
  }
}

std::vector<std::string> parse_command(const std::string& str) {
  std::istringstream parser(str);
  std::string token;

  std::vector<std::string> tokens;
  while(parser >> token) {
    tokens.emplace_back(token);
  } 

  return tokens;
}

int exec_command(const std::vector<std::string>& v) 
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

bool run_builtin_command(const std::vector<std::string>& tokens,
                         const std::deque<std::string>& history,
                         int& last_status) 
{
  std::string cmd = tokens.at(0);
  switch(cmd) {
    case "history":
      for(int i = 0; i < history.size(); i++) {
        std::cout << i << ": " << history.at(i);
      }
      break;
    default:
      return false;
  }
  return true;
}
