project(cheat)

# Source files
set(CHEAT_SOURCES
    src/dllmain.cpp
    src/memory/memory.cpp
    src/memory/signature_scanner.cpp
    src/memory/offset_manager.cpp
    src/entity/entity_manager.cpp
    src/esp/esp_renderer.cpp
    src/esp/world_to_screen.cpp
    src/hooks/hook_manager.cpp
    src/hooks/present_hook.cpp
    src/hooks/wndproc_hook.cpp
    src/ui/menu_manager.cpp
    src/ui/style.cpp
    src/config/config_manager.cpp
    src/future/rage_aimbot.cpp
    src/future/anti_aim.cpp
    src/future/resolver.cpp
)

# Header files
set(CHEAT_HEADERS
    include/memory/memory.h
    include/memory/signature_scanner.h
    include/memory/offset_manager.h
    include/entity/entity_manager.h
    include/esp/esp_renderer.h
    include/hooks/hook_manager.h
    include/ui/menu_manager.h
    include/config/config_manager.h
    include/future/rage_aimbot.h
    include/future/anti_aim.h
    include/future/resolver.h
    include/common/types.h
)

# Create DLL
add_library(cheat SHARED ${CHEAT_SOURCES} ${CHEAT_HEADERS})

# Include directories
target_include_directories(cheat PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/minhook/include
    ${CMAKE_SOURCE_DIR}/external/json/include
)

# Link libraries
target_link_libraries(cheat PRIVATE
    imgui
    minhook
    d3d11
    dxgi
)

# Set output name
set_target_properties(cheat PROPERTIES
    OUTPUT_NAME "cheat"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Use static runtime to avoid VCRUNTIME dependencies
if(MSVC)
    target_compile_options(cheat PRIVATE /MT$<$<CONFIG:Debug>:d>)
endif()

# Visual Studio source groups
source_group("Source Files\\Memory" FILES 
    src/memory/memory.cpp
    src/memory/signature_scanner.cpp
    src/memory/offset_manager.cpp
)
source_group("Source Files\\Entity" FILES src/entity/entity_manager.cpp)
source_group("Source Files\\ESP" FILES 
    src/esp/esp_renderer.cpp
    src/esp/world_to_screen.cpp
)
source_group("Source Files\\Hooks" FILES 
    src/hooks/hook_manager.cpp
    src/hooks/present_hook.cpp
    src/hooks/wndproc_hook.cpp
)
source_group("Source Files\\UI" FILES 
    src/ui/menu_manager.cpp
    src/ui/style.cpp
)
source_group("Source Files\\Config" FILES src/config/config_manager.cpp)
source_group("Source Files\\Future" FILES 
    src/future/rage_aimbot.cpp
    src/future/anti_aim.cpp
    src/future/resolver.cpp
)
source_group("Header Files" FILES ${CHEAT_HEADERS})
