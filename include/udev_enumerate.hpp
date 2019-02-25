#pragma once
#include <libudev.h>
#include "udev_list.hpp"

namespace udevadm {

    class udev_enumerate
    {
    public:
        //! 构造
        udev_enumerate(struct ::udev_enumerate *handle) : handle_(handle) { }
        
        //! 拷贝构造
        udev_enumerate(const udev_enumerate &other) : handle_(udev_enumerate_ref(other.handle_)) { }

        //! 移动构造
        udev_enumerate(udev_enumerate &&other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }

        //! 析构
        virtual ~udev_enumerate() noexcept
        {
            if (handle_) {
                udev_enumerate_unref (handle_);
                handle_ = nullptr;
            }
        }

        //! 拷贝赋值
        udev_enumerate& operator=(const udev_enumerate &other)
        {
            if (handle_) udev_enumerate_ref(handle_);
            handle_ = udev_enumerate_ref(other.handle_);
            return *this;
        }

        //! 移动赋值
        udev_enumerate& operator=(udev_enumerate &&other) noexcept
        {
            if (handle_) udev_enumerate_ref(handle_);
            handle_ = other.handle_;
            other.handle_ = nullptr;
            return *this;
        };
        operator bool() const { return handle_; }

    public:                     // 接口部分
        // subsystem : filter for a subsystem of the device to include in the list
        // Returns : 0 on success, otherwise a negative error value.
        int add_match_subsystem(const char * subsystem) const
        {
            return udev_enumerate_add_match_subsystem (handle_, subsystem);
        }
        
        int scan_devices() const
        {
            return udev_enumerate_scan_devices(handle_);
        };

        udev_list_entry get_list_entry() const
        {
            return udev_list_entry(udev_enumerate_get_list_entry(handle_));
        }
        
    private:
        struct ::udev_enumerate *handle_;
    };
    

}  // Udev
