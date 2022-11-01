#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <regex>
#include <exception>
#include <utility>
#include "symbol_table.h"
#include "data_structures/linked_list.h"

class Scanner {

public: 

    HashTable* tokens;
    SymbolTable* identifier_constant_sym_table;
    // First - the id from the tokens table
    // Second - the id from the symbol table or -1
    LinkedList<std::pair<std::string, std::pair<int, int>>>* pif;

    Scanner(std::string tokens_path){
        tokens = new HashTable(11);
        identifier_constant_sym_table = new SymbolTable();
        pif = new LinkedList<std::pair<std::string, std::pair<int, int>>>();
        fetch_tokens(tokens_path);
    }

    void fetch_tokens(std::string tokens_path){
        std::string content;
        std::ifstream f(tokens_path.c_str());
        std::string line;
        int token_count = 0;
        while (std::getline(f, line)) {
            tokens->insert(line, token_count++, TOKEN);
        }
    }

    EntryType ident_const_token_classifier(std::string token){
        if(std::regex_match(token, std::regex("(_)*[a-zA-Z]+[a-zA-Z0-9]*"))){
            return IDENTIFIER;
        } else if(std::regex_match(token, std::regex("'.'")) || std::regex_match(token, std::regex("([+-]?[1-9][0-9]*)|0"))) {
            return CONSTANT;
        } else {
            throw std::runtime_error(("Lexical error " + token).c_str());
        }
    } 

    

    void scan(std::string file_path){
        std::string content;
        std::ifstream f(file_path.c_str());
        getline(f, content, char(-1));
        f.close();
        char c;
        bool is_token;
        bool is_current_token = false;
        std::string current_token = "";
        std::set<std::string> ident_const;
        std::set<std::string> program_tokens;
        signed int current_line = 1;
        std::string current_separator = "";
        int i;
        try{
            for(i = 0; i < content.length(); i++){
                c = content[i];
                if (c == '\n'){
                    current_line++;
                    continue;
                }

                is_token = tokens->search(std::string(1,c)).has_value();

                if(is_token){
                    std::optional<int> token_id = tokens->search(current_token);
                    is_current_token = token_id.has_value();

                    if(current_token != "" && !is_current_token){
                        EntryType current_token_type = ident_const_token_classifier(current_token);
                        int token_id = identifier_constant_sym_table->add(current_token, current_token_type);
                        switch(current_token_type){
                            case IDENTIFIER:
                                pif->insert({current_token, {tokens->search("ident").value(), token_id}});
                                break;
                            case CONSTANT:
                                pif->insert({current_token, {tokens->search("const").value(), token_id}});
                                break;
                        }
                    }

                    if (c == '\''){ 
                        std::string possible_const_char  = "'" + std::string(1,content[i+1]) + "'";
                        if(ident_const_token_classifier(possible_const_char) == CONSTANT){
                            i += 2;
                            int const_id = identifier_constant_sym_table->add(possible_const_char, CONSTANT);
                            pif->insert({possible_const_char, {tokens->search("const").value(), const_id}});
                        }
                    }

                    if (current_token != "" && is_current_token){
                        pif->insert({current_token, {token_id.value(), -1}}); 
                        // program_tokens.insert(current_token);
                    }

                                        current_separator = std::string(1,c);
                    if ((c == '<' || c == '>' || c == '=' || c == '!') && content[i+1] == '='){
                        current_separator += content[i+1];
                        i++;
                    }

                    if (current_separator != " " && current_separator != "'"){
                        std::optional<int> separator_id = tokens->search(current_separator);
                        // Add the current separator to the PIF List
                        if (separator_id.has_value()){
                            pif->insert({current_separator, {separator_id.value(), -1}});
                        }
                    }

                    current_token = "";
                } else {
                    current_token += c;
                }
            }
        } catch(std::runtime_error& e){
            std::cout << e.what() << " on line " << current_line << std::endl;
        }

        std::cout << std::string(*identifier_constant_sym_table);
        std::cout << "----------------" << std::endl;
        std::cout << std::string(*tokens);
        std::cout << "----------------" << std::endl;

        for ( LinkedList<std::pair<std::string, std::pair<int, int>>>::Iterator iterator = (*pif).begin(); iterator != (*pif).end(); iterator++){
            auto p = * iterator;
            std::cout << p.first << " {" << p.second.first << ", " << p.second.second << "}" << std::endl;
        }
    }

};