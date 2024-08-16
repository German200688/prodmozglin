#pragma once

#include <memory>
#include <mutex>
#include <libpq-fe.h>
#include "log.hpp"

class Database
{
public:
    Database();
    std::shared_ptr<PGconn> GetConnection() const;
    bool ExecuteSQL(const std::string& sql);

int nFields1;
char dbcount;
private:
 
    

    std::shared_ptr<PGconn>  m_connection;
};
