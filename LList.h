//
// Created by dinoyoshi on 9/9/26.
//

#ifndef LLIST_LLIST_H
#define LLIST_LLIST_H

template <typename T>
class LList {

    /* Necessary features
    * PushFront(Key)
    • PushBack(Key)
    • TopFront()
    • PopFront()
    • TopBack()
    • PopBack()
    • Find(Key)
    • Erase(Key)
    • Empty()
    • AddBefore(Node, Key)
    • AddAfter(Node, Key)
     */

    private:
        int numNodes;
        struct Node {
            T data;
            Node* next;
        };
        Node* head;
    public:

        LList();
        bool PushFront(T);
        bool PushBack(T);
        T TopFront();
        T PopFront();
        T TopBack();
        T PopBack();
        bool Find(T);
        bool Erase(T);
        bool Empty();
        bool AddBefore(Node*, T);
        bool AddAfter(Node*, T);
    };


#endif //LLIST_LLIST_H
