#pragma once
#include <libudev.h>
#include "udev_device.hpp"

namespace udevadm {

    class udev_monitor
    {
    public:
        //! 构造
        udev_monitor(struct ::udev_monitor *handle) : handle_(handle) { }
        
        //! 拷贝构造
        udev_monitor(const udev_monitor &other) : handle_(udev_monitor_ref(other.handle_)) { }

        //! 移动构造
        udev_monitor(udev_monitor &&other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }

        //! 析构
        virtual ~udev_monitor() noexcept
        {
            if (handle_) {
                udev_monitor_unref (handle_);
                handle_ = nullptr;
            }
        }

        //! 拷贝赋值
        udev_monitor& operator=(const udev_monitor &other)
        {
            if (handle_) udev_monitor_ref(handle_);
            handle_ = udev_monitor_ref(other.handle_);
            return *this;
        }

        //! 移动赋值
        udev_monitor& operator=(udev_monitor &&other) noexcept
        {
            if (handle_) udev_monitor_ref(handle_);
            handle_ = other.handle_;
            other.handle_ = nullptr;
            return *this;
        };
        
        operator bool() const { return handle_; }

    public:                     // 接口部分
        int filter_add_match_subsystem_devtype(const char * subsystem, const char * devtype)
        {
            return udev_monitor_filter_add_match_subsystem_devtype(handle_, subsystem, devtype);
        }

        int enable_receiving()
        {
            return udev_monitor_enable_receiving(handle_);
        }

        int get_fd()
        {
            return udev_monitor_get_fd(handle_);
        }

        udev_device receive_device()
        {
            return udev_device(udev_monitor_receive_device(handle_));
        }
        
    private:
        struct ::udev_monitor *handle_ = nullptr;
    };
    

}  // Udev
