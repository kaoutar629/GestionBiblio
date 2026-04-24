#ifndef AUDITLOGGER_H
#define AUDITLOGGER_H
#include <vector>
#include <string>


class AuditLogger {
public:
    static void log(const std::string& message,
                    const std::string& level);
};



#endif // AUDITLOGGER_H
