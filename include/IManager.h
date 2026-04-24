#ifndef IMANAGER_H
#define IMANAGER_H
#include <vector>
#include <optional>

template <typename T>
class IManager {
public:
    virtual ~IManager() = default;

    virtual void add(const T& item) = 0;
    virtual bool remove(int id) = 0;
    virtual bool update(const T& item) = 0;

    virtual std::optional<T> getById(int id) const = 0;
    virtual std::vector<T> getAll() const = 0;
};


#endif // IMANAGER_H
