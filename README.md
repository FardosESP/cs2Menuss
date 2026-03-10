# Fardos.cc - CS2 Premium Menu

## ⚠️ ADVERTENCIA LEGAL

**ADVERTENCIA CRÍTICA**: Este software está diseñado EXCLUSIVAMENTE para propósitos educativos en entornos controlados (partidas offline vs bots).

- ❌ NO permitido para uso en partidas online
- ❌ NO permitido en servidores VAC-secured
- ❌ El uso online resultará en ban permanente
- ✅ Solo para aprendizaje de ingeniería inversa
- ✅ Solo para uso en partidas locales vs bots

**El desarrollador NO se hace responsable del mal uso de este software.**

## Descripción General

Menú premium para CS2 inspirado en Neverlose, construido con arquitectura modular para propósitos educativos. Este proyecto demuestra técnicas avanzadas de game hacking incluyendo:

- Manipulación de memoria y pattern scanning
- Hooking de DirectX y renderizado de overlay
- Sistema de entidades y ESP (Extra Sensory Perception)
- UI premium basada en ImGui
- Arquitectura completa de features HvH (Head vs Head)

## Estructura del Proyecto

```
cs2-premium-menu/
├── launcher/          # Aplicación inyectora externa
├── cheat/            # DLL principal del cheat
├── external/         # Dependencias de terceros
└── .kiro/specs/      # Documentación de diseño
```

## Features Implementadas

### ✅ Fase 1: Core Infrastructure (COMPLETO)
- Detección y gestión de procesos
- Inyección DLL con manual mapping
- Técnicas de VAC bypass (solo educativo)
- Motor de memoria con lectura/escritura segura
- Scanner de signatures con caché
- Gestión dinámica de offsets

### ✅ Fase 2: ESP System (COMPLETO)
- Detección y tracking de entidades
- Transformación WorldToScreen
- Box ESP (2D)
- Información de jugadores (nombre, salud, distancia)
- Skeleton ESP
- Snaplines
- Filtrado por equipo

### ✅ Fase 3: UI System (COMPLETO)
- Integración de ImGui con hooks de DirectX
- Estilo inspirado en Neverlose
- Sistema de configuración con JSON
- Menú multi-tab (Legit, Rage, Anti-Aim, Visuals, Misc, Skins, Config)

### ✅ Fase 4: Config System (COMPLETO)
- Guardado/carga de configuraciones en JSON
- Gestión de múltiples configs
- Versionado de configs
- Serialización de todas las opciones

### 🔜 Fases Futuras (Documentadas)
- Rage Aimbot con multipoint
- Anti-Aim (pitch, yaw, desync, fakelag)
- Resolver para romper anti-aim enemigo
- Backtrack
- Exploits (double tap, hide shots)
- Auto duck, auto peek
- Y 60+ features HvH más

## Requisitos

- Windows 10/11 (x64)
- Visual Studio 2022 con soporte C++20
- CMake 3.20+
- Counter-Strike 2 (para testing en modo offline solamente)

## Dependencias

- **ImGui** v1.89+ (MIT License) - Renderizado de UI
- **MinHook** v1.3.3+ (BSD 2-Clause) - Function hooking
- **nlohmann/json** v3.11+ (MIT License) - Serialización de configuración

## Compilación

1. Clonar el repositorio con submodules:
```bash
git clone --recursive https://github.com/yourusername/cs2-premium-menu.git
cd cs2-premium-menu
```

2. Inicializar dependencias externas:
```bash
# Descargar ImGui, MinHook, y nlohmann/json al directorio external/
# Ver external/README.md para instrucciones
```

3. Generar archivos de build:
```bash
mkdir build
cd build
cmake ..
```

4. Compilar el proyecto:
```bash
cmake --build . --config Release
```

5. Los archivos de salida estarán en `build/bin/`:
   - `CS2Launcher.exe` - Inyector
   - `cheat.dll` - Librería principal del cheat

## Uso (Solo Educativo)

1. Iniciar CS2 en modo offline con bots
2. Ejecutar `CS2Launcher.exe` como Administrador
3. El launcher detectará CS2 e inyectará el cheat
4. Presionar `INSERT` para abrir el menú en el juego
5. Configurar opciones de ESP en el menú
6. Presionar `INSERT` nuevamente para cerrar el menú
7. Presionar `END` para descargar el cheat

## Controles

- **INSERT** - Abrir/cerrar menú
- **END** - Descargar cheat

## Configuraciones

Las configuraciones se guardan en:
```
%APPDATA%/Fardos.cc/configs/
```

Puedes guardar múltiples configuraciones y cargarlas desde el tab Config del menú.

## Documentación

Documentación técnica completa disponible en `.kiro/specs/cs2-premium-menu/`:
- `design.md` - Arquitectura técnica y algoritmos
- `requirements.md` - 80 requisitos funcionales
- `tasks.md` - Plan de implementación

## Desarrollo

### Estilo de Código
- Estándar C++20
- Prácticas modernas de C++ (RAII, smart pointers, etc.)
- Convenciones de nombres claras
- Comentarios comprensivos para algoritmos complejos

### Testing
- Unit tests para componentes críticos
- Property-based tests para algoritmos
- Testing manual en entorno offline

### Objetivos de Performance
- Impacto en FPS: <5% de reducción
- Uso de memoria: <50MB adicional
- Uso de CPU: <5% de un core
- Tiempo de inicio: <2 segundos desde inyección

## Consideraciones de Seguridad

Este proyecto demuestra técnicas de evasión de anti-cheat para propósitos educativos:
- Manual mapping para evitar detección de LoadLibrary
- Ofuscación de strings
- Ocultación de imports
- Ocultación de threads
- Protección de memoria

**Estas técnicas son solo para aprendizaje. Usarlas en juegos online es poco ético e ilegal.**

## Contribuciones

Este es un proyecto educativo. Las contribuciones deben enfocarse en:
- Mejoras de calidad de código
- Mejoras de documentación
- Corrección de bugs
- Contenido educativo

**No contribuir features diseñadas para cheating online.**

## Licencia

Este proyecto es solo para propósitos educativos. Ver archivo LICENSE para detalles.

## Agradecimientos

- Valve Corporation por Counter-Strike 2
- ImGui por Omar Cornut
- MinHook por Tsuda Kageyu
- nlohmann/json por Niels Lohmann
- La comunidad de game hacking por recursos educativos

## Disclaimer (De Nuevo)

**ESTE SOFTWARE ES SOLO PARA PROPÓSITOS EDUCATIVOS.**

Usar este software en partidas online viola:
- Steam Subscriber Agreement
- Counter-Strike 2 Terms of Service
- Potencialmente leyes locales sobre fraude informático

**Has sido advertido. Usa responsable y éticamente.**

---

*Construido con ❤️ para aprender, no para hacer trampa.*
