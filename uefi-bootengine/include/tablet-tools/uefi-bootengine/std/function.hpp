#pragma once

#include "tablet-tools/uefi-bootengine/std/shared_ptr.hpp"

namespace tablet_tools::uefi_bootengine::std {
    template <typename ReturnType, typename ...Args>
    class Functor {
    public:
        virtual ~Functor() = default;

        virtual ReturnType invoke(Args &&...args) = 0;
        virtual ReturnType invoke(Args &&...args) const = 0;
    };

    template <typename ReturnType, typename ...Args>
    class RawFunctor : public Functor<ReturnType, Args...> {
    private:
        using function_ptr_t = ReturnType(*)(Args...);
        function_ptr_t function;

    public:
        explicit RawFunctor(function_ptr_t function) : function(function) {}

        ReturnType invoke(Args &&...args) final {
            if constexpr(std::is_void_v<ReturnType>) {
                function(std::forward<Args>(args)...);
            } else {
                return function(std::forward<Args>(args)...);
            }
        }

        ReturnType invoke(Args &&...args) const final {
            if constexpr(std::is_void_v<ReturnType>) {
                function(std::forward<Args>(args)...);
            } else {
                return function(std::forward<Args>(args)...);
            }
        }
    };

    template <typename FunctionType, typename ReturnType, typename ...Args>
    class StoringFunctor : public Functor<ReturnType, Args...> {
    private:
        FunctionType function;

    public:
        explicit StoringFunctor(FunctionType function) : function(std::move(function)) {}

        ReturnType invoke(Args &&...args) final {
            if constexpr(std::is_void_v<ReturnType>) {
                function(std::forward<Args>(args)...);
            } else {
                return function(std::forward<Args>(args)...);
            }
        }

        ReturnType invoke(Args &&...args) const final {
            if constexpr(std::is_void_v<ReturnType>) {
                function(std::forward<Args>(args)...);
            } else {
                return function(std::forward<Args>(args)...);
            }
        }
    };

    template <typename ReturnType, typename ...Args>
    class Function;

    template <typename ReturnType, typename ...Args>
    class Function<ReturnType(Args...)> {
    private:
        SharedPtr<Functor<ReturnType, Args...>> functor;

    public:
        explicit Function(ReturnType(*function)(Args...)) :
            functor(std::make_shared<RawFunctor<ReturnType, Args...>>(function)) {}

        template <typename FunctionType,
                std::enable_if_t<
                        !std::is_same_v<
                                std::remove_const_t<std::remove_reference_t<FunctionType>>,
                                Function<ReturnType(Args...)>
                        >,
                        int
                > = 0>
        explicit Function(FunctionType function) :
            functor(std::make_shared<StoringFunctor<FunctionType, ReturnType, Args...>>(function)) {}

        Function(const Function &other) : functor(other.functor) {}
        Function(Function &&other) noexcept : functor(std::move(other.functor)) {}

        Function &operator =(const Function &other) {
            if(this != &other) {
                functor = other.functor;
            }
            return *this;
        }

        Function &operator =(Function &&other) noexcept {
            functor = std::move(other.functor);
            return *this;
        }

        ReturnType operator()(Args &&...args) {
            if constexpr(std::is_void_v<ReturnType>) {
                functor->invoke(std::forward<Args>(args)...);
            } else {
                return functor->invoke(std::forward<Args>(args)...);
            }
        }

        ReturnType operator()(Args &&...args) const {
            if constexpr(std::is_void_v<ReturnType>) {
                functor->invoke(std::forward<Args>(args)...);
            } else {
                return functor->invoke(std::forward<Args>(args)...);
            }
        }
    };
}