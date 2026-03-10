# Plan de Implementación: CS2 Premium Menu

## ⚠️ DISCLAIMER LEGAL

**ADVERTENCIA**: Este software es EXCLUSIVAMENTE para propósitos educativos en entornos offline (vs bots). El uso en partidas online resultará en ban permanente de VAC.

---

## Visión General

Este plan implementa un cheat premium para CS2 estilo Neverlose, comenzando con ESP como primera feature funcional. La arquitectura está diseñada para soportar todas las features HvH/rage documentadas en requirements.md.

**Estrategia de Implementación:**
- Fase 1: Core Infrastructure (Launcher, Memory Engine, Hooks)
- Fase 2: ESP System (Primera feature funcional)
- Fase 3: UI System (ImGui con estilo Neverlose)
- Fases futuras: Rage aimbot, anti-aim, resolver, y todas las features HvH

**Lenguaje:** C++ (C++20)
**Arquitectura:** Modular, extensible, optimizada para performance

---

## Tareas


### Fase 1: Core Infrastructure

- [x] 1. Configurar estructura del proyecto
  - Crear estructura de directorios: launcher/, cheat/, external/
  - Configurar CMakeLists.txt para compilación multi-proyecto
  - Integrar dependencias: ImGui, MinHook, nlohmann/json
  - Configurar Visual Studio 2022 con C++20
  - _Requisitos: Todos (base del proyecto)_

- [x] 2. Implementar Process Manager (Launcher)
  - [x] 2.1 Crear clase ProcessManager con detección de proceso CS2
    - Implementar FindCS2Process() usando EnumProcesses
    - Implementar GetProcessHandle() con PROCESS_ALL_ACCESS
    - Implementar GetModuleBase() para client.dll
    - Validar arquitectura x64
    - _Requisitos: 1.1, 1.2, 1.3, 1.4, 1.5, 1.6_
  
  - [ ]* 2.2 Escribir tests unitarios para ProcessManager
    - Test de detección de proceso válido/inválido
    - Test de validación de arquitectura
    - _Requisitos: 1.1-1.6_

- [x] 3. Implementar DLL Injector (Launcher)
  - [x] 3.1 Crear clase DLLInjector con manual mapping
    - Implementar lectura de archivo DLL
    - Implementar asignación de memoria en proceso remoto
    - Implementar resolución de imports (IAT)
    - Implementar reubicación de secciones
    - Implementar llamada a DllMain remoto
    - _Requisitos: 2.1, 2.2, 2.3, 2.4, 2.5_
  
  - [ ]* 3.2 Escribir tests de inyección
    - Test de manual mapping completo
    - Test de resolución de imports
    - _Requisitos: 2.1-2.6_

- [ ] 4. Implementar VAC Bypass (Launcher)
  - [ ] 4.1 Crear clase VACBypass con técnicas de evasión
    - Implementar detección de módulos VAC
    - Implementar parcheo de funciones de verificación
    - Implementar ocultación de threads
    - Implementar protección de memoria (PAGE_EXECUTE_READ)
    - **NOTA:** Por implementar
    - _Requisitos: 3.1, 3.2, 3.3, 3.4, 3.5_
  
  - [ ]* 4.2 Escribir tests de VAC bypass
    - Test de detección de módulos
    - Test de ocultación de memoria
    - _Requisitos: 3.1-3.5_

- [x] 5. Checkpoint - Verificar Launcher funcional
  - Compilar launcher sin errores
  - Verificar detección de CS2
  - Verificar inyección exitosa (DLL vacía de prueba)
  - Asegurar que todos los tests pasan
  - Preguntar al usuario si hay dudas


- [x] 6. Implementar Memory Engine (Cheat DLL)
  - [x] 6.1 Crear clase Memory con lectura/escritura segura
    - Implementar template Read<T>(address)
    - Implementar template Write<T>(address, value)
    - Implementar IsValidAddress() con validación de permisos
    - Implementar GetModuleBase() para módulos del juego
    - Agregar manejo de excepciones para operaciones de memoria
    - _Requisitos: 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7_
  
  - [ ]* 6.2 Escribir tests de Memory Engine
    - Test de lectura/escritura de tipos básicos
    - Test de validación de direcciones
    - _Requisitos: 4.1-4.7_

