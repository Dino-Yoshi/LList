//
// Created by dinoyoshi on 9/9/26.
//

#include "LList.h"

// Time Complexity: O(1)
template<typename T>
LList<T>::LList() {
    head = nullptr;
    numNodes = 0;
}

// Time Complexity: O(1)
template<typename T>
bool LList<T>::PushFront(T data) {

    // if list empty
    if (this->head == nullptr) {
        this->head = new Node();
        this->head->data = data;
        this->head->next = nullptr;
        numNodes++;
        return true;

    }

    // if list not empty
    Node *temp = this->head;
    this->head = new Node();
    this->head->data = data;
    this->head->next = temp;
    numNodes++;
    return true;

}

// Time Complexity: O(N)
template<typename T>
bool LList<T>::PushBack(T data) {
    // empty case

    if (head == nullptr) {
        this->head = new Node();
        this->head->data = data;
        this->head->next = nullptr;
        numNodes++;
        return true;
    }

    // general case i.e. [4, 5, 7], data is 9

    Node *currNode = head;
    while (currNode->next != nullptr) { // traverse to end
        currNode = currNode->next;
    }
    currNode->next = new Node();
    currNode->next->data = data;
    currNode->next->next = nullptr;
    numNodes++;
    return true;
}

// Time Complexity: O(1)
template<typename T>
T LList<T>::TopFront() {
    if (head != nullptr) {
        return head->data;
    }
    return T();
}

// Time Complexity: O(1)
template<typename T>
T LList<T>::PopFront() {
    if (this->head == nullptr) {
        return T();
    }

    Node *tmp = this->head;
    T tmpDat = this->head->data;
    if (this->head->next == nullptr) {
        head = nullptr;
        delete tmp;
        numNodes--;
        return tmpDat;
    }
    head = tmp->next;
    delete tmp;
    numNodes--;
    return tmpDat;
}

// Time Complexity: O(N)
template<typename T>
T LList<T>::TopBack() {
    if (head == nullptr) {
        return T();
    }

    Node *currNode = head;
    while (currNode->next != nullptr) {
        currNode = currNode->next;
    }
    return currNode->data;
}

// Time Complexity: O(N)
template<typename T>
T LList<T>::PopBack() {
    if (this->head == nullptr) {
        return T();
    }

    // i.e. [0] <-> [1] <-> [3] <-> [4]
    // tail should go to 3, its prev should be 1, its next should be null.
    Node *currNode = head;

    if (currNode->next == nullptr) {
        T tmp = currNode->data;
        delete currNode;
        head = nullptr;
        numNodes--;
        return tmp;
    }

    Node *prevNode = nullptr;
    while (currNode->next != nullptr) {
        prevNode = currNode;
        currNode = currNode->next;
    }

    T tmp = currNode->data;
    prevNode->next = nullptr;
    delete currNode;
    numNodes--;
    return tmp;


}

// Time Complexity: O(N)
template<typename T>
bool LList<T>::Find(T data) {
    Node *currNode = this->head;
    while (currNode != nullptr) {
        if (currNode->data == data) {
            return true;
        }
        currNode = currNode->next;
    }
    return false;
}

// TIme Complexity: O(N)
template<typename T>
bool LList<T>::Erase(T data) {
    // beem

    // empty case
    if (this->head == nullptr) {
        return false;
    }

    Node *currNode = this->head;

    if (currNode->next == nullptr) {
        if (currNode->data == data) {
            head = nullptr;
            delete currNode;
            numNodes--;
            return true;
        }
        return false;
    }

    Node *prevNode = nullptr;
    while (currNode != nullptr) {
        if (currNode->data == data) {
            // Middle Case
            if (prevNode != nullptr && currNode->next != nullptr) {
                prevNode->next = currNode->next;
                numNodes--;
                delete currNode;
                return true;
            }
            // Beginning Case
            if (currNode == head) {
                head = currNode->next;
                if (head == nullptr) {
                    prevNode = nullptr;
                    delete currNode;
                    numNodes--;
                    return true;
                }
                delete currNode;
                numNodes--;
                return true;
            }
            // End Case
            if (currNode->next == nullptr) {
                prevNode->next = nullptr;
                delete currNode;
                numNodes--;
                return true;
            }
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    return false;
}

// Time Complexity: O(1)
template<typename T>
bool LList<T>::Empty() {
    if (head == nullptr) {
        return true;
    }
    return false;
}

// Time Complexity: O(N)
template<typename T>
bool LList<T>::AddBefore(Node * desNode, T data) {
    // empty case
    if (this->head == nullptr) {
        return false;
    }

    Node *prevNode = nullptr;
    Node *currNode = this->head;
    while (currNode != nullptr) {
        if (currNode == desNode) {
            // add logic, bem, reverse erase logic

            // Middle Case
            if (prevNode != nullptr && currNode->next != nullptr) {
                Node *newNode = new Node();
                newNode->data = data; // initialize
                newNode->next = currNode; // should point next to desired node
                prevNode->next = newNode; // desiredNode previous shall now point to the new node behind it
                numNodes++;
                return true;
            }
            // Beginning Case
            if (currNode == head) {
                Node *newNode = new Node();
                newNode->data = data; // data
                newNode->next = currNode; // new Node goes before head.
                head = newNode;
                numNodes++;
                return true;
            }

            // End Case
            if (currNode->next == nullptr) {
                Node *newNode = new Node();
                newNode->data = data;
                prevNode->next = newNode;
                newNode->next = currNode;
                numNodes++;
                return true;
            }

        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    return false;
}


// Time Complexity: O(N)
template<typename T>
bool LList<T>::AddAfter(Node * desNode, T data) {
    // empty case
    if (this->head == nullptr) {
        return false;
    }


    Node *prevNode = nullptr;
    Node *currNode = this->head;

    while (currNode != nullptr) {
        if (currNode == desNode) {
            // add logic, bem, reverse erase logic

            // Middle Case
            if (prevNode != nullptr && currNode->next != nullptr) {
                Node *newNode = new Node();
                newNode->data = data;
                newNode->next = currNode->next;
                currNode->next = newNode;
                numNodes++;
                return true;
            }

            // Beginning Case
            if (currNode == head) {
                Node *newNode = new Node();
                newNode->data = data;
                newNode->next = currNode->next;
                currNode->next = newNode;
                numNodes++;
                return true;
            }

            // End Case
            if (currNode->next == nullptr) {
                Node *newNode = new Node();
                newNode->data = data;
                currNode->next = newNode;
                newNode->next = nullptr;
                numNodes++;
                return true;
            }
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    return false;
}
