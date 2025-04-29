#include "List.h"
#include "ListNode.h"

List::List(){
    head = nullptr; 
}


ListNode* List::getListByPos(int pos){
    int i = 0; 
    ListNode *currentNode = head; 
    while(i != pos) {
        currentNode = currentNode->getNext(); 
        i++; 
    }
    return currentNode; 
}

void List::show(){
    ListNode *currentNode = head; 
    std::cout << "LISTS" << std::endl;
    while(currentNode != nullptr) {
        std::cout << currentNode->id << std::endl; 
        currentNode = currentNode->getNext(); 

    }
}

void List::insertList(int id){
    if (head == nullptr){
        InstructionList *newList = new InstructionList(); 
        ListNode *newNode = new ListNode(*newList); 
        newNode->id = id; 
        head = newNode;  
    }else{
        InstructionList *newList = new InstructionList(); 
        ListNode *newNode = new ListNode(*newList); 
        newNode->id = id; 
        
        ListNode *currentNode = head; 
        while (currentNode->getNext()!=nullptr) {
            currentNode = currentNode->getNext(); 
        }
        currentNode->setNext(newNode); 
    }
}
