#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

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
        node->prev = nullptr;
        if (head == nullptr) {
            node->next = nullptr;
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

class LRUCache {
private:
    int capacity;
    std::unordered_map<std::string, Node*> map;
    DoublyLinkedList list;

public:
    LRUCache(int cap) : capacity(cap) {}

    bool get(std::string key, int& value) {
        auto it = map.find(key);
        if (it == map.end()) {
            return false;
        }
        Node* node = it->second;
        list.moveToFront(node);
        value = node->value;
        return true;
    }

    void put(std::string key, int value) {
        auto it = map.find(key);

        if (it != map.end()) {
            Node* node = it->second;
            node->value = value;
            list.moveToFront(node);
            return;
        }

        if (map.size() >= (size_t)capacity) {
            Node* lru = list.getTail();
            map.erase(lru->key);
            list.removeNode(lru);
            delete lru;
        }

        Node* newNode = new Node(key, value);
        list.insertAtFront(newNode);
        map[key] = newNode;
    }

    bool del(std::string key) {
        auto it = map.find(key);
        if (it == map.end()) {
            return false;
        }
        Node* node = it->second;
        list.removeNode(node);
        map.erase(key);
        delete node;
        return true;
    }
};

int main() {
    LRUCache cache(3);

    std::cout << "LRU Key-Value Store (capacity 3). Commands: SET key value | GET key | DEL key | EXIT" << std::endl;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "SET") {
            std::string key;
            int value;
            if (iss >> key >> value) {
                cache.put(key, value);
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "ERROR: usage is SET key value" << std::endl;
            }
        } else if (command == "GET") {
            std::string key;
            if (iss >> key) {
                int value;
                if (cache.get(key, value)) {
                    std::cout << value << std::endl;
                } else {
                    std::cout << "(not found)" << std::endl;
                }
            } else {
                std::cout << "ERROR: usage is GET key" << std::endl;
            }
        } else if (command == "DEL") {
            std::string key;
            if (iss >> key) {
                bool removed = cache.del(key);
                std::cout << (removed ? "OK" : "(not found)") << std::endl;
            } else {
                std::cout << "ERROR: usage is DEL key" << std::endl;
            }
        } else if (command == "EXIT") {
            break;
        } else if (command.empty()) {
            // ignore blank lines
        } else {
            std::cout << "ERROR: unknown command '" << command << "'" << std::endl;
        }
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;
}