- [x] 7. Implementar Signature Scanner (Cheat DLL)
  - [x] 7.1 Crear clase SignatureScanner con pattern matching
    - Implementar FindPattern() con algoritmo de búsqueda
    - Implementar ComparePattern() con soporte de máscaras
    - Implementar FindPatternInSection() para secciones específicas
    - Implementar sistema de caché con std::unordered_map
    - Implementar CacheSignature() y GetCachedSignature()
    - _Requisitos: 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7_
  
  - [ ]* 7.2 Escribir property test para pattern scanning
    - **Property 1: Signature encontrada debe coincidir con el patrón**
    - **Valida: Requisitos 5.1, 5.7**
    - Test con patrones conocidos
    - _Requisitos: 5.1, 5.7_

- [x] 8. Implementar Offset Manager (Cheat DLL)
  - [x] 8.1 Crear estructura Offsets y clase OffsetManager
    - Definir estructura Offsets con todos los offsets necesarios
    - Implementar Initialize() con escaneo de signatures
    - Implementar UpdateOffsets() para actualización dinámica
    - Implementar ValidateOffsets() para verificación
    - Agregar thread-safety con std::mutex
    - Incluir offsets para: dwEntityList, dwLocalPlayerController, dwViewMatrix, m_iHealth, m_iTeamNum, m_vecOrigin, m_pGameSceneNode, etc.
    - _Requisitos: 6.1, 6.2, 6.3, 6.4, 6.5, 6.6_
  
  - [ ]* 8.2 Escribir tests de validación de offsets
    - Test de escaneo de signatures críticas
    - Test de validación de offsets
    - _Requisitos: 6.1-6.6_

- [x] 9. Checkpoint - Verificar Memory Engine funcional
  - Compilar cheat DLL sin errores
  - Verificar lectura de memoria del juego
  - Verificar escaneo de signatures exitoso
  - Verificar offsets encontrados y válidos
  - Asegurar que todos los tests pasan
  - Preguntar al usuario si hay dudas


### Fase 2: ESP System (Primera Feature Funcional)

- [x] 10. Implementar Entity Manager
  - [x] 10.1 Crear estructura EntityData y clase EntityManager
    - Definir estructura EntityData con todos los campos necesarios
    - Implementar Update() para iterar entity list
    - Implementar GetPlayers() para obtener jugadores válidos
    - Implementar GetLocalPlayer() para jugador local
    - Implementar IsEntityValid() con validaciones
    - Implementar GetBonePosition() para posiciones de huesos
    - Filtrar entidades dormant y muertas
    - Cachear datos de entidades por frame
    - _Requisitos: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8_
  
  - [ ]* 10.2 Escribir property test para entity validation
    - **Property 2: Entidades válidas deben tener health > 0**
    - **Valida: Requisitos 7.2, 7.3**
    - Test de filtrado de entidades
    - _Requisitos: 7.2, 7.3, 7.4_

- [x] 11. Implementar WorldToScreen
  - [x] 11.1 Crear función WorldToScreen con transformación de coordenadas
    - Implementar lectura de view matrix del juego
    - Implementar transformación a clip space
    - Implementar validación de W (detrás de cámara)
    - Implementar normalización a NDC
    - Implementar conversión a coordenadas de pantalla
    - Validar que el punto está dentro de la pantalla
    - _Requisitos: 9.1, 9.2, 9.3, 9.4, 9.5, 9.6_
  
  - [ ]* 11.2 Escribir property test para WorldToScreen
    - **Property 3: Puntos detrás de la cámara deben retornar false**
    - **Valida: Requisitos 9.3**
    - Test de transformación de coordenadas
    - _Requisitos: 9.1-9.6_

