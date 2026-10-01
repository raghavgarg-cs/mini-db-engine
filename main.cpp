#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdint>

struct row {
    int id;
    char username[32];
    char email[255];
};
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROW_SIZE = sizeof(row);
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;

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
    row r1 ={};
    if (command.length()>=6 && (command.substr(0,6) == "insert" || command.substr(0,6) == "INSERT")) {
        std::istringstream  stream(command);
        std::string temp_user,temp_email;
        std::string dummy_keyword;
        stream >> dummy_keyword >>r1.id>>temp_user>>temp_email;
        strncpy(r1.username,temp_user.c_str(),sizeof(r1.username)-1);
        strncpy(r1.email,temp_email.c_str(),sizeof(r1.email)-1);
        table.push_back(r1);
        std::ofstream outfile("databse.bin", std::ios::app | std::ios::binary);
        outfile.write(reinterpret_cast<char*>(&r1),sizeof(row));
        std::cout<<"Success! Parsed Id: "<<r1.id<<" Name: "<<r1.username<<" Email: "<<r1.email<<std::endl;
        outfile.close();
        return 0;
    }
    else if (command.length()>=6 && (command.substr(0,6)=="select" || command.substr(0,6) == "SELECT")) {
        for (row r : table) {
            std::cout<<"Id: "<<r.id<<" Name: "<<r.username<<" Email: "<<r.email<<std::endl;
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
    std::cout<<ROW_SIZE<<std::endl;
    std::cout<<ROWS_PER_PAGE<<std::endl;
    std::vector<row> table;
    std::ifstream infile("databse.bin",std::ios::binary);
    row temp_row;
    while (infile.read(reinterpret_cast<char*>(&temp_row),sizeof(row))) {
        table.push_back(temp_row);
    }
    infile.close();
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