#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H
#include <string>
#include <string>
#include <optional>
#include "Models.h"


class LoginManager {
public:
    std::optional<User> login(const std::string& username,
                              const std::string& password);

    void logout(int userId);
};
#endif // LOGINMANAGER_H
