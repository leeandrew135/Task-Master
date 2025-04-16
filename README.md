# Task Master

### Project Management Software

---

# PROJECT: TASK MASTER

# GROUP 56 - LUCA DUARTE, ROBIN LEE, ANDREW LEE, ETHAN CARVALHO, ALI FARHANGI

---

### Required:

- CMake (https://cmake.org/download/)
- Node.js (https://nodejs.org/en/download)
- IDE of your choice
- C++ version 17 or later

### Libraries needed:

- Crow (git clone https://github.com/CrowCpp/Crow.git)
- SQLite (https://www.sqlite.org/download.html)
- Asio (git clone https://github.com/chriskohlhoff/asio.git)

### Library installation steps:

1. Download CMake and Node.js
2. Clone repo from gitlab
3. in root folder (group 56), clone the following libraries from github: Crow and Asio
4. Download SQLite from link above, download the 64-bit (or 32-bit depending on your system) precompiled binaries for your respective OS and the sqlite-amalgamation file
5. Create "sqlite" folder in root
6. Extract the content from the SQLite zips into the root folder, sqlite should contain ONLY the following files (there should be no other folders within sqlite folder): shell.c, sqlite3.c, sqlite3.def, sqlite3.dll, sqlite3.h, sqlite3ext.h
7. Create a file named "CMakeLists.txt"

The CMake is sometimes system-dependent, but here is a copy of mine that should work if pasted into CMakeLists.txt:

---

cmake_minimum_required(VERSION 3.10)
project(group56)

#Set C++ standard (Crow recommends C++14 or later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add Windows API version definition
add_definitions(-D_WIN32_WINNT=0x0601)

#Find Asio library
set(ASIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/asio/include")

#Find Crow library
set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/crow/include")

#SQLite manual include
set(SQLITE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/sqlite")
set(SQLITE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/sqlite")

#Add executable
add_executable(${PROJECT_NAME} 
    backend/Comment.cpp
    backend/Project.cpp
    backend/server.cpp
    backend/Task.cpp
    backend/TodoList.cpp
    backend/User.cpp
    "${SQLITE_SOURCE_DIR}/sqlite3.c"
)

target_link_libraries(group56 PRIVATE ws2_32 mswsock)

#Include Crow headers
target_include_directories(${PROJECT_NAME} PRIVATE
${CROW_INCLUDE_DIR}
${ASIO_INCLUDE_DIR}
${SQLITE_INCLUDE_DIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
ASIO_STANDALONE
)

#Compiler-specific warnings
if(MSVC) # Microsoft Visual C++
target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    # GCC or Clang
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

---

### To run the server and frontend:

1. In root folder, make a folder called "build"
2. In root folder, run the following commands: cd build -> cmake .. -> make
3. Run the generated file "group56.exe". This will run the backend server
4. Go back to root folder, then run the following commands: cd frontend -> npm i -> npm start
5. This should run the frontend website in your browser
6. Done

To access doxygen documentation, go to: html/index.html

If you run into any issues running the program, contact me (lduarte@uwo.ca) or Robin Lee

Here is a youtube link to a video demo:
https://youtu.be/WjS0Eun9Fvw
