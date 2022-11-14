#include<string>
#include<iostream>
#include<vector>
#include <sstream>
#include <string>
#include <optional>

// Enum used to identify if hash map entry is identifier/constant
enum EntryType {
    IDENTIFIER,
    CONSTANT,
    TOKEN
};

// Entry - hash map entry 
// key : string - key of hash map entry
// value :  int - value of hash map entry
// type : EntryType - type of hash map entry 
struct Entry{
    std::string key;
    int value;
    EntryType type;
    Entry(std::string k, int v, EntryType t): key(k), value(v), type(t) {}
};

// LLItem - util struct used for Separate Chaining Hash Table implementation
//  entry : Entry* - pointer to corresponding hash map entry
//  next - LLItem* - pointer to the next linked list bucket item
struct LLItem{
    Entry* entry;
    LLItem* next;
    LLItem(Entry* entry) : entry(entry), next(nullptr){}
};

class HashTable{

private:
    LLItem** linked_list_array;
public:
    int size;
    int cap;
    
    // HashMap Constructor
    // c : int - initial capacity of hash map
    HashTable(int c){
        cap = next_prime(c);
        size = 0;
        linked_list_array = new LLItem*[cap]();
    }

    // util function to check if num is prime
    bool is_prime(int num){
        if(num == 1) return false;
        for(int i = 2;i * i <= num; i++){
            if(num % i == 0) return false;
        }
        return true;
    }

    // finds the next prime number after num
    int next_prime(int num){
        while(!is_prime(num)){
            num++;
        }
        return num;
    }

    // Hash Map Resize function - this method resizes the load factor of the hash map reaches a certain critical value
    // all items are stored in an auxiliary data structure, memory is properlly deallocated,
    // and the number of buckets is chosen as the next prime number after the doulbe of the current capacity.
    // after this, all the elements in the hash map are added again to the hash map
    void resize(){
        std::vector<Entry*> aux;
        for(int i = 0; i < cap; i++){
            LLItem* head = linked_list_array[i];
            LLItem* prev;
            while(head != nullptr){
                aux.push_back(head->entry);
                prev = head;
                head = head->next;
                delete prev;
            }
        }

        delete linked_list_array;
        cap = next_prime(2 * cap);

        linked_list_array = new LLItem*[cap]();

        for(auto e: aux){
            _insert(e->key, e->value, e->type, false);
        }
    }


    // Hash Map Insert - inserts a (key,value) pair into the hash map, with a specific Entry type (identifier/constant)
    // key : string - key of the hash map entry that needs to be inserted
    // value : int - value of the hash map entry that needs to be inserted
    // type : EntryType - type of the hash map entry that needs to be inserted
    void insert(std::string key, int value, EntryType type){
        _insert(key, value, type, false);
    }

    void _insert(std::string key, int value, EntryType type, bool with_resize){
        int hashed_pos = poly_hash(key, this->cap);
        size++;
        LLItem* currentBucket = linked_list_array[hashed_pos];

        if(currentBucket == nullptr){
            linked_list_array[hashed_pos] = new LLItem(new Entry(key, value, type));
            return;
        }

        bool found_key = false;

        while(currentBucket->next != nullptr){
            if(currentBucket->entry->key == key){
                found_key = true;
                currentBucket->entry->value = value;
            }
            currentBucket = currentBucket->next;
        }
        if(!found_key){
            currentBucket->next = new LLItem(new Entry(key, value, type));
        }

        if(size > cap * 0.6 && with_resize){
            resize();
        }
    }

    // Hash Map Search - searches for a (key,value) pair from the hash map. If the
    // key : string - key of the hash map entry that we search for
    // returns : optional<int> - std::nullopt if entry with key is not found, 
    //                           value of entry otherwise
    std::optional<int> search(std::string key){
        int hashed_pos = poly_hash(key, this->cap);
        LLItem* currentBucket = linked_list_array[hashed_pos];

        while(currentBucket != nullptr && currentBucket->entry->key != key){
            currentBucket = currentBucket->next;
        }
        
        if(currentBucket == nullptr){
            return std::nullopt;
        }

        return currentBucket->entry->value;
    }


    // util function for polynomial hash for strings 
    static int poly_hash(std::string value, int cap){
        int p = 53;
        int pi = 1;
        unsigned int hash = 0;
        for(auto c : value){
            hash += c * pi;
            pi *= p; 
        }
        return hash % cap;
    }
    
    operator std::string() const {
        LLItem* current;
        std::stringstream s;
        for(int i = 0; i < cap; i++){
            current = linked_list_array[i];
            if(current == nullptr){
                continue;
            }
            while(current != nullptr){
                s << current->entry->key << " -> " << current->entry->value << "\n";
                current = current->next; 
            }
        }
        return s.str();
    }
};