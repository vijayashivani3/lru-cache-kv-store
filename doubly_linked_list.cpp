#include <iostream>
#include <string>

struct Node {
    std::string key;
    int value;
    Node* prev;
    Node* next;

    Node(std::string k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

int main() {
    Node* n = new Node("apple", 42);
    std::cout << "key: " << n->key << ", value: " << n->value << std::endl;
    std::cout << "prev is null: " << (n->prev == nullptr) << std::endl;
    std::cout << "next is null: " << (n->next == nullptr) << std::endl;
    delete n;
    return 0;
}