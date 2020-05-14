#pragma once

namespace tablet_tools::uefi_bootengine::std {
    template <typename T>
    struct LinkedListEntry {
        T value;
        LinkedListEntry<T> *next_entry;

        explicit LinkedListEntry(T t) : value(std::move(t)), next_entry(nullptr) {}
    };

    template <typename T>
    class LinkedList {
    private:
        LinkedListEntry<T> *first_entry;

    public:
        explicit LinkedList() : first_entry(nullptr) {}

        ~LinkedList() {
            LinkedListEntry<T> *entry = first_entry;
            while(entry) {
                LinkedListEntry<T> *current = entry;
                entry = current->next_entry;
                delete current;
            }
        }

        void append(T t) {
            if(!first_entry) {
                first_entry = new LinkedListEntry<T>(std::move(t));
                return;
            }

            LinkedListEntry<T> *current_entry = first_entry;
            while(current_entry->next_entry) {
                current_entry = current_entry->next_entry;
            }

            current_entry->next_entry = new LinkedListEntry<T>(std::move(t));
        }

        T remove_first() {
            LinkedListEntry<T> *entry = first_entry;
            T val = std::move(entry->value);
            first_entry = entry->next_entry;
            delete entry;
            return val;
        }
    };
}