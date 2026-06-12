#pragma once

#include <iostream>
#include <limits>
#include <limits.h>

#include "models/Library.h"
#include "User.h" // Ensure this is included for the User class and UserRole enum

class Router {
private:
    Library lib;
    User currentUser; // Tracks the currently active session

    // Input Validation Helpers
    bool handleNumericInput();
    void clearInputBuffer();

    // Menu Renderers
    void show_admin_menu() const;
    void show_student_menu() const;

    // Route Evaluators
    void handleAdminRouter(int choice);
    void handleStudentRouter(int choice);

public:
    Router();   // Constructor: Coordinates system database file loads
    void run(); // Main execution engine loop
};