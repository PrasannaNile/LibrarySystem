#pragma once

#include <mysql.h>

namespace sql {
    class Connection;
    namespace mysql {
        class MySQL_Driver;
    }
}


class DBManager {
private:
    sql::mysql::MySQL_Driver* driver; // The driver factory instance
    MYSQL* con;             // The live server socket pointer
    
public:
    DBManager();  // Opens socket on application boot
    ~DBManager(); // Safely teardowns socket on exit

    bool connect(); // Private engine that runs the handshake protocol
    MYSQL* get_connection() const;
};