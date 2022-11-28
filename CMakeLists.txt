cmake_minimum_required(VERSION 3.23)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        # Important: Specify a GIT_TAG XXXXX here.
        )

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------


#------------------------------------- SELF ----------------------------
project(perf_tui
        LANGUAGES CXX
        VERSION 1.0.0)
set(CMAKE_CXX_FLAGS "-O3")
add_executable(perf_tui main.cpp)

add_library(sysmonitor sysmonitor/cpu.cpp sysmonitor/cpu.h sysmonitor/machine.cpp sysmonitor/machine.h)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(sysmonitor PROPERTIES CXX_STANDARD 20)
#-----------------------------------------------------------------------

target_link_libraries(perf_tui
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component)