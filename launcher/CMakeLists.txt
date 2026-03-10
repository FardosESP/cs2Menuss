project(launcher)

# Source files
set(LAUNCHER_SOURCES
    src/main.cpp
    src/process_manager.cpp
    src/dll_injector.cpp
    src/vac_bypass.cpp
)

# Header files
set(LAUNCHER_HEADERS
    include/process_manager.h
    include/dll_injector.h
    include/vac_bypass.h
)

# Create executable
add_executable(launcher ${LAUNCHER_SOURCES} ${LAUNCHER_HEADERS})

# Include directories
target_include_directories(launcher PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(launcher PRIVATE
    psapi
    ntdll
)

# Set output name
set_target_properties(launcher PROPERTIES
    OUTPUT_NAME "CS2Launcher"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Visual Studio source groups
source_group("Source Files" FILES ${LAUNCHER_SOURCES})
source_group("Header Files" FILES ${LAUNCHER_HEADERS})
