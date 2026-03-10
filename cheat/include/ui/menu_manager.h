#pragma once

// Menu Manager - Task 17
// TODO: Implement in Task 17

enum class MenuTab {
    LEGIT,
    RAGE,
    ANTIAIM,
    VISUALS,
    MISC,
    SKINS,
    CONFIG
};

class MenuManager {
public:
    MenuManager() = default;
    ~MenuManager() = default;

    // TODO: Implement these methods in Task 17
    void Render();
    void SetVisible(bool visible);
    bool IsVisible();
    void ApplyNeverloseStyle();

private:
    bool visible = false;
    MenuTab currentTab = MenuTab::RAGE;

    void RenderLegitTab();
    void RenderRageTab();
    void RenderAntiAimTab();
    void RenderVisualsTab();
    void RenderMiscTab();
    void RenderSkinsTab();
    void RenderConfigTab();

    void RenderTabBar();
};