- [x] 12. Implementar ESP Renderer
  - [x] 12.1 Crear estructura ESPConfig y clase ESPRenderer
    - Definir estructura ESPConfig con todas las opciones
    - Implementar Render() para loop principal de ESP
    - Implementar RenderBox() para cajas 2D
    - Implementar RenderName() para nombres de jugadores
    - Implementar RenderHealth() para barras de salud
    - Implementar RenderDistance() para distancia
    - Implementar RenderWeapon() para arma equipada
    - Implementar RenderSkeleton() para esqueleto
    - Implementar RenderSnaplines() para líneas
    - Aplicar team check y filtros configurables
    - Usar colores configurables para enemigos/aliados
    - _Requisitos: 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8, 8.9, 8.10, 8.11_
  
  - [ ]* 12.2 Escribir tests de renderizado ESP
    - Test de cálculo de cajas 2D
    - Test de filtrado por team check
    - _Requisitos: 8.1-8.11_

- [x] 13. Checkpoint - Verificar ESP funcional
  - Compilar con ESP sin errores
  - Verificar detección de entidades
  - Verificar WorldToScreen correcto
  - Verificar renderizado de cajas y nombres
  - Asegurar que todos los tests pasan
  - Preguntar al usuario si hay dudas


### Fase 3: UI System (ImGui)

- [x] 14. Implementar Hook Manager
  - [x] 14.1 Crear clase HookManager con MinHook
    - Implementar Initialize() para setup de MinHook
    - Implementar HookPresent() para DirectX/Vulkan
    - Implementar HookWndProc() para input
    - Implementar UnhookAll() para cleanup
    - Guardar punteros a funciones originales
    - Implementar trampolines para llamadas originales
    - _Requisitos: 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 10.7_
  
  - [ ]* 14.2 Escribir tests de hook manager
    - Test de instalación de hooks
    - Test de restauración de funciones
    - _Requisitos: 10.1-10.7_

- [x] 15. Implementar Present Hook
  - [x] 15.1 Crear función HookedPresent con renderizado
    - Implementar inicialización de ImGui en primer frame
    - Implementar ImGui_ImplDX11_NewFrame() y ImGui_ImplWin32_NewFrame()
    - Implementar llamada a ESPRenderer.Render()
    - Implementar renderizado de menú si está visible
    - Implementar ImGui.Render() y RenderDrawData()
    - Llamar a Present original al final
    - _Requisitos: 10.3_
  
  - [ ]* 15.2 Escribir tests de present hook
    - Test de inicialización de ImGui
    - Test de llamada a función original
    - _Requisitos: 10.3_

- [x] 16. Implementar WndProc Hook
  - [x] 16.1 Crear función HookedWndProc con captura de input
    - Implementar toggle de menú con tecla INSERT
    - Implementar paso de input a ImGui cuando menú visible
    - Implementar bloqueo de input del juego cuando menú activo
    - Llamar a WndProc original al final
    - _Requisitos: 10.3_
  
  - [ ]* 16.2 Escribir tests de wndproc hook
    - Test de captura de teclas
    - Test de toggle de menú
    - _Requisitos: 10.3_

- [x] 17. Implementar Menu Manager
  - [x] 17.1 Crear clase MenuManager con UI completa
    - Implementar Render() para ventana principal
    - Implementar ApplyNeverloseStyle() con colores y estilo
    - Implementar RenderTabBar() con tabs: ESP, Aimbot, Misc, Visuals, Config
    - Implementar RenderESPTab() con todas las opciones ESP
    - Implementar RenderAimbotTab() (placeholder para futuro)
    - Implementar RenderMiscTab() (placeholder para futuro)
    - Implementar RenderVisualsTab() (placeholder para futuro)
    - Implementar RenderConfigTab() con save/load
    - Aplicar estilo Neverlose: colores oscuros, bordes redondeados, acentos morados
    - _Requisitos: 11.1, 11.2, 11.3, 11.4, 11.5, 11.6, 11.7_
  
  - [ ]* 17.2 Escribir tests de menu manager
    - Test de renderizado de tabs
    - Test de aplicación de estilo
    - _Requisitos: 11.1-11.7_

- [x] 18. Checkpoint - Verificar UI funcional
  - Compilar con UI sin errores
  - Verificar menú se abre con INSERT
  - Verificar estilo Neverlose aplicado
  - Verificar todas las opciones ESP funcionan
  - Asegurar que todos los tests pasan
  - Preguntar al usuario si hay dudas


