#include <IconsMaterialDesign.h>

#include "imgui.h"

#include "Glacier/ZGameLoopManager.h"
#include "Glacier/ZLevelManager.h"
#include "Glacier/UI/ZHUDManager.h"
#include "Glacier/UI/EHUDFadeReason.h"
#include "Glacier/Player/ZHitman5.h"

#include "Hooks.h"
#include "HUDPersist.h"

// Bitmask covering all HUD_ITEM_NUM elements
static constexpr unsigned int ALL_HUD_ELEMENTS = (1u << HUD_ITEM_NUM) - 1;

HUDPersist::HUDPersist() :
    isHUDPersistEnabled(true)
{
}

HUDPersist::~HUDPersist()
{
    const ZMemberDelegate<HUDPersist, void(const SGameUpdateEvent&)> delegate(this, &HUDPersist::OnFrameUpdate);

    GameLoopManager->UnregisterForFrameUpdate(delegate);
}

void HUDPersist::OnEngineInitialized()
{
    const ZMemberDelegate<HUDPersist, void(const SGameUpdateEvent&)> delegate(this, &HUDPersist::OnFrameUpdate);

    GameLoopManager->RegisterForFrameUpdate(delegate, 1);
}

void HUDPersist::OnDrawMenu()
{
    ImGui::Checkbox(ICON_MD_SELF_IMPROVEMENT " HUD Persist", &isHUDPersistEnabled);
}

void HUDPersist::OnFrameUpdate(const SGameUpdateEvent& updateEvent)
{
    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

    if (!isHUDPersistEnabled || !hitman)
    {
        return;
    }

    for (int reason = 0; reason < HUD_FADE_REASON_NUM; ++reason)
    {
        HUDManager->FadeHUDElements(ALL_HUD_ELEMENTS, static_cast<EHUDFadeReason>(reason), true, 0.0f);
    }
}

DEFINE_MOD(HUDPersist);
