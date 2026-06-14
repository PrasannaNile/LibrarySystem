#include "database/DBManager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
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
    try {
        // Module 1: Retrieve the global MySQL driver instance
        driver = sql::mysql::get_mysql_driver_instance();
        
        if (driver == nullptr) {
            std::cerr << "[DB Error] Failed to retrieve MySQL driver instance!\n";
            return false;
        }
        
        std::cout << "[DBManager] Driver instance retrieved successfully.\n";
        
        // Module 2: Open the network pipe using your credentials
        // Parameters: (Server host + port, username, password)
        con = driver->connect("tcp://127.0.0.1:3306", "root", "Nile@26");
        
        // Connect specifically to our LibrarySystem database schema
        con->setSchema("LibrarySystem");
        
        std::cout << "[DBManager] Securely connected to LibrarySystem database server!\n";

        return true;
    }
    catch (sql::SQLException &e) {
        std::cerr << "[DB Exception] Driver setup failed: " << e.what() << "\n";
        return false;
    }
}