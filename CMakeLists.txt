cmake_minimum_required(VERSION 3.10)
project(TicketSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

# Source files
set(SOURCES
    main.cpp
    user.cpp
    train.cpp
    order.cpp
    utils.cpp
)

# Header files
set(HEADERS
    utils.h
    user.h
    train.h
    order.h
)

# Create executable
add_executable(code ${SOURCES} ${HEADERS})

# Set output name explicitly to 'code'
set_target_properties(code PROPERTIES OUTPUT_NAME "code")