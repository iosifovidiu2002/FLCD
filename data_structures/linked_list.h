#include<sstream>


template <class T>
class LinkedList{

    struct Item{
        Item* next;
        T value;
        Item(T _value): value(_value), next(nullptr){}
    };

    Item* head;
    Item* tail;

public:
    LinkedList(){
        head = nullptr;
        tail = nullptr;
    }

    class Iterator;

    Iterator begin(){
        return Iterator(head);
    }

    Iterator end(){
        return Iterator(nullptr);
    }

    class Iterator
    {
    public:
    Iterator() noexcept :
        current (head) { }
 
    Iterator(const Item* pItem) noexcept :
        current (pItem) { }
 
        Iterator& operator=(Item* pItem)
        {
            this->current = pItem;
            return *this;
        }
 
        Iterator& operator++(){
            if (current)
                current = current->next;
            return *this;
        }
 
        Iterator operator++(int){
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
 
        bool operator!=(const Iterator& iterator)
        {
            return current != iterator.current;
        }
 
        T operator*()
        {
            return current->value;
        }
 
    private:
        const Item* current;
    };

    void insert(T t){
        
        if(head == nullptr){
            head = new Item(t);
            tail = head;
            return;
        }

        tail->next = new Item(t);
        tail = tail->next;

    }

    operator std::string() const{
         Item* current = head;
         std::stringstream s;
         while(current->next != nullptr){
            s << current->value;
            current = current->next;
        }
        return s.str();
    }
};