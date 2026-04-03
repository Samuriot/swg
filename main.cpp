#include <iostream>
#include <string>

void query_command();

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
    std::cout << "$ " << std::flush;
    std::cin >> prompt;
    std::cout << prompt << std::endl;
  }
}
