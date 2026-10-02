#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <cstdlib>

struct row {
    int id;
    char username[32];
    char email[255];
};
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROW_SIZE = sizeof(row);
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_PAGES = 100;

struct Table {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
};

Table* new_table() {
    Table* table = new Table;
    table->num_rows = 0;
    for (uint32_t i = 0; i<TABLE_MAX_PAGES; i++) {
        table->pages[i] = nullptr;
    }
    return table;
}
void* row_slot(Table* table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if (page == nullptr) {
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return (char*)page + byte_offset;
}

int do_meta_command(std::string command) {
    if (command == ".exit") {
        return 1;
    }
    else {
        std::cout<<"Unrecognized Command"<<std::endl;
        return 0;
    }
}
int prepare_statement(std::string command, Table* table) {
    row r1 ={};
    if (command.length()>=6 && (command.substr(0,6) == "insert" || command.substr(0,6) == "INSERT")) {
        std::istringstream  stream(command);
        std::string temp_user,temp_email;
        std::string dummy_keyword;
        stream >> dummy_keyword >>r1.id>>temp_user>>temp_email;
        strncpy(r1.username,temp_user.c_str(),sizeof(r1.username)-1);
        strncpy(r1.email,temp_email.c_str(),sizeof(r1.email)-1);
        void* destination = row_slot(table, table->num_rows);
        memcpy(destination, &r1, ROW_SIZE);
        table->num_rows++;
        std::ofstream outfile("databse.bin", std::ios::app | std::ios::binary);
        outfile.write(reinterpret_cast<char*>(&r1),sizeof(row));
        std::cout<<"Success! Parsed Id: "<<r1.id<<" Name: "<<r1.username<<" Email: "<<r1.email<<std::endl;
        outfile.close();
        return 0;
    }
    else if (command.length()>=6 && (command.substr(0,6)=="select" || command.substr(0,6) == "SELECT")) {
        for (uint32_t i = 0; i < table->num_rows; i++) {
            row* r = (row*)row_slot(table, i);
            std::cout << "Id: " << r->id << " Name: " << r->username << " Email: " << r->email << std::endl;
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
    Table* table = new_table();
    std::ifstream infile("databse.bin", std::ios::binary);
    if (infile.is_open()) {
        row temp_row;
        while (infile.read(reinterpret_cast<char*>(&temp_row), ROW_SIZE)) {
            // 1. Find the exact memory address for the next row
            void* destination = row_slot(table, table->num_rows);

            // 2. Copy the bytes from the file directly into that RAM slot
            memcpy(destination, &temp_row, ROW_SIZE);

            // 3. Increment the table's row counter
            table->num_rows++;
        }
        infile.close();
    }
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