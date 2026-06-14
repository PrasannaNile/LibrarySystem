#include "models/Router.h"

#include <iostream>
#include <limits>
#include <limits.h>



int main() {
    Router app;

    std::cout << "<--- Library Management System --->\n";
    while(true) {
        if(app.authenticate()) app.run();
    }
    return 0;


    return 0;
}