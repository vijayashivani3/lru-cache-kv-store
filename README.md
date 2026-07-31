# LRU Cache / Mini Key-Value Store (C++)

A Least Recently Used (LRU) cache built from scratch in C++, wrapped in an
interactive command-line key-value store. The core data structure — a
doubly linked list combined with a hash map — is hand-built (no
`std::list`) to demonstrate pointer manipulation and O(1) design.

## Features

- **O(1) `get`, `put`, and eviction** — a hash map (`std::unordered_map`)
  maps keys directly to nodes in a hand-built doubly linked list, so no
  operation ever needs to scan the cache.
- **Hand-built doubly linked list** — `insertAtFront`, `removeNode`, and
  `moveToFront` are implemented from raw pointers, not `std::list`.
- **Fixed-capacity cache** with automatic eviction of the least-recently-used
  entry when full.
- **Interactive CLI** supporting `SET`, `GET`, and `DEL` commands.
- **Assert-based automated tests** for the linked list and cache logic,
  covering basic operations, eviction, recency refresh, and edge cases.

## How it works

- `Node` — stores a key, value, and `prev`/`next` pointers.
- `DoublyLinkedList` — maintains recency order: most-recently-used at the
  head, least-recently-used at the tail.
- `LRUCache` — combines a `std::unordered_map<string, Node*>` (for O(1)
  lookup) with a `DoublyLinkedList` (for O(1) recency tracking and
  eviction) via composition.

## Building and running

Requires a C++ compiler (developed and tested with g++/MinGW-w64 on
Windows).

g++ main.cpp -o main.exe -mconsole
./main.exe

## Example session:


SET apple 5

OK

GET apple

5

SET banana 10

OK

SET cherry 15

OK

SET date 20

OK

GET banana

(not found)

## Running the tests
g++ test_dll.cpp -o test_dll.exe -mconsole
./test_dll.exe

g++ test_lru_cache.cpp -o test_lru_cache.exe -mconsole
./test_lru_cache.exe

## Design notes

- The linked list is hand-built rather than using `std::list`, since the
  pointer manipulation is the point of the exercise for an LRU cache
  interview question — using a built-in list would sidestep exactly what
  the question is testing.
- The hash map uses the STL (`std::unordered_map`), since hash table
  internals are a separate concern from LRU cache design.
- `LRUCache` uses composition (holds a `DoublyLinkedList`) rather than
  inheritance, so external code can only interact with the cache through
  `get`/`put`/`del` — never able to bypass the hash map and corrupt the
  cache's internal consistency.
