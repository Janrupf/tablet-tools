#pragma once

#include "tablet-tools/uefi-bootengine/std/move.hpp"
#include "tablet-tools/uefi-bootengine/stddefs.hpp"

void *malloc(size_t);
void free(void *);

namespace tablet_tools::uefi_bootengine::std {
    struct vector_internal_construct_tag{};

    template <typename T>
    class Vector {
    public:
        using IteratorType = T *;

    private:
        size_t buffer_size;
        size_t used_size;

        T *buffer;

    public:
        explicit Vector() : buffer_size(16), used_size(0), buffer(static_cast<T *>(malloc(sizeof(T) * 16))) {}

        explicit Vector(size_t size)
            : buffer_size(size), used_size(size), buffer(static_cast<T *>(malloc(sizeof(T) * size))) {
            for(size_t i = 0; i < used_size; i++) {
                new(&buffer[i]) T();
            }
        }

        explicit Vector(vector_internal_construct_tag, size_t buffer_size, size_t used_size, T *buffer)
                : buffer_size(buffer_size), used_size(used_size), buffer(buffer) {}

        ~Vector() {
            if(buffer) {
                for(size_t i = 0; i < used_size; i++) {
                    buffer[i].~T();
                }
                free(buffer);
            }
        }

        Vector(const Vector &other)
            : buffer_size(other.buffer_size), used_size(other.used_size),
              buffer(static_cast<T *>(malloc(sizeof(T) * other.buffer_size))) {
            for(size_t i = 0; i < used_size; i++) {
                new(&buffer[i]) T(other.buffer[i]);
            }
        }

        Vector(Vector &&other) noexcept
            : buffer_size(other.buffer_size), used_size(other.used_size), buffer(other.buffer) {
            other.buffer_size = 0;
            other.used_size = 0;
            other.buffer = nullptr;
        }

        Vector &operator=(const Vector &other) {
            if(this != &other) {
                resize_buffer(0);
                buffer_size = other.buffer_size;
                used_size = other.used_size;
                buffer = malloc(sizeof(T) * buffer_size);

                for(size_t i = 0; i < used_size; i++) {
                    new(&buffer[i]) T(other.buffer[i]);
                }
            }

            return *this;
        }

        Vector &operator=(Vector &&other) noexcept {
            resize_buffer(0);

            buffer_size = other.buffer_size;
            used_size = other.used_size;
            buffer = other.buffer;

            other.buffer_size = 0;
            other.used_size = 0;
            other.buffer = nullptr;

            return *this;
        }

        void add(T t) {
            if(used_size == buffer_size) {
                resize_buffer(buffer_size == 0 ? 16 : buffer_size * 2);
            }
            new(&buffer[used_size]) T(std::move(t));
            used_size++;
        }

        void set(size_t index, T t) {
            buffer[index] = t;
        }

        T get(size_t index) {
            return buffer[index];
        }

        void resize(size_t new_size) {
            if(new_size == used_size) {
                return;
            }

            T *new_buffer = new_size == 0 ? nullptr : reinterpret_cast<T *>(malloc(sizeof(T) * new_size));
            if(new_size > used_size) {
                for(size_t i = 0; i < used_size; i++) {
                    new(&new_buffer[i]) T(std::move(buffer[i]));
                    buffer[i].~T();
                }

                for(size_t i = used_size; i < new_size; i++) {
                    new(&new_buffer[i]) T();
                }
            } else if(new_size < used_size) {
                for(size_t i = 0; i < new_size; i++) {
                    new(&new_buffer[i]) T(std::move(buffer[i]));
                    buffer[i].~T();
                }

                for(size_t i = new_size; i < used_size; i++) {
                    buffer[i].~T();
                }
            }

            free(buffer);
            buffer = new_buffer;
            buffer_size = new_size;
            used_size = new_size;
        }

        void clear(bool dealloc = false) {
            for(size_t i = 0; i < used_size; i++) {
                buffer[i].~T();
            }
            used_size = 0;
            if(dealloc) {
                buffer_size = 0;
                free(buffer);
                buffer = nullptr;
            }
        }

        void resize_buffer(size_t new_size) {
            if(new_size == buffer_size) {
                return;
            }

            T *new_buffer = new_size == 0 ? nullptr : reinterpret_cast<T *>(malloc(sizeof(T) * new_size));
            if(new_size > buffer_size) {
                for(size_t i = 0; i < used_size; i++) {
                    new(&new_buffer[i]) T(std::move(buffer[i]));
                    buffer[i].~T();
                }
            } else if(new_size < buffer_size) {
                size_t max_copy_size = new_size >= used_size ? used_size : new_size;

                for(size_t i = 0; i < max_copy_size; i++) {
                    new(&new_buffer[i]) T(std::move(buffer[i]));
                    buffer[i].~T();
                }

                for(size_t i = max_copy_size; i < used_size; i++) {
                    buffer[i].~T();
                    used_size--;
                }
            }

            free(buffer);
            buffer = new_buffer;
            buffer_size = new_size;
        }

        T *data() {
            return &buffer[0];
        }

        const T *data() const {
            return &buffer[0];
        }

        [[nodiscard]] size_t size() const {
            return used_size;
        }

        [[nodiscard]] bool empty() const {
            return used_size == 0;
        }

        IteratorType begin() {
            return buffer;
        }

        IteratorType begin() const {
            return buffer;
        }

        IteratorType cbegin() const {
            return buffer;
        }

        IteratorType end() {
            return buffer + used_size;
        }

        IteratorType end() const {
            return buffer + used_size;
        }

        IteratorType cend() const {
            return buffer + used_size;
        }

        template <typename NewT>
        Vector<NewT> map() {
            NewT *new_buffer = reinterpret_cast<NewT *>(malloc(sizeof(NewT) * used_size));
            for(size_t i = 0; i < used_size; i++) {
                new(&new_buffer[i]) NewT(buffer[i]);
            }
            return Vector<NewT>(vector_internal_construct_tag{}, used_size, used_size, new_buffer);
        }

        template <typename NewT>
        Vector<NewT> map() const {
            NewT *new_buffer = reinterpret_cast<NewT *>(malloc(sizeof(NewT) * used_size));
            for(size_t i = 0; i < used_size; i++) {
                new(&new_buffer[i]) NewT(buffer[i]);
            }
            return Vector<NewT>(vector_internal_construct_tag{}, used_size, used_size, new_buffer);
        }

        template <typename Fn, typename NewT = std::invoke_result_t<Fn, T>>
        Vector<NewT> map(Fn &&fn) {
            NewT *new_buffer = reinterpret_cast<NewT *>(malloc(sizeof(NewT) * used_size));
            for(size_t i = 0; i < used_size; i++) {
                new(&new_buffer[i]) NewT(fn(buffer[i]));
            }
            return Vector<NewT>(vector_internal_construct_tag{}, used_size, used_size, new_buffer);
        }

        template <typename Fn, typename NewT = std::invoke_result_t<Fn, T>>
        Vector<NewT> map(Fn &&fn) const {
            NewT *new_buffer = reinterpret_cast<NewT *>(malloc(sizeof(NewT) * used_size));
            for(size_t i = 0; i < used_size; i++) {
                new(&new_buffer[i]) NewT(fn(buffer[i]));
            }
            return Vector<NewT>(vector_internal_construct_tag{}, used_size, used_size, new_buffer);
        }
    };
}