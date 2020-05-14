#pragma once

#include <libusb.h>
#include <exception>
#include <string>

namespace tablet_tools::usb_control {
    class LibUsbError : public std::exception {
    private:
        int libusb_error;
        std::string message;

    public:
        explicit LibUsbError(int libusb_error);
        [[nodiscard]] const char *what() const noexcept final;
    };

    class LibUsbInstance {
    private:
        libusb_context *context;

    public:
        explicit LibUsbInstance();
        ~LibUsbInstance();

        template <typename Fn, typename ...Args,
                typename ReturnType = std::invoke_result_t<Fn, libusb_context*, Args...>,
                std::enable_if_t<std::is_invocable_v<Fn, libusb_context*, Args...>, int> = 0>
        ReturnType run(Fn fn, Args &&...args) {
            ReturnType return_value = fn(context, std::forward<Args>(args)...);

            if constexpr(std::is_integral_v<ReturnType>) {
                if (return_value < 0) {
                    throw LibUsbError(static_cast<int>(return_value));
                }
            }

            return return_value;
        }

        template <typename Fn, typename ...Args,
                typename ReturnType = std::invoke_result_t<Fn, Args...>,
                std::enable_if_t<std::is_invocable_v<Fn, Args...>, int> = 0>
        ReturnType run(Fn fn, Args &&...args) {
            ReturnType return_value = fn(std::forward<Args>(args)...);

            if constexpr(std::is_integral_v<ReturnType>) {
                if (return_value < 0) {
                    throw LibUsbError(static_cast<int>(return_value));
                }
            }

            return return_value;
        }
    };
}