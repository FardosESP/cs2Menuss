# Fardos.cc - CS2 Premium Menu

Premium menu for CS2 inspired by Neverlose, built with modular architecture. This project demonstrates advanced game hacking techniques including memory manipulation, pattern scanning, DirectX hooking, and overlay rendering.

## Features

### Core Infrastructure
- Process detection and management
- DLL injection with manual mapping
- Memory engine with safe read/write operations
- Signature scanner with caching
- Dynamic offset management

### ESP System
- Entity detection and tracking
- WorldToScreen transformation
- Box ESP (2D)
- Player information (name, health, distance)
- Skeleton ESP
- Snaplines
- Team filtering

### UI System
- ImGui integration with DirectX hooks
- Neverlose-inspired styling
- JSON configuration system
- Multi-tab menu (Legit, Rage, Anti-Aim, Visuals, Misc, Skins, Config)

### Config System
- Save/load configurations in JSON
- Multiple config management
- Config versioning
- Serialization of all options

### Future Features (Documented)
- Rage Aimbot with multipoint
- Anti-Aim (pitch, yaw, desync, fakelag)
- Resolver for breaking enemy anti-aim
- Backtrack
- Exploits (double tap, hide shots)
- Auto duck, auto peek
- 60+ additional HvH features

## Project Structure

```
cs2-premium-menu/
├── launcher/          # External injector application
├── cheat/            # Main cheat DLL
├── external/         # Third-party dependencies
└── .kiro/specs/      # Design documentation
```

## Requirements

- Windows 10/11 (x64)
- Visual Studio 2022 with C++20 support
- CMake 3.20+
- Counter-Strike 2

## Dependencies

- **ImGui** v1.89+ (MIT License) - UI rendering
- **MinHook** v1.3.3+ (BSD 2-Clause) - Function hooking
- **nlohmann/json** v3.11+ (MIT License) - Configuration serialization

## Building

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/FardosESP/cs2Menuss.git
cd cs2Menuss
```

2. Initialize external dependencies:
```bash
# Download ImGui, MinHook, and nlohmann/json to external/
# See external/README.md for instructions
```

3. Generate build files:
```bash
mkdir build
cd build
cmake ..
```

4. Compile the project:
```bash
cmake --build . --config Release
```

5. Output files will be in `build/bin/`:
   - `CS2Launcher.exe` - Injector
   - `cheat.dll` - Main cheat library

## Usage

1. Launch CS2
2. Run `CS2Launcher.exe` as Administrator
3. The launcher will detect CS2 and inject the cheat
4. Press `INSERT` to open the menu in-game
5. Configure ESP options in the menu
6. Press `INSERT` again to close the menu
7. Press `END` to unload the cheat

## Controls

- **INSERT** - Open/close menu
- **END** - Unload cheat

## Configuration

Configurations are saved in:
```
%APPDATA%/Fardos.cc/configs/
```

You can save multiple configurations and load them from the Config tab in the menu.

## Documentation

Complete technical documentation available in `.kiro/specs/cs2-premium-menu/`:
- `design.md` - Technical architecture and algorithms
- `requirements.md` - 80 functional requirements
- `tasks.md` - Implementation plan

## Development

### Code Style
- C++20 standard
- Modern C++ practices (RAII, smart pointers, etc.)
- Clear naming conventions
- Comprehensive comments for complex algorithms

### Testing
- Unit tests for critical components
- Property-based tests for algorithms
- Manual testing in offline environment

### Performance Goals
- FPS impact: <5% reduction
- Memory usage: <50MB additional
- CPU usage: <5% of one core
- Startup time: <2 seconds from injection

## Security Considerations

This project demonstrates anti-cheat evasion techniques:
- Manual mapping to avoid LoadLibrary detection
- String obfuscation
- Import hiding
- Thread hiding
- Memory protection

## Contributing

Contributions focused on:
- Code quality improvements
- Documentation enhancements
- Bug fixes
- Educational content

## License

This project is licensed under the MIT License. See LICENSE file for details.

## Acknowledgments

- Valve Corporation for Counter-Strike 2
- ImGui by Omar Cornut
- MinHook by Tsuda Kageyu
- nlohmann/json by Niels Lohmann
- The game hacking community for educational resources

---

*Built with ❤️ for learning*
