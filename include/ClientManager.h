#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include "IManager.h"
#include <string>
#include "Models.h"

class ClientManager : public IManager<Client> {
public:
    void add(const Client& client) override;
    bool remove(int id) override;
    bool update(const Client& client) override;

    std::optional<Client> getById(int id) const override;
    std::vector<Client> getAll() const override;
};


#endif // CLIENTMANAGER_H
