#include <IconsMaterialDesign.h>

#include "imgui.h"
#include "Glacier/ZGameLoopManager.h"
#include "Glacier/ZLevelManager.h"
#include "Glacier/UI/ZHUDManager.h"

#include "Hooks.h"
#include "LevelTimer.h"

LevelTimer::LevelTimer() : m_elapsedSeconds(0.f), isLevelTimerEnabled(true) {}

LevelTimer::~LevelTimer()
{
    const ZMemberDelegate<LevelTimer, void(const SGameUpdateEvent&)> delegate(this, &LevelTimer::OnFrameUpdate);
    GameLoopManager->UnregisterForFrameUpdate(delegate);

    Hooks::ZEntitySceneContext_CreateScene.RemoveHook();
    Hooks::ZEntitySceneContext_ClearScene.RemoveHook();
}

void LevelTimer::Initialize()
{
    ModInterface::Initialize();

    Hooks::ZEntitySceneContext_CreateScene.CreateHook("ZEntitySceneContext::CreateScene", 0x4479E0, ZEntitySceneContext_CreateSceneHook);
    Hooks::ZEntitySceneContext_ClearScene.CreateHook("ZEntitySceneContext::ClearScene", 0x265A80, ZEntitySceneContext_ClearSceneHook);

    Hooks::ZEntitySceneContext_CreateScene.EnableHook();
    Hooks::ZEntitySceneContext_ClearScene.EnableHook();
}

void LevelTimer::OnEngineInitialized()
{
    const ZMemberDelegate<LevelTimer, void(const SGameUpdateEvent&)> delegate(this, &LevelTimer::OnFrameUpdate);
    GameLoopManager->RegisterForFrameUpdate(delegate, 1);
}

void LevelTimer::OnFrameUpdate(const SGameUpdateEvent& updateEvent)
{
    m_elapsedSeconds += static_cast<float>(updateEvent.m_GameTimeDelta.ToSeconds());
}

void LevelTimer::OnCreateScene(ZEntitySceneContext* entitySceneContext, const ZString& streamingState)
{
    m_elapsedSeconds = 0.f;
}

void LevelTimer::OnClearScene(ZEntitySceneContext* entitySceneContext, bool fullyUnloadScene)
{
}

void LevelTimer::OnDrawMenu()
{
    ImGui::Checkbox(ICON_MD_SELF_IMPROVEMENT " Level timer", &isLevelTimerEnabled);
}

void LevelTimer::OnDrawUI(bool hasFocus)
{
    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();
    bool isPaused = HUDManager->IsPauseMenuActive();

    if (!isLevelTimerEnabled || !hitman || isPaused)
    {
        return;
    }

    int minutes = static_cast<int>(m_elapsedSeconds) / 60;
    int seconds = static_cast<int>(m_elapsedSeconds) % 60;

    auto& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.25f, io.DisplaySize.y * 0.012f), ImGuiCond_Always);
    ImGui::Begin("Timer", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowFontScale(3.5f);
    ImGui::Text("%02d:%02d", minutes, seconds);
    ImGui::End();
}

DEFINE_MOD(LevelTimer)

void __fastcall ZEntitySceneContext_CreateSceneHook(ZEntitySceneContext* pThis, int edx, const ZString& sStreamingState)
{
    GetModInstance()->OnCreateScene(pThis, sStreamingState);
    Hooks::ZEntitySceneContext_CreateScene.CallOriginalFunction(pThis, sStreamingState);
}

void __fastcall ZEntitySceneContext_ClearSceneHook(ZEntitySceneContext* pThis, int edx, bool bFullyUnloadScene)
{
    GetModInstance()->OnClearScene(pThis, bFullyUnloadScene);
    Hooks::ZEntitySceneContext_ClearScene.CallOriginalFunction(pThis, bFullyUnloadScene);
}