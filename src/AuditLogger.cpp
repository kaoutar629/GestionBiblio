#include "AuditLogger.h"

#include "AuditLogger.h"
#include <fstream>
#include <ctime>

void AuditLogger::log(const std::string& message,
                      const std::string& level) {
    // timestamp
    std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp),
                  "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // écriture en append
    std::ofstream file("audit.log", std::ios::app);
    if (file.is_open()) {
        file << "[" << timestamp << "]"
             << "[" << level    << "] "
             << message         << "\n";
    }
}
