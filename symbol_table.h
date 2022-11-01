#include "data_structures/hash_table.h"
#include <fstream>
#include <set>

class SymbolTable{

private:

    // Hash Table used to store (token,code) pairs
    HashTable* hash_table;
    // Current code to assign to new (token,code) entry
    int current_code;

public:
    SymbolTable(){
        current_code = 0;
        this->hash_table = new HashTable(11);
    }

    // add method - add a new (token,code) entry, with current_code code
    int add(std::string token, EntryType type){
        std::optional<int> token_id = hash_table->search(token);
        if(!token_id.has_value()){
            this->hash_table->insert(token, current_code, type);
            return current_code++;
        }
        return token_id.value();
    }

    // add method - add a new (token,code) entry, with custom code
    void add(std::string token, int code, EntryType type){
        this->hash_table->insert(token, code, type);
    }

    // search method - search a (token,code) by token
    // returns: optional<int> - std::nullopt if entry with token is not found, 
    //                          code value of entry otherwise
    std::optional<int> search(std::string token){
        return this->hash_table->search(token);
    }

    operator std::string() const {
        return std::string(*hash_table);
    }

};