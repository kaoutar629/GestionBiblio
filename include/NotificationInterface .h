#ifndef NOTIFICATION INTERFACE_H
#define NOTIFICATION INTERFACE_H

#include <string>

class INotification {
public:
    virtual ~INotification() = default;

    virtual void send(const std::string& recipient,
                      const std::string& message) = 0;
};
#endif
