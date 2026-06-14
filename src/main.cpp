#include "models/Router.h"
#include <database/DBManager.h>

#include <iostream>
#include <limits>
#include <limits.h>



int main() {
    DBManager db;

    Router app;
    app.setDatabase(db);

    std::cout << "<--- Library Management System --->\n";
    while(true) {
        if(app.authenticate()) app.run();
    }
    return 0;


    return 0;
}