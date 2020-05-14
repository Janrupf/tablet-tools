#pragma once

#include "tablet-tools/uefi-bootengine/stdlib.hpp"
#include "tablet-tools/uefi-bootengine/std/move.hpp"
#include "tablet-tools/uefi-bootengine/std/forward.hpp"

namespace tablet_tools::uefi_bootengine::std {
    template <typename T>
    class SharedPtr {
    private:
        size_t *reference_counter;
        T *self;

    public:
        explicit SharedPtr(T *self) : self(self), reference_counter(new size_t(1)) {}
        ~SharedPtr() {
            if(reference_counter && --(*reference_counter) == 0) {
                delete self;
            }
        }

        SharedPtr(const SharedPtr &other) : self(other.self), reference_counter(other.reference_counter) {
            (*reference_counter)++;
        }

        SharedPtr(SharedPtr &&other) noexcept :
            self(move(other.self)), reference_counter(move(other.reference_counter)) {
            other.reference_counter = nullptr;
            other.self = nullptr;
        }

        SharedPtr &operator =(const SharedPtr &other) {
            if(this != &other) {
                self = other.self;
                reference_counter = other.reference_counter;
                (*reference_counter)++;
            }
            return *this;
        }

        SharedPtr &operator =(SharedPtr &&other) noexcept {
            self = move(other.self);
            reference_counter = move(other.reference_counter);
            other.reference_counter = nullptr;
            other.self = nullptr;
            return *this;
        }

        T &operator *() {
            return *self;
        }

        const T &operator *() const {
            return *self;
        }

        T *operator ->() {
            return self;
        }

        const T *operator ->() const {
            return self;
        }

        T *get() {
            return self;
        }

        const T *get() const {
            return self;
        }

        template <typename NewT, std::enable_if_t<std::is_base_of_v<NewT, T>, int> = 0>
        explicit operator SharedPtr<NewT>() {
            NewT *new_t = static_cast<NewT *>(new T(*self));
            return SharedPtr<NewT>(new_t);
        }
    };

    template <typename T, typename ...Args>
    SharedPtr<T> make_shared(Args &&...args) {
        return SharedPtr<T>(new T(args...));
    }
}