- [x] 19. Implementar Config Manager
  - [x] 19.1 Crear clase ConfigManager con serialización JSON
    - Implementar SaveConfig() con serialización a JSON
    - Implementar LoadConfig() con deserialización desde JSON
    - Implementar GetConfigList() para listar configs disponibles
    - Implementar DeleteConfig() para eliminar configs
    - Implementar SerializeESPConfig() para ESP settings
    - Implementar DeserializeESPConfig() para cargar ESP settings
    - Incluir número de versión en configs
    - Validar versión al cargar configs
    - _Requisitos: 12.1, 12.2, 12.3, 12.4, 12.5, 12.6, 12.7, 12.8_
  
  - [ ]* 19.2 Escribir property test para config serialization
    - **Property 4: Config guardado y cargado debe ser idéntico**
    - **Valida: Requisitos 12.1, 12.2**
    - Test de round-trip serialization
    - _Requisitos: 12.1, 12.2_

- [x] 20. Implementar DllMain y inicialización
  - [x] 20.1 Crear DllMain con secuencia de inicialización completa
    - Implementar DLL_PROCESS_ATTACH handler
    - Crear thread para inicialización asíncrona
    - Inicializar Memory Engine
    - Inicializar Offset Manager y escanear signatures
    - Inicializar Entity Manager
    - Inicializar Hook Manager e instalar hooks
    - Inicializar ESP Renderer
    - Inicializar Menu Manager
    - Cargar configuración por defecto
    - Implementar DLL_PROCESS_DETACH para cleanup
    - _Requisitos: Todos (integración completa)_
  
  - [ ]* 20.2 Escribir tests de integración
    - Test de secuencia de inicialización completa
    - Test de cleanup al desinyectar
    - _Requisitos: Todos_

- [x] 21. Checkpoint Final - Verificar sistema completo
  - Compilar proyecto completo sin errores ni warnings
  - Ejecutar launcher y verificar inyección exitosa
  - Verificar ESP renderiza correctamente en juego
  - Verificar menú funciona con todas las opciones
  - Verificar save/load de configuraciones
  - Verificar performance (FPS no debe bajar >5%)
  - Asegurar que todos los tests pasan
  - Preguntar al usuario si hay dudas o problemas


---

## Preparación para Futuras Features

- [x] 22. Preparar arquitectura para Rage Aimbot
  - [x] 22.1 Crear interfaces y estructuras base para aimbot
    - Definir estructura AimbotConfig con todas las opciones
    - Crear clase RageAimbot con métodos stub
    - Definir interfaces para: target selection, hitbox calculation, multipoint
    - Preparar integración con Entity Manager
    - Documentar requisitos: 13.1-13.10
    - _Requisitos: 13.1-13.10 (preparación)_

- [x] 23. Preparar arquitectura para Anti-Aim
  - [x] 23.1 Crear interfaces y estructuras base para anti-aim
    - Definir estructura AntiAimConfig con todas las opciones
    - Crear clase AntiAim con métodos stub
    - Definir interfaces para: pitch, yaw, desync, fakelag
    - Preparar hooks para manipulación de ángulos
    - Documentar requisitos: 14.1-14.10
    - _Requisitos: 14.1-14.10 (preparación)_

- [x] 24. Preparar arquitectura para Resolver
  - [x] 24.1 Crear interfaces y estructuras base para resolver
    - Definir estructura ResolverConfig con opciones
    - Crear clase Resolver con métodos stub
    - Definir interfaces para: pattern detection, prediction, bruteforce
    - Preparar historial de ángulos por jugador
    - Documentar requisitos: 15.1-15.8
    - _Requisitos: 15.1-15.8 (preparación)_

---

## Fase 5: Skin Changer y Visuals Avanzados

