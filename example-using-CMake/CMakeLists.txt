cmake_minimum_required(VERSION 2.8)
project(untitled12)
set(TARGET_NAME ${PROJECT_NAME})

set(INC_DIR1
    /usr/include/dbus-1.0
   /usr/lib/x86_64-linux-gnu/dbus-1.0/include
    )

set(INC_DIR2
    /usr/lib/x86_64-linux-gnu/dbus-1.0/include
    )

include_directories(
    ${INC_DIR1}
   # ${INC_DIR2}
    )

add_executable(myserver "myserver.cpp")
add_executable(myclient "myclient.cpp")

target_link_libraries(myserver
    dbus-1 )


target_link_libraries(myclient
    dbus-1 )

