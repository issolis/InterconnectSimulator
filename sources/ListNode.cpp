#include "ListNode.h"


ListNode::ListNode(InstructionList &list){
    this->list = &list;
    id = 0; 
    next = nullptr;
    
}

void ListNode::setNext(ListNode *nextList)
{
    this->next = nextList;
}

ListNode* ListNode::getNext(){
    return next; 
}

InstructionList *ListNode::getList(){
    return list;
}
