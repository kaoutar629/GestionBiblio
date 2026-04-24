#ifndef RESERVATIONMANAGER_H
#define RESERVATIONMANAGER_H
#include <string>
#include <vector>
#include <optional>
#include "Models.h"
#include "IManager.h"

class ReservationManager : public IManager<Reservation> {
public:
    void add(const Reservation& item) override;
    bool remove(int id) override;
    bool update(const Reservation& item) override;
    std::optional<Reservation> getById(int id) const override;
    std::vector<Reservation> getAll() const override;
};
#endif
