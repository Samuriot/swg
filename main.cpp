#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void query_command();
std::vector<std::string> parse_command(const std::string& str);
int exec_command();

int main(int argc, char** argv) {
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
    if(!std::getline(std::cin, prompt))
      break;
    if(prompt == "") 
      continue;
    std::vector<std::string> tokens = parse_command(prompt);
  }
}

std::vector<std::string> parse_command(const std::string& str) {
  std::istringstream parser(str);
  std::string token;

  std::vector<std::string> tokens;
  while(parser >> token) {
    tokens.push_back(token);
  } 

  return tokens;
}

int exec_command() {
  return 0;
}
