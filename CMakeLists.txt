cmake_minimum_required(VERSION 3.15)
project(tablet_tools)

# Enable C++17 for the entire project
set(CMAKE_CXX_STANDARD 17)

# Regquired cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(TABLET_TOOLS_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}")

find_package(PkgConfig REQUIRED)
find_package(Efi REQUIRED)

# Add usb-control folder
add_subdirectory(usb-control)

# Add uefi-bootengine folder
add_subdirectory(uefi-bootengine)