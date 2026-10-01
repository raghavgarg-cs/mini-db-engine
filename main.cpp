#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct row {
    int id;
    std::string username;
    std::string email;
};

int do_meta_command(std::string command) {
    if (command == ".exit") {
        return 1;
    }
    else {
        std::cout<<"Unrecognized Command"<<std::endl;
        return 0;
    }
}
int prepare_statement(std::string command, std::vector<row>& table) {
    row r1;
    if (command.length()>=6 && (command.substr(0,6) == "insert" || command.substr(0,6) == "INSERT")) {
        std::istringstream  stream(command);
        std::string dummy_keyword;
        stream >> dummy_keyword >>r1.id>>r1.username>>r1.email;
        table.push_back(r1);
        std::cout<<"Success! Parsed Id: "<<r1.id<<" Name: "<<r1.username<<" Email: "<<r1.email<<std::endl;
        return 0;
    }
    else if (command.length()>=6 && (command.substr(0,6)=="select" || command.substr(0,6) == "SELECT")) {
        for (row r : table) {
            std::cout<<"Id: "<<r.id<<" Name: "<<r.username<<" Email: "<<r1.email<<std::endl;
        }
        return 0;
    }
    else {
        std::cout<<"Unrecognized Command Detected"<<std::endl;
        return 0;
    }
}

//-----------------------------------------------MAIN--------------------------------------------------------//
int main() {
    std::vector<row> table;
    while (true) {
        std::cout<<"db >  ";
        std::string prompt;
        std::getline(std::cin,prompt);
        if (prompt.empty()) continue;
        else if (prompt[0] == '.') {
            if (do_meta_command(prompt) == 1) break;
        }
        else {
            if (prepare_statement(prompt,table)==0) continue;
        }
    }
}