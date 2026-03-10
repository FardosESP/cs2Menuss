# External Dependencies

This directory contains third-party libraries required for the project.

## Required Libraries

### 1. ImGui (v1.89+)
**Purpose:** UI rendering  
**License:** MIT  
**Repository:** https://github.com/ocornut/imgui

**Installation:**
```bash
cd external
git clone https://github.com/ocornut/imgui.git
cd imgui
git checkout v1.89.9  # or latest stable version
```

**Required files:**
- `imgui/*.cpp` and `imgui/*.h`
- `imgui/backends/imgui_impl_dx11.*`
- `imgui/backends/imgui_impl_win32.*`

### 2. MinHook (v1.3.3+)
**Purpose:** Function hooking  
**License:** BSD 2-Clause  
**Repository:** https://github.com/TsudaKageyu/minhook

**Installation:**
```bash
cd external
git clone https://github.com/TsudaKageyu/minhook.git
cd minhook
git checkout v1.3.3  # or latest stable version
```

**Required files:**
- `minhook/include/MinHook.h`
- `minhook/src/*.c` and `minhook/src/*.h`

### 3. nlohmann/json (v3.11+)
**Purpose:** JSON serialization/deserialization  
**License:** MIT  
**Repository:** https://github.com/nlohmann/json

**Installation:**
```bash
cd external
git clone https://github.com/nlohmann/json.git
cd json
git checkout v3.11.2  # or latest stable version
```

**Required files:**
- `json/include/nlohmann/json.hpp` (header-only library)

## Directory Structure

After installation, your `external/` directory should look like:

```
external/
├── CMakeLists.txt
├── README.md
├── imgui/
│   ├── imgui.cpp
│   ├── imgui.h
│   ├── backends/
│   │   ├── imgui_impl_dx11.cpp
│   │   ├── imgui_impl_dx11.h
│   │   ├── imgui_impl_win32.cpp
│   │   └── imgui_impl_win32.h
│   └── ...
├── minhook/
│   ├── include/
│   │   └── MinHook.h
│   ├── src/
│   │   ├── buffer.c
│   │   ├── hook.c
│   │   ├── trampoline.c
│   │   └── hde/
│   └── ...
└── json/
    ├── include/
    │   └── nlohmann/
    │       └── json.hpp
    └── ...
```

## Alternative: Git Submodules

You can also add these as git submodules:

```bash
git submodule add https://github.com/ocornut/imgui.git external/imgui
git submodule add https://github.com/TsudaKageyu/minhook.git external/minhook
git submodule add https://github.com/nlohmann/json.git external/json
git submodule update --init --recursive
```

## Verification

After installation, verify all files are present:

```bash
# Check ImGui
ls external/imgui/imgui.cpp
ls external/imgui/backends/imgui_impl_dx11.cpp

# Check MinHook
ls external/minhook/include/MinHook.h
ls external/minhook/src/hook.c

# Check nlohmann/json
ls external/json/include/nlohmann/json.hpp
```

## License Information

All dependencies are used under their respective open-source licenses:
- **ImGui:** MIT License
- **MinHook:** BSD 2-Clause License
- **nlohmann/json:** MIT License

Please review each library's license file before use.
