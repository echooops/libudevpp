#pragma once
#include <libudev.h>

namespace udevadm {

    // libudev 提供的接口有限无法封装为迭代器
    class udev_list_entry
    {
    public:
        //! 构造
        udev_list_entry(struct ::udev_list_entry *handle) : handle_(handle) { }
        
        //! 拷贝构造
        udev_list_entry(const udev_list_entry &other) : handle_(other.handle_) { };

        //! 移动构造
        udev_list_entry(udev_list_entry &&other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }

        //! 析构
        virtual ~udev_list_entry() noexcept { if (handle_) handle_ = nullptr; }

        //! 拷贝赋值
        udev_list_entry& operator=(const udev_list_entry &other)
        {
            handle_ = other.handle_;
            return *this;
        }

        //! 移动赋值
        udev_list_entry& operator=(udev_list_entry &&other) noexcept
        {
            handle_ = other.handle_;
            other.handle_ = nullptr;
            return *this;
        };
        
        operator bool() const { return handle_; }

        udev_list_entry operator++()
        {
            handle_ = udev_list_entry_get_next(handle_);
            return *this;
        }

        udev_list_entry operator++(int)
        {
            udev_list_entry tmp = *this; ++*this; return tmp;
        }

        const std::string operator[](const char* name) const
        {
            return udev_list_entry(udev_list_entry_get_by_name(handle_, name)).get_value();
        }
        
    private:
        
        inline const std::string tostring(const char * str) const
        {
            if (str) return str;
            return "";
        }
        
    public:                     // 接口部分
        udev_list_entry get_next() const
        {
            return udev_list_entry(udev_list_entry_get_next(handle_));
        }
        
        udev_list_entry get_by_name(const char * name) const
        {
            return udev_list_entry(udev_list_entry_get_by_name(handle_, name));
        }
        
        const std::string get_name() const
        {
            return tostring(udev_list_entry_get_name(handle_));
        }
        
        const std::string get_value() const
        {
            return tostring(udev_list_entry_get_value(handle_));
        }
        
    private:
        struct ::udev_list_entry *handle_;
    };

}  // Udev
