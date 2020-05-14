#pragma once

#include "tablet-tools/uefi-bootengine/std/move.hpp"
#include "tablet-tools/uefi-bootengine/std/std_stream.hpp"
#include "tablet-tools/uefi-bootengine/std/type_traits.hpp"
#include "tablet-tools/uefi-bootengine/extra_type_traits.hpp"
#include "tablet-tools/uefi-bootengine/stdlib.hpp"

namespace tablet_tools::uefi_bootengine {
    template <typename OkType, typename ErrorType>
    class Result {
    public:
        static constexpr size_t storage_size = biggest_size_v<OkType, ErrorType>;

    private:
        bool ok;
        char storage[storage_size];

        explicit Result(std::true_type, OkType t) : ok(true), storage{0} {
            new(&storage[0]) OkType(std::move(t));
        }

        explicit Result(std::false_type, ErrorType t) : ok(false), storage{0} {
            new(&storage[0]) ErrorType(std::move(t));
        }

    public:
        static Result make_ok(OkType t) {
            return std::move(Result(std::true_type{}, std::move(t)));
        }

        static Result make_error(ErrorType t) {
            return std::move(Result(std::false_type{}, std::move(t)));
        }

        explicit operator bool() const {
            return ok;
        }

        OkType &value() {
            if(!ok) {
                std::cerr << u"Tried to access success value of errored result!" << std::endl;
            }
            return *reinterpret_cast<OkType *>(storage);
        }

        const OkType &value() const {
            if(!ok) {
                std::cerr << u"Tried to access success value of errored result!" << std::endl;
            }
            return *reinterpret_cast<const OkType *>(storage);
        }

        ErrorType error() {
            if(ok) {
                std::cerr << u"Tried to access error value of succeeded result!" << std::endl;
            }
            return *reinterpret_cast<ErrorType *>(storage);
        }

        const ErrorType &error() const {
            if(ok) {
                std::cerr << u"Tried to access error value of succeeded result!" << std::endl;
            }
            return *reinterpret_cast<const ErrorType *>(storage);
        }

        template <typename Fn, typename NewOkType = std::invoke_result_t<Fn, OkType>>
        Result<NewOkType, ErrorType> map(Fn &&fn) {
            if(ok) {
                return Result<NewOkType, ErrorType>::make_ok(fn(*reinterpret_cast<OkType *>(storage)));
            } else {
                return Result<NewOkType, ErrorType>::make_error(*reinterpret_cast<ErrorType *>(storage));
            }
        }

        template <typename Fn, typename NewErrorType = std::invoke_result_t<Fn, ErrorType>>
        Result<OkType, NewErrorType> map_error(Fn &&fn) {
            if(ok) {
                return Result<OkType, NewErrorType>::make_ok(*reinterpret_cast<OkType *>(storage));
            } else {
                return Result<OkType, NewErrorType>::make_error(fn(*reinterpret_cast<ErrorType *>(storage)));
            }
        }

        template <typename Fn, typename NewOkType = std::invoke_result_t<Fn, OkType>>
        Result<NewOkType, ErrorType> flat_map(Fn &&fn) {
            if(ok) {
                return std::move(fn(*reinterpret_cast<OkType *>(storage)));
            } else {
                return Result<NewOkType, ErrorType>::make_error(*reinterpret_cast<ErrorType *>(storage));
            }
        }

        template <typename Fn, typename NewErrorType = std::invoke_result_t<Fn, ErrorType>>
        Result<OkType, NewErrorType> flat_map_error(Fn &&fn) {
            if(ok) {
                return Result<OkType, NewErrorType>::make_ok(*reinterpret_cast<OkType *>(storage));
            } else {
                return std::move(fn(*reinterpret_cast<ErrorType *>(storage)));
            }
        }
    };
}