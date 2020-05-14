#pragma once

#include "tablet-tools/uefi-bootengine/std/move.hpp"
#include "tablet-tools/uefi-bootengine/std/vector.hpp"

namespace tablet_tools::uefi_bootengine::std {
    template <typename KeyType, typename ValueType>
    class HashNode {
    private:
        KeyType key;
        ValueType value;
        HashNode *next_node;

    public:
        explicit HashNode(KeyType key, ValueType value)
            : key(std::move(key)), value(std::move(value)), next_node(nullptr) {}

        KeyType &get_key() {
            return key;
        }

        const KeyType &get_key() const {
            return key;
        }

        ValueType &get_value() {
            return value;
        }

        const ValueType &get_value() const {
            return value;
        }

        HashNode *get_next_node() {
            return next_node;
        }

        void set_next_node(HashNode *new_next_node) {
            next_node = new_next_node;
        }

        void set_value(ValueType new_value) {
            value = std::move(new_value);
        }
    };

    template <typename KeyType, typename ValueType, size_t table_size = 1024>
    class HashMap {
    private:
        HashNode<KeyType, ValueType> **nodes;

        size_t hash_of(const KeyType &key) {
            return reinterpret_cast<size_t>(key) % table_size;
        }

    public:
        explicit HashMap() : nodes(new HashNode<KeyType, ValueType>*[table_size]) {}

        ~HashMap() {
            for(size_t i = 0; i < table_size; i++) {
                HashNode<KeyType, ValueType> *node = nodes[i];
                while(node) {
                    HashNode<KeyType, ValueType> *c_node = node;
                    node = node->get_next_node();
                    delete c_node;
                }
                nodes[i] = nullptr;
            }

            delete[] nodes;
        }

        ValueType *get(const KeyType &key) {
            size_t hash = hash_of(key);
            HashNode<KeyType, ValueType> *node = nodes[hash];
            while(node) {
                if(node->get_key() == key) {
                    return &node->get_value();
                }
                node = node->get_next_node();
            }
            return nullptr;
        }

        void put(const KeyType &key, ValueType value) {
            size_t hash = hash_of(key);

            HashNode<KeyType, ValueType> *node = nodes[hash];
            HashNode<KeyType, ValueType> *previous_node = nullptr;

            while(node && node->get_key() != key) {
                previous_node = node;
                node = node->get_next_node();
            }

            if(!node) {
                node = new HashNode<KeyType, ValueType>(key, std::move(value));
                if(previous_node) {
                    previous_node->set_next_node(node);
                } else {
                    nodes[hash] = node;
                }
            } else {
                node->set_value(std::move(value));
            }
        }

        bool remove(const KeyType &key) {
            size_t hash = hash_of(key);

            HashNode<KeyType, ValueType> *node = nodes[hash];
            HashNode<KeyType, ValueType> *previous_node = nullptr;

            while(node && node->get_key() != key) {
                previous_node = node;
                node = node->get_next_node();
            }

            if(!node) {
                return false;
            } else {
                if(!previous_node) {
                    nodes[hash] = node->get_next_node();
                } else {
                    previous_node->set_next_node(node->get_next_node());
                }

                delete node;
                return true;
            }
        }

        bool contains(const KeyType &key) {
            size_t hash = hash_of(key);

            HashNode<KeyType, ValueType> *node = nodes[hash];

            while(node && node->get_key() != key) {
                node = node->get_next_node();
            }

            return node != nullptr;
        }

        Vector<KeyType> keys() const {
            std::Vector<KeyType> keys;
            for(size_t i = 0; i < table_size; i++) {
                HashNode<KeyType, ValueType> *node = nodes[i];
                if(node) {
                    keys.add(node->get_key());
                }
            }
            return std::move(keys);
        }
    };
}