#pragma once
#include "Logger.h"
#include <memory>
class WrapLogger {
public:
    static std::shared_ptr<Logger*> getLogger();
    WrapLogger();
    ~WrapLogger();
private:
    static std::shared_ptr<Logger*> m_logger;
};