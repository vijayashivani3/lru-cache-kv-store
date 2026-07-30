#include <iostream>
#include <cassert>
#include <string>

struct Node {
    std::string key;
    int value;
    Node* prev;
    Node* next;

    Node(std::string k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void insertAtFront(Node* node) {
    node->prev = nullptr;   // NEW: always true for the new head, regardless of node's prior state
    if (head == nullptr) {
        node->next = nullptr;   // defensive: correct for a truly empty list either way
        head = node;
        tail = node;
    } else {
        node->next = head;
        head->prev = node;
        head = node;
    }
}

    void removeNode(Node* n) {
        if (n->prev != nullptr) {
            n->prev->next = n->next;
        } else {
            head = n->next;
        }

        if (n->next != nullptr) {
            n->next->prev = n->prev;
        } else {
            tail = n->prev;
        }
    }

    void moveToFront(Node* node) {
        removeNode(node);
        insertAtFront(node);
    }

    Node* getHead() { return head; }
    Node* getTail() { return tail; }
};

int main() {
    // --- insertAtFront tests ---
    DoublyLinkedList list;

    Node* a = new Node("A", 1);
    list.insertAtFront(a);
    assert(list.getHead() == a);
    assert(list.getTail() == a);
    std::cout << "Test 1 passed: insert into empty list" << std::endl;

    Node* b = new Node("B", 2);
    list.insertAtFront(b);
    assert(list.getHead() == b);
    assert(list.getTail() == a);
    std::cout << "Test 2 passed: insert at front of non-empty list" << std::endl;

    Node* c = new Node("C", 3);
    list.insertAtFront(c);
    assert(list.getHead() == c);
    assert(c->next == b);
    assert(b->prev == c);
    std::cout << "Test 3 passed: insert at front with 3 nodes" << std::endl;

    // List: C -> B -> A   (head=C, tail=A)

    // --- moveToFront tests (run BEFORE removeNode tests, so the list is intact) ---

    // Test 4: move the middle node (B) to front
    list.moveToFront(b);
    assert(list.getHead() == b);       // B is now the head
    assert(list.getTail() == a);       // tail unchanged
    assert(b->next == c);              // B -> C
    assert(c->prev == b);              // C's prev is B
    assert(c->next == a);              // C -> A (C now links to A directly)
    assert(a->prev == c);
    std::cout << "Test 4 passed: moveToFront on middle node" << std::endl;

    // List is now: B -> C -> A   (head=B, tail=A)

    // Test 5: move the node that's already at the front (B) — should be a no-op in effect
    list.moveToFront(b);
    assert(list.getHead() == b);
    assert(list.getTail() == a);
    assert(b->next == c);
    assert(c->prev == b);
    std::cout << "Test 5 passed: moveToFront on already-front node" << std::endl;

    // Test 6: move the tail node (A) to front
    list.moveToFront(a);
    assert(list.getHead() == a);       // A is now the head
    assert(list.getTail() == c);       // C is the new tail
    assert(a->next == b);              // A -> B
    assert(b->prev == a);
    assert(b->next == c);              // B -> C (unchanged)
    assert(c->next == nullptr);        // C (new tail) has no next
    std::cout << "Test 6 passed: moveToFront on tail node" << std::endl;

    // List is now: A -> B -> C   (head=A, tail=C)

    // --- removeNode tests ---

    // Test 7: remove the middle node (B)
    list.removeNode(b);
    assert(list.getHead() == a);
    assert(list.getTail() == c);
    assert(a->next == c);
    assert(c->prev == a);
    std::cout << "Test 7 passed: remove middle node" << std::endl;

    // List: A -> C  (head=A, tail=C)

    // Test 8: remove the head (A)
    list.removeNode(a);
    assert(list.getHead() == c);
    assert(list.getTail() == c);
    assert(c->prev == nullptr);
    std::cout << "Test 8 passed: remove head node" << std::endl;

    // Test 9: remove the only remaining node (C)
    list.removeNode(c);
    assert(list.getHead() == nullptr);
    assert(list.getTail() == nullptr);
    std::cout << "Test 9 passed: remove the only node (list becomes empty)" << std::endl;

    std::cout << "All tests passed!" << std::endl;

    delete a;
    delete b;
    delete c;

    return 0;
}