- [ ] 25. Implementar Skin Changer Interactivo
  - [ ] 25.1 Crear sistema de skin changer con UI visual
    - Definir estructura SkinData con información de skins
    - Crear base de datos de skins con nombres, IDs y rareza
    - Implementar SkinChanger clase con métodos de aplicación
    - Implementar lectura de inventario del jugador
    - Implementar escritura de skin IDs en memoria
    - Implementar aplicación de wear (desgaste) personalizado
    - Implementar aplicación de seed (patrón) personalizado
    - Implementar aplicación de StatTrak™
    - Implementar aplicación de stickers personalizados
    - Implementar knife changer (cambio de cuchillo)
    - Implementar glove changer (cambio de guantes)
    - _Requisitos: Nuevos - Skin Changer_
  
  - [ ] 25.2 Crear UI interactiva con imágenes de armas
    - Implementar grid de armas con imágenes/iconos
    - Implementar preview de skin seleccionado
    - Implementar búsqueda y filtrado de skins
    - Implementar categorías (Rifles, Pistols, SMGs, etc.)
    - Implementar filtro por rareza (Consumer, Industrial, Mil-Spec, etc.)
    - Implementar selector de wear con slider visual
    - Implementar selector de seed con preview
    - Implementar selector de StatTrak™ counter
    - Implementar sistema de favoritos
    - Usar ImGui::Image() para mostrar iconos de armas
    - _Requisitos: Nuevos - Skin Changer UI_
  
  - [ ]* 25.3 Escribir tests de skin changer
    - Test de aplicación de skins
    - Test de validación de IDs
    - _Requisitos: Skin Changer_

- [ ] 26. Implementar Tercera Persona (Thirdperson)
  - [ ] 26.1 Crear sistema de tercera persona
    - Implementar detección de offset de cámara
    - Implementar manipulación de distancia de cámara
    - Implementar toggle de tercera persona con hotkey
    - Implementar ajuste de distancia con slider
    - Implementar ajuste de ángulo de cámara
    - Implementar modo "alive only" (solo cuando vivo)
    - Implementar modo "spectator" (en espectador)
    - Implementar suavizado de transición de cámara
    - Agregar opción en tab Visuals del menú
    - _Requisitos: Nuevos - Thirdperson_
  
  - [ ]* 26.2 Escribir tests de thirdperson
    - Test de manipulación de cámara
    - Test de restauración de valores originales
    - _Requisitos: Thirdperson_

- [ ] 27. Checkpoint - Verificar Skin Changer y Thirdperson
  - Compilar con nuevas features sin errores
  - Verificar skin changer aplica skins correctamente
  - Verificar UI muestra imágenes de armas
  - Verificar tercera persona funciona correctamente
  - Verificar ajustes de distancia y ángulo
  - Asegurar que todos los tests pasan
  - Preguntar al usuario si hay dudas

---

## Notas de Implementación

### Estructura de Archivos

```
cs2-premium-menu/
├── launcher/
│   ├── src/
│   │   ├── main.cpp                    # Entry point del launcher
│   │   ├── process_manager.cpp         # Tarea 2
│   │   ├── dll_injector.cpp            # Tarea 3
│   │   └── vac_bypass.cpp              # Tarea 4
│   ├── include/
│   │   ├── process_manager.h
│   │   ├── dll_injector.h
│   │   └── vac_bypass.h
│   └── CMakeLists.txt
├── cheat/
│   ├── src/
│   │   ├── dllmain.cpp                 # Tarea 20
│   │   ├── memory/
│   │   │   ├── memory.cpp              # Tarea 6
│   │   │   ├── signature_scanner.cpp   # Tarea 7
│   │   │   └── offset_manager.cpp      # Tarea 8
│   │   ├── entity/
│   │   │   └── entity_manager.cpp      # Tarea 10
│   │   ├── esp/
│   │   │   └── esp_renderer.cpp        # Tarea 12
│   │   ├── hooks/
│   │   │   ├── hook_manager.cpp        # Tarea 14
│   │   │   ├── present_hook.cpp        # Tarea 15
│   │   │   └── wndproc_hook.cpp        # Tarea 16
│   │   ├── ui/
│   │   │   ├── menu_manager.cpp        # Tarea 17
│   │   │   └── style.cpp               # Parte de tarea 17
│   │   ├── config/
│   │   │   └── config_manager.cpp      # Tarea 19
│   │   ├── skins/
│   │   │   └── skin_changer.cpp        # Tarea 25 (nuevo)
│   │   ├── visuals/
│   │   │   └── thirdperson.cpp         # Tarea 26 (nuevo)
│   │   └── future/
│   │       ├── rage_aimbot.cpp         # Tarea 22 (stub)
│   │       ├── anti_aim.cpp            # Tarea 23 (stub)
│   │       └── resolver.cpp            # Tarea 24 (stub)
│   ├── include/
│   └── CMakeLists.txt
├── external/
│   ├── imgui/                          # Tarea 1
│   ├── minhook/                        # Tarea 1
│   └── json/                           # Tarea 1
└── CMakeLists.txt                      # Tarea 1
```

