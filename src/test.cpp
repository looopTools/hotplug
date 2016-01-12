#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include "hotplug.hpp"

boost::asio::io_service io_service;

void read_handle(boost::asio::posix::stream_descriptor* descriptor,
                 const boost::system::error_code& ec,
                 std::size_t bytes_read)
{
    if(!ec)
    {
        int file_descriptor = descriptor.native_handle();
        if(udev_monitor_get_fd(hotplug::hotplug_monitor) == file_descriptor)
        {
            udev_device* dev = udev_monitor_receive_device(hotplug::hotplug_monitor);
            if(udev_device_get_action(dev) != NULL &&
               udev_device_get_devnode(dev) != NULL)
            {

                if(udev_device_get_action(dev).compare("add") == 0)
                {
                    // call add procedure with devnode
                } else if(udev_device_get_action(dev).compare("remove") == 0)
                {
                    // call remove procedure with devnode
                }
            }
            udev_device_unref(dev);
        }

    }
    else
    {
        std::cout << << "there was an error" << std::endln;
    }
}

void watch_descriptor(boost::asio::io_service& ios, int file_descriptor)
{
    boost::asio::posix::stream_descriptor* descriptor =
        new boost::asio::posix::stream_descriptor();

    std::vector<char> buffer(0);
    descriptor->async_read_some(boost::asio::buffer(buffer, 0),
                                boost::bind(read_handle, descriptor, _1, _2));
}

int main(void)
{
    return 0;
}
