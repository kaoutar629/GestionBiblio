#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H
#include "IManager.h"
#include <string>
#include "Models.h"

class VehicleManager : public IManager<Vehicle> {
public:
    void add(const Vehicle& vehicle) override;
    bool remove(int id) override;
    bool update(const Vehicle& vehicle) override;

    std::optional<Vehicle> getById(int id) const override;
    std::vector<Vehicle> getAll() const override;
};
#endif // VEHICLEMANAGER_H
