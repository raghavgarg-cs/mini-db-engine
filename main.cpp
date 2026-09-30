#include <iostream>
#include <string>

int do_meta_command(std::string command) {
    if (command == ".exit") {
        return 1;
    }
    else {
        std::cout<<"Unrecognized Command"<<std::endl;
        return 0;
    }
}
int prepare_statement(std::string command) {
    std::cout<<"SQL statement recognized, but execution is not built yet"<<std::endl;
    return 0;
}


int main() {
    while (true) {
        std::cout<<"db > ";
        std::string prompt;
        std::getline(std::cin,prompt);
        if (prompt.empty()) continue;
        else if (prompt[0] == '.') {
            if (do_meta_command(prompt) == 1) break;
        }
        else {
            if (prepare_statement(prompt)==0) continue;
        }
    }
}