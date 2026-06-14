#pragma once

namespace sql {
    class Connection;
    namespace mysql {
        class MySQL_Driver;
    }
}


class DBManager {
private:
    sql::mysql::MySQL_Driver* driver; // The driver factory instance
    sql::Connection* con;             // The live server socket pointer

    bool connect(); // Private engine that runs the handshake protocol

public:
    DBManager();  // Opens socket on application boot
    ~DBManager(); // Safely teardowns socket on exit
};