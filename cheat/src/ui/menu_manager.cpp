// Menu Manager - Task 17
// Professional menu with Neverlose style

#include "ui/menu_manager.h"
#include "imgui.h"
#include <iostream>

void MenuManager::Render() {
    if (!visible) return;
    
    // Apply Neverlose style on first render
    static bool styleApplied = false;
    if (!styleApplied) {
        ApplyNeverloseStyle();
        styleApplied = true;
    }
    
    // Main menu window
    ImGui::SetNextWindowSize(ImVec2(750, 550), ImGuiCond_FirstUseEver);
    ImGui::Begin("Fardos.cc - Premium CS2 Cheat", nullptr, 
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    // Tab bar
    RenderTabBar();
    
    // Render current tab content
    ImGui::BeginChild("TabContent", ImVec2(0, 0), true);
    
    switch (currentTab) {
        case MenuTab::LEGIT:
            RenderLegitTab();
            break;
        case MenuTab::RAGE:
            RenderRageTab();
            break;
        case MenuTab::ANTIAIM:
            RenderAntiAimTab();
            break;
        case MenuTab::VISUALS:
            RenderVisualsTab();
            break;
        case MenuTab::MISC:
            RenderMiscTab();
            break;
        case MenuTab::SKINS:
            RenderSkinsTab();
            break;
        case MenuTab::CONFIG:
            RenderConfigTab();
            break;
    }
    
    ImGui::EndChild();
    ImGui::End();
}

void MenuManager::SetVisible(bool vis) {
    visible = vis;
}

bool MenuManager::IsVisible() {
    return visible;
}

void MenuManager::RenderTabBar() {
    if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Legit")) {
            currentTab = MenuTab::LEGIT;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Rage")) {
            currentTab = MenuTab::RAGE;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Anti-Aim")) {
            currentTab = MenuTab::ANTIAIM;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Visuals")) {
            currentTab = MenuTab::VISUALS;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Misc")) {
            currentTab = MenuTab::MISC;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Skins")) {
            currentTab = MenuTab::SKINS;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Config")) {
            currentTab = MenuTab::CONFIG;
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void MenuManager::RenderLegitTab() {
    ImGui::Text("Legit Aimbot");
    ImGui::Separator();
    
    static bool legitAimbotEnabled = false;
    static float legitFOV = 2.0f;
    static float legitSmooth = 5.0f;
    static int legitBone = 0;
    static bool legitRCS = true;
    static float legitRCSAmount = 100.0f;
    static bool legitTriggerbot = false;
    static float legitTriggerDelay = 50.0f;
    
    ImGui::Checkbox("Enable Legit Aimbot", &legitAimbotEnabled);
    
    if (legitAimbotEnabled) {
        ImGui::Spacing();
        ImGui::Text("Aim Settings");
        ImGui::SliderFloat("FOV", &legitFOV, 0.5f, 10.0f);
        ImGui::SliderFloat("Smooth", &legitSmooth, 1.0f, 20.0f);
        
        const char* bones[] = {"Head", "Neck", "Chest", "Stomach"};
        ImGui::Combo("Target Bone", &legitBone, bones, IM_ARRAYSIZE(bones));
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Recoil Control");
        ImGui::Checkbox("RCS (Recoil Control)", &legitRCS);
        
        if (legitRCS) {
            ImGui::SliderFloat("RCS Amount", &legitRCSAmount, 0.0f, 100.0f, "%.0f%%");
        }
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Triggerbot");
    
    ImGui::Checkbox("Enable Triggerbot", &legitTriggerbot);
    
    if (legitTriggerbot) {
        ImGui::SliderFloat("Delay (ms)", &legitTriggerDelay, 0.0f, 200.0f);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("ESP (Legit Mode)");
    
    static bool legitESP = true;
    static bool legitBox = true;
    static bool legitName = true;
    static bool legitHealth = true;
    static bool legitDistance = false;
    static bool legitWeapon = true;
    static bool legitTeamCheck = true;
    
    ImGui::Checkbox("Enable ESP", &legitESP);
    
    if (legitESP) {
        ImGui::Checkbox("Box", &legitBox);
        ImGui::Checkbox("Name", &legitName);
        ImGui::Checkbox("Health Bar", &legitHealth);
        ImGui::Checkbox("Distance", &legitDistance);
        ImGui::Checkbox("Weapon", &legitWeapon);
        ImGui::Checkbox("Team Check", &legitTeamCheck);
    }
}

void MenuManager::RenderRageTab() {
    ImGui::Text("Rage Aimbot");
    ImGui::Separator();
    
    static bool rageEnabled = false;
    static float rageFOV = 180.0f;
    static bool rageMultipoint = true;
    static int rageMultipointScale = 85;
    static bool rageBaim = false;
    static bool rageSafepoint = true;
    static bool rageAutoFire = true;
    static bool rageAutoScope = true;
    static bool rageAutoWall = true;
    static int rageMinDamage = 40;
    
    ImGui::Checkbox("Enable Rage Aimbot", &rageEnabled);
    
    if (rageEnabled) {
        ImGui::Spacing();
        ImGui::Text("Target Selection");
        ImGui::SliderFloat("FOV", &rageFOV, 1.0f, 180.0f);
        
        const char* targetSelection[] = {"FOV", "Distance", "Health", "Damage"};
        static int targetMode = 0;
        ImGui::Combo("Priority", &targetMode, targetSelection, IM_ARRAYSIZE(targetSelection));
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Hitbox Selection");
        
        static bool hitboxHead = true;
        static bool hitboxChest = true;
        static bool hitboxStomach = true;
        static bool hitboxPelvis = false;
        static bool hitboxArms = false;
        static bool hitboxLegs = false;
        
        ImGui::Checkbox("Head", &hitboxHead);
        ImGui::SameLine();
        ImGui::Checkbox("Chest", &hitboxChest);
        ImGui::SameLine();
        ImGui::Checkbox("Stomach", &hitboxStomach);
        
        ImGui::Checkbox("Pelvis", &hitboxPelvis);
        ImGui::SameLine();
        ImGui::Checkbox("Arms", &hitboxArms);
        ImGui::SameLine();
        ImGui::Checkbox("Legs", &hitboxLegs);
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Accuracy");
        
        ImGui::Checkbox("Multipoint", &rageMultipoint);
        if (rageMultipoint) {
            ImGui::SliderInt("Multipoint Scale", &rageMultipointScale, 50, 100, "%d%%");
        }
        
        ImGui::Checkbox("Body Aim (Baim)", &rageBaim);
        ImGui::Checkbox("Safe Point", &rageSafepoint);
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Automation");
        
        ImGui::Checkbox("Auto Fire", &rageAutoFire);
        ImGui::Checkbox("Auto Scope", &rageAutoScope);
        ImGui::Checkbox("Auto Wall", &rageAutoWall);
        
        if (rageAutoWall) {
            ImGui::SliderInt("Min Damage", &rageMinDamage, 1, 100);
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Resolver");
        
        static bool resolverEnabled = true;
        static bool resolverBruteforce = false;
        
        ImGui::Checkbox("Enable Resolver", &resolverEnabled);
        ImGui::Checkbox("Bruteforce", &resolverBruteforce);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Exploits");
    
    static bool doubleTap = false;
    static bool hideShots = false;
    
    ImGui::Checkbox("Double Tap", &doubleTap);
    ImGui::Checkbox("Hide Shots", &hideShots);
}

void MenuManager::RenderAntiAimTab() {
    ImGui::Text("Anti-Aim Configuration");
    ImGui::Separator();
    
    static bool antiAimEnabled = false;
    
    ImGui::Checkbox("Enable Anti-Aim", &antiAimEnabled);
    
    if (antiAimEnabled) {
        ImGui::Spacing();
        ImGui::Text("Pitch");
        
        const char* pitchModes[] = {"None", "Down", "Up", "Zero", "Fake Up", "Fake Down"};
        static int pitchMode = 1;
        ImGui::Combo("Pitch Mode", &pitchMode, pitchModes, IM_ARRAYSIZE(pitchModes));
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Yaw");
        
        const char* yawModes[] = {"None", "Backward", "Spin", "Jitter", "Sideways", "Random"};
        static int yawMode = 1;
        ImGui::Combo("Yaw Mode", &yawMode, yawModes, IM_ARRAYSIZE(yawModes));
        
        static float yawOffset = 0.0f;
        ImGui::SliderFloat("Yaw Offset", &yawOffset, -180.0f, 180.0f);
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Desync");
        
        static bool desyncEnabled = true;
        static int desyncAmount = 60;
        static bool desyncJitter = false;
        
        ImGui::Checkbox("Enable Desync", &desyncEnabled);
        
        if (desyncEnabled) {
            ImGui::SliderInt("Desync Amount", &desyncAmount, 0, 100, "%d%%");
            ImGui::Checkbox("Jitter Desync", &desyncJitter);
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Fake Lag");
        
        static bool fakelagEnabled = true;
        static int fakelagAmount = 14;
        static int fakelagMode = 0;
        
        ImGui::Checkbox("Enable Fake Lag", &fakelagEnabled);
        
        if (fakelagEnabled) {
            const char* fakelagModes[] = {"Static", "Adaptive", "Random", "Break LC"};
            ImGui::Combo("Fake Lag Mode", &fakelagMode, fakelagModes, IM_ARRAYSIZE(fakelagModes));
            ImGui::SliderInt("Fake Lag Amount", &fakelagAmount, 1, 14);
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Special");
        
        static bool fakeDuck = false;
        static bool slowWalk = false;
        
        ImGui::Checkbox("Fake Duck", &fakeDuck);
        ImGui::Checkbox("Slow Walk", &slowWalk);
    }
}

void MenuManager::RenderVisualsTab() {
    ImGui::Text("Glow ESP");
    ImGui::Separator();
    
    static bool glowEnabled = false;
    static float glowColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    static float glowTeamColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    static float glowIntensity = 1.0f;
    static bool glowTeamCheck = true;
    
    ImGui::Checkbox("Enable Glow", &glowEnabled);
    
    if (glowEnabled) {
        ImGui::ColorEdit4("Enemy Glow Color", glowColor);
        ImGui::ColorEdit4("Team Glow Color", glowTeamColor);
        ImGui::SliderFloat("Glow Intensity", &glowIntensity, 0.1f, 2.0f);
        ImGui::Checkbox("Team Check##Glow", &glowTeamCheck);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Chams (Material Override)");
    
    static bool chamsEnabled = false;
    static int chamsMaterial = 0;
    static bool chamsXQZ = true;
    static float chamsVisibleColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    static float chamsOccludedColor[4] = {1.0f, 0.0f, 0.0f, 0.5f};
    static bool chamsTeamCheck = true;
    
    ImGui::Checkbox("Enable Chams", &chamsEnabled);
    
    if (chamsEnabled) {
        const char* materials[] = {"Flat", "Textured", "Metallic", "Glow", "Glass"};
        ImGui::Combo("Material", &chamsMaterial, materials, IM_ARRAYSIZE(materials));
        
        ImGui::Checkbox("XQZ (Through Walls)", &chamsXQZ);
        
        if (chamsXQZ) {
            ImGui::ColorEdit4("Visible Color", chamsVisibleColor);
            ImGui::ColorEdit4("Occluded Color", chamsOccludedColor);
        } else {
            ImGui::ColorEdit4("Chams Color", chamsVisibleColor);
        }
        
        ImGui::Checkbox("Team Check##Chams", &chamsTeamCheck);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Removals");
    
    static bool noFlash = false;
    static bool noSmoke = false;
    static bool noFog = false;
    static bool noRecoil = false;
    
    ImGui::Checkbox("No Flash", &noFlash);
    ImGui::Checkbox("No Smoke", &noSmoke);
    ImGui::Checkbox("No Fog", &noFog);
    ImGui::Checkbox("No Visual Recoil", &noRecoil);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("FOV & Viewmodel");
    
    static bool fovChangerEnabled = false;
    static float fovValue = 90.0f;
    static float viewmodelFOV = 68.0f;
    
    ImGui::Checkbox("FOV Changer", &fovChangerEnabled);
    
    if (fovChangerEnabled) {
        ImGui::SliderFloat("FOV", &fovValue, 60.0f, 140.0f);
        ImGui::SliderFloat("Viewmodel FOV", &viewmodelFOV, 54.0f, 90.0f);
    }
}

void MenuManager::RenderMiscTab() {
    ImGui::Text("Movement");
    ImGui::Separator();
    
    static bool bunnyHop = false;
    static bool autoStrafe = false;
    static bool fastStop = false;
    static bool slideWalk = false;
    static bool edgeJump = false;
    static bool jumpBug = false;
    
    ImGui::Checkbox("Bunny Hop", &bunnyHop);
    ImGui::Checkbox("Auto Strafe", &autoStrafe);
    ImGui::Checkbox("Fast Stop", &fastStop);
    ImGui::Checkbox("Slide Walk", &slideWalk);
    ImGui::Checkbox("Edge Jump", &edgeJump);
    ImGui::Checkbox("Jump Bug", &jumpBug);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Exploits");
    
    static bool doubleTap = false;
    static bool hideShots = false;
    static bool fakelag = false;
    static bool fakeDuck = false;
    
    ImGui::Checkbox("Double Tap", &doubleTap);
    ImGui::Checkbox("Hide Shots", &hideShots);
    ImGui::Checkbox("Fake Lag", &fakelag);
    ImGui::Checkbox("Fake Duck", &fakeDuck);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Automation");
    
    static bool autoDuck = false;
    static bool autoPeek = false;
    static bool autoAccept = false;
    static bool buyBot = false;
    
    ImGui::Checkbox("Auto Duck", &autoDuck);
    ImGui::Checkbox("Auto Peek", &autoPeek);
    ImGui::Checkbox("Auto Accept", &autoAccept);
    ImGui::Checkbox("Buy Bot", &buyBot);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Information");
    
    static bool radarHack = false;
    static bool spectatorList = false;
    static bool rankRevealer = false;
    static bool velocityIndicator = false;
    
    ImGui::Checkbox("Radar Hack", &radarHack);
    ImGui::Checkbox("Spectator List", &spectatorList);
    ImGui::Checkbox("Rank Revealer", &rankRevealer);
    ImGui::Checkbox("Velocity Indicator", &velocityIndicator);
}

void MenuManager::RenderConfigTab() {
    ImGui::Text("Configuration Management");
    ImGui::Separator();
    
    static char configName[128] = "default";
    
    ImGui::InputText("Config Name", configName, IM_ARRAYSIZE(configName));
    
    ImGui::Spacing();
    
    if (ImGui::Button("Save Config", ImVec2(200, 30))) {
        std::cout << "[*] Saving config: " << configName << std::endl;
        // TODO: Implement config save (Task 19)
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Load Config", ImVec2(200, 30))) {
        std::cout << "[*] Loading config: " << configName << std::endl;
        // TODO: Implement config load (Task 19)
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Available Configs");
    
    // TODO: List available configs (Task 19)
    ImGui::Text("- default.json");
    ImGui::Text("- rage.json");
    ImGui::Text("- legit.json");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("About");
    ImGui::Text("Fardos.cc - Premium CS2 Cheat");
    ImGui::Text("Version 1.0.0");
}


void MenuManager::RenderSkinsTab() {
    ImGui::Text("Skin Changer");
    ImGui::Separator();
    
    static bool skinChangerEnabled = false;
    
    ImGui::Checkbox("Enable Skin Changer", &skinChangerEnabled);
    
    if (skinChangerEnabled) {
        ImGui::Spacing();
        ImGui::Text("Weapon Selection");
        
        const char* weapons[] = {
            "AK-47", "M4A4", "M4A1-S", "AWP", "Desert Eagle",
            "Glock-18", "USP-S", "P250", "Five-SeveN", "Tec-9"
        };
        static int selectedWeapon = 0;
        ImGui::Combo("Weapon", &selectedWeapon, weapons, IM_ARRAYSIZE(weapons));
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Skin Settings");
        
        static int skinID = 0;
        static float skinWear = 0.0f;
        static int skinSeed = 0;
        static bool skinStattrak = false;
        static int skinStattrakValue = 0;
        static char skinNametag[32] = "";
        
        ImGui::InputInt("Skin ID", &skinID);
        ImGui::SliderFloat("Wear", &skinWear, 0.0f, 1.0f);
        ImGui::InputInt("Seed", &skinSeed);
        
        ImGui::Checkbox("StatTrak", &skinStattrak);
        if (skinStattrak) {
            ImGui::InputInt("StatTrak Value", &skinStattrakValue);
        }
        
        ImGui::InputText("Name Tag", skinNametag, IM_ARRAYSIZE(skinNametag));
        
        ImGui::Spacing();
        
        if (ImGui::Button("Apply Skin", ImVec2(200, 30))) {
            std::cout << "[*] Applying skin to weapon" << std::endl;
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Knife Changer");
        
        static bool knifeChangerEnabled = false;
        ImGui::Checkbox("Enable Knife Changer", &knifeChangerEnabled);
        
        if (knifeChangerEnabled) {
            const char* knives[] = {
                "Bayonet", "M9 Bayonet", "Karambit", "Butterfly",
                "Flip Knife", "Gut Knife", "Falchion", "Bowie",
                "Shadow Daggers", "Ursus", "Navaja", "Stiletto", "Talon"
            };
            static int selectedKnife = 0;
            ImGui::Combo("Knife Model", &selectedKnife, knives, IM_ARRAYSIZE(knives));
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Glove Changer");
        
        static bool gloveChangerEnabled = false;
        ImGui::Checkbox("Enable Glove Changer", &gloveChangerEnabled);
        
        if (gloveChangerEnabled) {
            const char* gloves[] = {
                "Sport Gloves", "Driver Gloves", "Hand Wraps",
                "Moto Gloves", "Specialist Gloves", "Hydra Gloves"
            };
            static int selectedGlove = 0;
            ImGui::Combo("Glove Type", &selectedGlove, gloves, IM_ARRAYSIZE(gloves));
        }
    }
}
