#pragma once
#include <libudev.h>
#include "udev_list.hpp"

namespace udevadm {

    // 内核系统设备的表示。
    // 设备由其系统路径唯一标识，每个设备在内核系统文件系统中只有一个路径。
    // 设备通常属于内核子系统，并且在该子系统中具有唯一的名称。
    class udev_device
    {
    public:
        //! 构造
        udev_device(struct ::udev_device *handle) : handle_(handle) { }
        
        //! 拷贝构造
        udev_device(const udev_device &other) : handle_(udev_device_ref(other.handle_)) { }

        //! 移动构造
        udev_device(udev_device &&other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }

        //! 析构
        virtual ~udev_device() noexcept
        {
            if (handle_) {
                udev_device_unref (handle_);
                handle_ = nullptr;
            }
        }

        //! 拷贝赋值
        udev_device& operator=(const udev_device &other)
        {
            if (handle_) udev_device_ref(handle_);
            handle_ = udev_device_ref(other.handle_);
            return *this;
        }

        //! 移动赋值
        udev_device& operator=(udev_device &&other) noexcept
        {
            if (handle_) udev_device_ref(handle_);
            handle_ = other.handle_;
            other.handle_ = nullptr;
            return *this;
        }
        
        operator bool() const { return handle_; }
        
    private:
        
        inline const std::string tostring(const char * str)
        {
            if (str) return str;
            return "";
        }
        
    public:                     // 接口部分
        // 获取父设备
        udev_device get_parent()
        {
            return udev_device(udev_device_get_parent(handle_));
        }
        udev_device get_parent_with_subsystem_devtype(const char *subsystem, const char *devtype)
        {
            return udev_device(udev_device_get_parent_with_subsystem_devtype(handle_, subsystem, devtype));
        }
        // 获取设备号
        dev_t get_devnum()
        {
            return udev_device_get_devnum(handle_);
        }
        // 获取设备路径 /sys/devices
        const std::string get_devpath()
        {
            return tostring(udev_device_get_devpath(handle_));
        }
        // 获取子系统 block
        const std::string get_subsystem()
        {
            return tostring(udev_device_get_subsystem(handle_));
        }
        // 获取设备类型 disk
        const std::string get_devtype()
        {
            return tostring(udev_device_get_devtype(handle_));
        }
        // 获取系统路径 /sys/
        const std::string get_syspath()
        {
            return tostring(udev_device_get_syspath(handle_));
        }
        // 获取系统名称 sr0
        const std::string get_sysname()
        {
            return tostring(udev_device_get_sysname(handle_));
        }
        // 获取系统设备号 0
        const std::string get_sysnum()
        {
            return tostring(udev_device_get_sysnum(handle_));
        }
        // 获取设备节点 /dev/sr0
        const std::string get_devnode()
        {
            return tostring(udev_device_get_devnode(handle_));
        }
        // 获取驱动
        const std::string get_driver()
        {
            return tostring(udev_device_get_driver(handle_));
        }
        // 获取行为
        const std::string get_action()
        {
            return tostring(udev_device_get_action(handle_));
        }
        const std::string get_sysattr_value(const char * sysattr)
        {
            return tostring(udev_device_get_sysattr_value(handle_, sysattr));
        }

        udev_list_entry get_devlinks_list_entry()
        {
            return udev_device_get_devlinks_list_entry(handle_);
        }
        udev_list_entry get_properties_list_entry()
        {
            return udev_device_get_properties_list_entry(handle_);
        }
        udev_list_entry get_tags_list_entry()
        {
            return udev_device_get_tags_list_entry(handle_);
        }
        
    private:
        struct ::udev_device *handle_;
    };
    
}  // Udev
