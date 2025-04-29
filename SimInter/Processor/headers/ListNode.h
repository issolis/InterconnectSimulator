#ifndef LISTNODE_H
#define LISTNODE_H
#include "InstructionList.h"

class ListNode
{
private:
    InstructionList *list;
    ListNode * next = nullptr;
public:
    int id; 
    ListNode(InstructionList &list);
    void setNext(ListNode *nextList); 
    ListNode* getNext();
    InstructionList* getList();

};

#endif
