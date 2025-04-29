#ifndef LIST_H
#define LIST_H
#include "ListNode.h"

class List
{
public:
    List();

    ~List();
    ListNode *head;
    ListNode* getListByPos(int pos);
    void show();
    void insertList(int id);
};



#endif
