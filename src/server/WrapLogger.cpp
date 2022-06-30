#include "WrapLogger.h"

std::shared_ptr<Logger*> WrapLogger::m_logger = nullptr;

std::shared_ptr<Logger*> WrapLogger::getLogger() {
    m_logger = std::make_shared<Logger*>(Logger::getInstance());
    return m_logger;
}

WrapLogger::~WrapLogger() {
    
    m_logger.reset();
    delete this;
}