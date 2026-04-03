#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>

void query_command();
std::vector<std::string> parse_command(const std::string& str);
int exec_command(const std::vector<std::string>& v);

int main() {
  query_command();
  return 0;
}

// basic workflow of shell
// Step 1: Wait for user input
// Step 2: Store user input & Parse Data
// Step 3: Run the commands
void query_command() {
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
    
    exec_command(tokens);
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

int exec_command(const std::vector<std::string>& v) {
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
    _exit(1);

  } else if(pid > 0) { // case parent
    // wait for child 
    int status = 0;
    if(waitpid(pid, &status, 0) == -1)
      perror("waitpid");
  } else // case fork error
    perror("fork");
  
  return 0;
}
