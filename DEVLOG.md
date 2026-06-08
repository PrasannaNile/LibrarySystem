# Developer Log: Enterprise Library & Analytics System

## Day 1: Project Setup & Core Domain Modeling
- **Goal:** Establish multi-file directory layout and implement the `Book` class core attributes.
- **Concepts Learned/Reviewed:** - Difference between Header (`.h`) and Source (`.cpp`) files.
  - Data Encapsulation and Access Specifiers (`private` vs `public`).
  - Member Initialization Lists.
- **Code Progress:** Created `Book.h`, `Book.cpp`, and initialized `main.cpp`.
- **Blockers/Notes:** Need to ensure MySQL connector paths are set up correctly in `CMakeLists.txt` tomorrow.


## Day 2. Recovery and User Modeling
- **Goal** Design User Model core attributes and getter & setter methods
- **Massive Collaspe:** Accidentally lost all work from Day 1. No trace of Day 1 work 
  - and also no permanent work push on github.
-  **Recovery:** Struture entire LibrarySystem from scratch and build again
- **Github repo:** New remote repo and everything is permanently tracked
- **CMake:** download CMake to manage huge multi-file project easily
  - creates a map for .cpp file and link them together to generate .exe
- **Pull Request:** reviewed feature/User-model and merge with main


## Day 3. Book Lifecycle
- **Goal:** Handle lifecycle of Book object 
- **Concept:** If class have dynamic memory allocation on heap using new keyword,
  - it must be deallocated after during object goes out of bound
- **Implementation:** Copy constructor to create exact same Book model
  - Copy assignment operator to match the state of existing Book model
  - Destructor to free up heap memory to avoid memory leaks
- **Issues:** Intellisense show fake red squiggly lines, no error but still project red
  - **solution** developer reload window from palette


## Day 4. Library Controller
- **Goal:** Design Library model which is center for all branch
- **Need:** Tracks all books and users current in library
- **structure:** add new book and register new user in Library
 - displays book_id and title of all books 