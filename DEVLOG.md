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


## Day 5. Transactions
- **Goal:** Keep a track of borrow and return book transaction
- **Need:** List to store bookId and userId involved in transaction
- **Implementation:** Major operation like issue book and return book 
  - needs new transaction creation 
- **Issue Book:** Book can be only issue if its available
- **Return Book:** Book can be only return if it is loaned
  - to return there must be a transaction entry for Issued Book (bookId, userId, ISSUE)

## Day 6. Model Persistence and File IO
- **Goal:** Users and Books are only store in vector
  - They only exist for a session when program runs
- **Need:** Data files on permenant storage to store all books and users
- **Concept:** Used of fstream header file for File IO
  - **ofstream(writing):** Outputing data to the file
  - **ifstream(reading):** Inputing data from the file
  - Just like cout, outputs data to console, ofstream does this in file
  - Just like cin, extract data from console, ifstream does from file
- **Auto generate Id:** Book and user have auto generate unique Id to avoid collision

## Day 7. Interactive Console UI Menu
### Progress Summary
- **Built Main Application Engine:** Implemented a continuous `while(true)` application loop in `main.cpp` to keep the program active until explicitly terminated by the user.
- **Engine Router:** Configured a native C++ block-scoped `switch` statement to seamlessly route user selections across available system functionalities.
- **Bulletproofed Input Buffers:** Integrated customized stream cleaning routines utilizing `std::cin.clear()` and `std::cin.ignore()` to entirely resolve input skipping bugs when mixing raw `std::cin >>` reads with `std::getline()`.
- **Crash-Proof Conversions:** Wrapped string-to-numeric casting (`std::stof`) inside robust `try-catch` blocks to protect the active run-time environment from throwing fatal unhandled exceptions on malformed alphanumeric inputs.

### Technical Learnings
- **Switch Case Variable Scoping:** Re-verified that initializing variables directly inside individual switch cases violates compiler rules unless explicitly wrapped in localized brace blocks `{ }` to define clear scope lifetimes.
- **Input Fail States:** Learned that pass-through alphanumeric values targeted at numerical variables trip `std::cin.fail()`, forcing infinite looping behavior if the stream state flags aren't forcefully reset and purged via the standard limits buffer.

## Day 8. Routing Seperation and Search Engine on Books
- **Menus:** Different menus for User.ADMIN and User.STUDENT 
- **Switch:** Seperate switch blocks on user choice base on menus
- **Admin Menu:** Admin have some priviledge operation like add books 
  - and registered new user
- **Student Menu:** Student can only search for books and borrow them
- **Sub-String Search Engine:** Implemented a real-time, case-insensitive keyword filtering module using 
  - -`std::transform` and `std::string::find` to sweep title and author fields.
- **Input Layer Resilience:** Secured the account generation pipeline by replacing raw input streams with explicit 
  - exception validation (`std::runtime_error`) to catch formatting out-of-bound errors.
- **Immediate Persistence (Transactional Integrity):** Configured automated background save triggers directly 
  - inside data-mutation methods (`add_book`, `register_user`) to guarantee disk sync on every state modification.

## Day 9: Relational Memory Linking & Pointer-Based Borrow System
### Architectural Updates
* **Memory Pipeline Integration:** Refactored the data-lookup strategy from passing objects by value to returning raw memory references (`Book*`) via an address-of operator (`&`) pipeline to prevent redundant object copying.
* **Relational Cross-Linking:** Expanded the `User` domain model to maintain a private tracking collection (`std::vector<Book*> borrowedBooks`) establishing a true memory-level association between users and library inventory.
* **Orchestrated Synchronization Handshakes:** Redesigned both `issue_book` and `return_book` routines within the `Library` controller to accept raw `User&` handles, coordinating real-time state manipulation and profile allocation in a single transaction loop.
### Feature Enhancements
* **Safe Pointer Retrieval:** Implemented a targeted ID scanner (`search_book_by_id`) utilizing strict `nullptr` verification boundaries to safely isolate mutable entity allocations before access manipulation.
* **Linear Garbage Scrubbing:** Deployed the optimized Erase-Remove Idiom (`std::remove` combined with vector `.erase()`) within the user domain to cleanly purge specific memory addresses and compress storage allocations during asset registration resets.


## Day 10: Application Controller Refactoring & Relational Report Engines

### Architectural Updates
* **Application Controller Pattern:** Extracted user interaction, menu rendering, and session state logic completely out of `main.cpp` into a dedicated `Router` class, reducing `main()` to a clean 3-line startup script.
* **State-Driven Menu Routing:** Eliminated insecure and redundant manual inputs (such as prompting for a `userId` during student operations) by leveraging the persistent `currentUser` session variable inside the application router.
* **Chronological State Reconstruction:** Integrated a live state tracking engine into the file initialization loop (`load_transactions_from_file`), allowing the system to replay transaction history sequentially on boot to build an accurate runtime state.

### Feature Enhancements
* **Student Portfolio Dashboard:** Implemented a memory-safe reporting method (`display_borrowed_books`) utilizing pointer iteration and arrow syntax (`->`) to let students view active asset allocations directly from live memory.
* **Global Asset Ledger Optimization:** Designed a high-performance auditing mechanism utilizing a constant-time $O(1)$ Hash Map lookup scheme (`std::unordered_map<std::string, std::string>`) to match `bookId` keys cleanly with active `userId` locations.
* **Constructor Boot Hardening:** Isolated and resolved a initialization-phase silent crash involving numeric type parsing (`std::stoi`) during structural id field formatting within the `Transaction` entity container.


## DEVLOG: Day 11 — Authentication Core & Guardrails Securing

### ⚙️ Architecture & Flow Refactors
* **Master State Loop:** Moved application lifecycle control to `main.cpp` using a `while(true)` engine block. 
* **Pristine Logouts:** Handled session termination (`choice == 0`) by clearing the `currentUser` pointer to `nullptr` and breaking to the master loop, prompting a clean re-login instead of shutting down.
* **Memory Link Realignment:** Fixed stack allocation traps by converting `currentUser` into a direct `User*` pointer tracking live memory inside `Library::users`.

### 🛡️ Security Validation Walls
* **Unique Email Check:** Implemented a `std::unordered_map<std::string, bool>` lookup cache, reducing signup email duplication checks to instant $O(1)$ complexity.
* **Identity Alignment Fix:** Restructured `handleUserLogin` to verify incoming credentials directly against the targeting user record attributes to protect against identity spoofing.
* **Asset Ceiling Constraints:** Injected transactional validation gates inside `Library::issue_book` to drop borrow attempts if an account holds $\ge 3$ active loans.