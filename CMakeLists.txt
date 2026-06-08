cmake_minimum_required(VERSION 3.10)
project(LibrarySystem)

# Force C++17 standard for modern features (like modern enums/initializers)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for our final executable application
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Tell the compiler where to find our header blueprints root
include_directories(include)

# Gather our source files and create the executable named "LibrarySystem"
add_executable(LibrarySystem 
    src/main.cpp 
    src/models/Book.cpp
    src/models/User.cpp
    src/models/Library.cpp
    src/models/Transaction.cpp
)