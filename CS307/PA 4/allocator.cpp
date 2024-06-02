#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <mutex>
#include <unistd.h>
using namespace std;


struct node
{
    int ID;
    int size;
    int index;
    node * next;
    //default constructor.
    node()
    {
        this->ID = -1;

    }

    //parametric constructor.
    node(int i, int s, int idx, node * p):ID(i), size(s), index(idx), next(p)
    {};
};



class HeapManager
{

    private:
        node * head;
        int heapSize;
        mutex mtx;
    public:
        HeapManager();
        ~HeapManager();
        int initHeap(int size);
        int myMalloc(int ID, int size);
        int myFree(int ID, int index);
        void print();
};

//constructor
HeapManager::HeapManager()
{
    this->head = NULL;
}

HeapManager::~HeapManager()
{
    //destructor
    node* current = head;
    while(current != nullptr) {
        node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

int HeapManager::initHeap(int size)
{
    if(head == NULL)
    {
        //if we have no head, then do this:
        head = new node(-1,size,0,NULL); // we make thread id = -1, since its free
        heapSize = size;
        print();
    }
    return 1;//assumed will always succeed.
}

int HeapManager::myMalloc(int ID, int size) {
    mtx.lock();
    node* current = head;
    node* newBlock = nullptr;
    int retIndex = -1;
    
    while (current != nullptr) {
        if (current->ID == -1 && current->size >= size) {
            if (current->size == size) {
                current->ID = ID;
                retIndex = current->index;
            } else {
                newBlock = new node(ID, size, current->index, new node(-1, current->size - size, current->index + size, current->next));
                if (current == head) {
                    head = newBlock;
                } else {
                    node* prev = head;
                    while (prev->next != current) {
                        prev = prev->next;
                    }
                    prev->next = newBlock;
                }
                retIndex = newBlock->index;
                delete current;
            }
            break;
        }
        current = current->next;
    }
    if(retIndex != -1)
    {
        cout << "Allocated for thread " << ID<<endl;
        this->print();
    }
    mtx.unlock();
    return retIndex;
}
int HeapManager::myFree(int ID, int index) {
    mtx.lock();
    node* current = head;
    node* prev = nullptr;

    while (current != nullptr) {
        if (current->ID == ID && current->index == index) {
            current->ID = -1;
            if (current->next != nullptr && current->next->ID == -1) {
                node* nextNode = current->next;
                current->size += nextNode->size;
                current->next = nextNode->next;
                delete nextNode;
            }
            if (prev != nullptr && prev->ID == -1) {
                prev->size += current->size;
                prev->next = current->next;
                delete current;
            }
            cout << "Freed for thread " << ID<<endl;
            this->print();
            mtx.unlock();
            return 1;
        }
        prev = current;
        current = current->next;
    }

    mtx.unlock();
    return -1;
}
void HeapManager::print()
{
    //will print the correct output
    node * temp = head;
    while(temp != NULL)
    {
        cout << "[" << temp->ID << "]" << "[" << temp->size <<"]" << "[" <<temp->index<<"]";
        if(temp->next != NULL)
        {
            cout <<"---";
        }
        temp = temp->next;
    }
    cout << endl;
}

#endif