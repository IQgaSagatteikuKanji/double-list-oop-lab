#pragma once

template<class Type>
struct Node{
    Type data;
    Node<Type>*   next = nullptr,
                *prev = nullptr;
};


template<class Type>
    class iterator;

template<class Type>
class Double_list{
private:
    Node<Type>  *head = nullptr,
                *tail = nullptr;
                //was only used for debugging
    void recursive_front_delete(Node<Type>*);
    void put_node_to_front_without_deletion(Node<Type> *);

public:

friend class iterator<Type>;

    Double_list(){};
    Double_list(const Double_list<Type>&);
    Double_list& operator=(const Double_list<Type>&);
    ~Double_list();

    bool is_empty(){return head == nullptr;};
    void clear();
    bool push_front(const Type&);
    bool push_back(const Type&);
    bool pop_front(Type&);
    bool pop_back(Type&);

    iterator<Type> search(const Type&);
    iterator<Type> beg();
    iterator<Type> end();
};


template<class Type>
    class iterator{
        Node<Type>* ptr = nullptr;
        Double_list<Type>* list = nullptr;
    public:
    friend class Double_list<Type>;
        iterator operator++(int);
        iterator& operator++();
        iterator operator--(int);
        iterator& operator--();
        Type& operator*();
        bool is_empty();
        bool add_after_node(const Type &);
        bool add_before_node(const Type&);
        void delete_node();

        bool operator==(const iterator&);
        bool operator!=(const iterator&);
    };


template<class Type>
void Double_list<Type>::put_node_to_front_without_deletion(Node<Type>* ptr){
    if(head != ptr){
        ptr->prev->next = ptr->next;
        if(ptr->next != nullptr)
            ptr->next->prev = ptr->prev;
        
        head->prev = ptr;
        ptr->prev = nullptr;
        ptr->next = head;

        head=ptr;
    }
}
template<class Type>
void Double_list<Type>::recursive_front_delete(Node<Type>* ptr){
    if(ptr->next != nullptr)
        recursive_front_delete(ptr->next);
    delete ptr;
}



template<class Type>
bool Double_list<Type>::pop_front(Type& value){
    bool errors_ocurred = true;
    iterator<Type> iter = beg();
    if(!iter.is_empty()){
        errors_ocurred = false;
        value = *iter;
        iter.delete_node();
    }
    return errors_ocurred;
}

template<class Type>
bool Double_list<Type>::pop_back(Type& value){
    bool errors_ocurred = true;
    iterator<Type> iter = end();
    if(!iter.is_empty()){
        errors_ocurred = false;
        value = *iter;
        iter.delete_node();
    }
    return errors_ocurred;
}


template<class Type>
Double_list<Type>::~Double_list(){
    clear();
}
template<class Type>
Double_list<Type>::Double_list(const Double_list<Type>& src){
    iterator<Type> src_iter = src.beg();
    while(!src_iter.is_empty()){
        push_back(*src_iter);
        src_iter++;
    }
}

template<class Type>
Double_list<Type>& Double_list<Type>::operator=(const Double_list<Type>&src){
    if(&src == this) return *this;
    clear();
    iterator<Type> src_iter = src.beg();
    while(!src_iter.is_empty()){
        push_back(*src_iter);
        src_iter++;
    }
    return *this;
}

template<class Type>
iterator<Type> Double_list<Type>::search(const Type& value){
    iterator<Type> iter = beg();
    bool not_found = true;

    while(!iter.is_empty() && not_found){
        if(*iter == value) not_found =false;
        else ++iter;
    }

    if(head != iter.ptr){
        put_node_to_front_without_deletion(iter.ptr);
    }
    return iter;
}

template<class Type>
void Double_list<Type>::clear(){
    iterator<Type> iter = beg();
    while(!iter.is_empty()){
        iter.delete_node();
    }
    // if(head != nullptr)
    //     recursive_front_delete(head);
    // head = nullptr;
    // tail = nullptr;
}

template<class Type>
bool Double_list<Type>::push_front(const Type& value){
    bool errors_ocurred = false;
    if(is_empty()){
        head = new Node<Type>;
        head->data = value;
        tail = head; 
    } else{
        iterator<Type> iter = beg();
        errors_ocurred = iter.add_before_node(value);
    } 
    return errors_ocurred;
}

template<class Type>
bool Double_list<Type>::push_back(const Type& value){
    bool errors_ocurred = false;
    if(is_empty()){
        head = new Node<Type>;
        head->data = value;
        tail = head;
    } else{
        iterator<Type> iter = end();
        errors_ocurred = iter.add_after_node(value);
    }
    return value;
}

template<class Type>
iterator<Type> Double_list<Type>::beg(){
    iterator<Type> iter;
    iter.ptr = head;
    iter.list = this;
    return iter;
}

template<class Type>
iterator<Type> Double_list<Type>::end(){
    iterator<Type> iter;
    iter.ptr = tail;
    iter.list = this;
    return iter;
};

template<class Type>
bool iterator<Type>::add_before_node(const Type& value){
    bool errors_ocurred = is_empty();
    
    if(!errors_ocurred){
        Node<Type> * holder = ptr->prev;
        ptr->prev = new Node<Type>;

        ptr->prev->data = value;
        ptr->prev->prev = holder;
        ptr->prev->next = ptr;
        if(holder != nullptr){
            holder->next = ptr->prev;
        }
    }
    if(list->head == ptr){
        list->head = ptr->prev;
    }
    return errors_ocurred;
}

template<class Type>
bool iterator<Type>::add_after_node(const Type& value){
    bool errors_ocurred = is_empty();

    if(!errors_ocurred){
        Node<Type> * holder = ptr->next;
        ptr->next = new Node<Type>;

        ptr->next->data = value;
        ptr->next->next = holder;
        ptr->next->prev = ptr;
        if(holder != nullptr){
            holder->prev = ptr->next;
        }
    }

    if(list->tail == ptr){
        list->tail = ptr->next;
    }

    return errors_ocurred;
}

template<class Type>
void iterator<Type>::delete_node(){
    if(!is_empty()){
        Node<Type> *new_ptr = nullptr;

        if(ptr->next != nullptr){
            ptr->next->prev = ptr->prev;
            new_ptr = ptr->next;
        } else{
            list->tail = ptr->prev;
            if(list->tail == nullptr)
                list->head = nullptr;
        }

        if(ptr->prev != nullptr){
            ptr->prev->next = ptr->next;
            new_ptr =ptr->prev;
        } else{
            list->head = ptr->next;
            if(list->head == nullptr)
                list->tail = nullptr;
        }
        
        delete ptr;
        ptr = new_ptr;
    }
}

template<class Type>
Type& iterator<Type>::operator*(){
    return ptr->data;
}

template<class Type>
bool iterator<Type>::is_empty(){
    return ptr == nullptr;
}

template<class Type>
bool iterator<Type>::operator!=(const iterator<Type>& other){
    return ptr != other.ptr;
}

template<class Type>
bool iterator<Type>::operator==(const iterator<Type>& other){
    return ptr == other.ptr;
}

template<class Type>
iterator<Type>& iterator<Type>::operator++(){
    ptr = ptr->next;
    return *this;
}

template<class Type>
iterator<Type>& iterator<Type>::operator--(){
    ptr = ptr->prev;
    return *this;
}

template<class Type>
iterator<Type> iterator<Type>::operator++(int){
    iterator holder = *this;
    ptr= ptr->next;
    return holder;
}

template<class Type>
iterator<Type> iterator<Type>::operator--(int){
    iterator holder = *this;
    ptr = ptr->prev;
    return holder;
}
