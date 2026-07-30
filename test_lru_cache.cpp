#include <iostream>
#include <cassert>
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

    // Test-only accessors
    Node* _testOnlyGetHead() { return list.getHead(); }
    Node* _testOnlyGetTail() { return list.getTail(); }
    size_t _testOnlySize() { return map.size(); }
};

int main() {
    // --- Test 1: basic put and get ---
    LRUCache cache(3);
    cache.put("A", 1);
    cache.put("B", 2);
    cache.put("C", 3);

    int result;
    assert(cache.get("A", result) == true);
    assert(result == 1);
    assert(cache.get("B", result) == true);
    assert(result == 2);
    assert(cache.get("C", result) == true);
    assert(result == 3);
    std::cout << "Test 1 passed: basic put/get for multiple keys" << std::endl;

    // After the gets above, recency order is C -> B -> A (C most recent, from the last get)
    assert(cache._testOnlyGetHead()->key == "C");
    assert(cache._testOnlyGetTail()->key == "A");
    std::cout << "Test 2 passed: get() correctly refreshes recency order" << std::endl;

    // --- Test 3: eviction when capacity is exceeded ---
    // Cache is full (A, B, C; capacity 3). Current order: C -> B -> A (A is LRU)
    cache.put("D", 4);  // should evict A, the current tail

    int evictedCheck;
    assert(cache.get("A", evictedCheck) == false);  // A should be gone
    assert(cache._testOnlySize() == 3);              // size stays at capacity
    std::cout << "Test 3 passed: eviction removes the least-recently-used entry" << std::endl;

    // --- Test 4: getting a missing key ---
    int missing = 777;
    assert(cache.get("Z", missing) == false);
    assert(missing == 777);  // untouched
    std::cout << "Test 4 passed: get() on a missing key returns false" << std::endl;

    // --- Test 5: updating an existing key's value without exceeding capacity ---
    // Current keys: D, C, B (A was evicted). Update B's value.
    cache.put("B", 999);
    int updated;
    assert(cache.get("B", updated) == true);
    assert(updated == 999);
    assert(cache._testOnlySize() == 3);  // size unchanged — this was an update, not an insert
    std::cout << "Test 5 passed: updating an existing key does not evict or grow the cache" << std::endl;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}