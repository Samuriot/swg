#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void query_command();
std::vector<std::string> parse_command(std::string& str);

int main(int argc, char** argv) {
  query_command();
  return 0;
}

// basic workflow of shell
// Step 1: Wait for user input
// Step 2: Store user input & Parse Data
// Step 3: Run the commands
void query_command() {
  while(1) {
    std::string prompt;
    std::vector<std::string> tokens;
    std::cout << "$ " << std::flush;
    std::getline(std::cin, prompt);
    if(prompt == "") {
      continue;
    }
    tokens = parse_command(prompt);
    for(auto& token: tokens) {
      std::cout << token << " ";
    }
    std::cout << std::endl;
  }
}

std::vector<std::string> parse_command(std::string& str) {
  std::istringstream parser(str);
  std::string token;

  std::vector<std::string> tokens;
  while(parser >> token) {
    tokens.push_back(token);
  } 

  return tokens;
}
