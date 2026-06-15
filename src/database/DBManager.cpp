#include "database/DBManager.h"

// #include <jdbc/mysql_driver.h>
// #include <jdbc/mysql_connection.h>
#include <iostream>

// Constructor
DBManager::DBManager() : driver(nullptr), con(nullptr) {
    std::cout << "[DBManager] Initializing database controller engine...\n";
    
    // Automatically trigger the handshake protocol on boot!
    if (!connect()) {
        std::cerr << "[Critical Error] Core database initialization sequence failed!\n";
    }
}

// Destructor
DBManager::~DBManager() {
    std::cout << "[DBManager] Shutting down engine. Cleaning up connection resources...\n";
    if (con != nullptr) {
        delete con;  // Hang up the live network pipe safely
        con = nullptr;
    }
}



bool DBManager::connect() {
    // 1. Initialize the native connection handler instance
    this->con = mysql_init(nullptr);
    if (con == nullptr) {
        std::cerr << "[DB Error] Failed to initialize MySQL handler!\n";
        return false;
    }

    // 2. Establish the secure connection pipe to localhost
    // Parameters: handler, host, user, password, database, port, unix_socket, client_flag
    if (!mysql_real_connect(con, "127.0.0.1", "root", "Nile@26", "LibrarySystem", 3306, nullptr, 0)) {
        std::cerr << "[DB Exception] Driver setup failed: " << mysql_error(con) << "\n";
        mysql_close(con);
        return false;
    }

    std::cout << "[DBManager] Securely connected to LibrarySystem database server!\n";
    
    // Note: Store 'conn' inside your class state (e.g., this->connection = conn;) 
    // so your other database query functions can reuse it later!
    return true;
}



MYSQL* DBManager::get_connection() const { return con; } 