### Dependencias entre Tareas

- Tareas 2-4 (Launcher) son independientes entre sí pero deben completarse antes de tarea 5
- Tareas 6-8 (Memory Engine) deben completarse en orden
- Tarea 10 (Entity Manager) depende de tareas 6-8
- Tarea 11 (WorldToScreen) depende de tarea 6
- Tarea 12 (ESP Renderer) depende de tareas 10-11
- Tareas 14-16 (Hooks) deben completarse en orden
- Tarea 17 (Menu Manager) depende de tareas 14-16
- Tarea 19 (Config Manager) puede hacerse en paralelo con otras
- Tarea 20 (DllMain) integra todas las anteriores
- Tareas 22-24 (Preparación) pueden hacerse después de tarea 21

### Testing

- Tests marcados con `*` son opcionales pero recomendados
- Usar Google Test (gtest) para unit tests
- Property-based tests usan generadores aleatorios para validar propiedades universales
- Checkpoints (tareas 5, 9, 13, 18, 21) son momentos para validar manualmente

### Performance

- Target: No reducir FPS del juego en más de 5%
- Memory usage: <50MB adicional
- CPU usage: <5% de un core
- Optimizar loops de entity iteration
- Cachear datos por frame, no leer múltiples veces

### Seguridad

- **CRÍTICO:** Este código es SOLO educativo
- NO usar en partidas online
- NO usar en servidores VAC-secured
- Implementar logging extensivo para debugging
- Todas las técnicas anti-cheat son solo para aprendizaje

### Offsets y Signatures

- Los offsets cambian con cada update de CS2
- Mantener lista de signatures actualizadas
- Implementar fallbacks con offsets hardcoded
- Documentar versión del juego para cada set de offsets

---

## Próximos Pasos Después de Este Plan

Una vez completadas todas las tareas de este plan, el sistema tendrá:
- ✅ Launcher funcional con inyección segura
- ✅ ESP completo con todas las opciones
- ✅ UI premium estilo Neverlose
- ✅ Sistema de configuración
- ✅ Arquitectura preparada para features HvH

**Próximas implementaciones (Fase 5):**
- **Tarea 25:** Skin Changer interactivo con UI visual e imágenes de armas
- **Tarea 26:** Tercera persona (Thirdperson) con ajustes de distancia y ángulo
- **Tarea 27:** Checkpoint de validación

Las siguientes fases (según requirements.md) serán:
- **Fase 6:** Rage Aimbot completo (Requisitos 13.1-13.10)
- **Fase 7:** Anti-Aim y evasión (Requisitos 14.1-14.10)
- **Fase 8:** Resolver y backtrack (Requisitos 15.1-15.8, 16.1-16.6)
- **Fase 9:** Exploits (Double Tap, Hide Shots) (Requisitos 17.1-17.7)
- **Fases 10-14:** Todas las demás features HvH documentadas

Cada fase futura seguirá el mismo patrón:
1. Implementar core functionality
2. Agregar tests (opcionales)
3. Integrar con UI
4. Checkpoint de validación

---

## Recordatorio Final

⚠️ **ESTE SOFTWARE ES EXCLUSIVAMENTE EDUCATIVO**

- Solo para aprendizaje de ingeniería inversa
- Solo para uso en partidas offline vs bots
- El uso en online resultará en ban permanente
- El desarrollador NO se hace responsable del mal uso

**Buena suerte con la implementación!** 🚀

