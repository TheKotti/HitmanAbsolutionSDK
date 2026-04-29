#pragma once

#include <Glacier/SGameUpdateEvent.h>
#include <Glacier/Scene/ZEntitySceneContext.h>
#include <ModInterface.h>

void __fastcall ZEntitySceneContext_CreateSceneHook(ZEntitySceneContext* pThis, int edx, const ZString& sStreamingState);
void __fastcall ZEntitySceneContext_ClearSceneHook(ZEntitySceneContext* pThis, int edx, bool bFullyUnloadScene);

class LevelTimer : public ModInterface
{
public:
    LevelTimer();
    ~LevelTimer();

    void Initialize() override;
    void OnEngineInitialized() override;
    void OnDrawMenu() override;
    void OnDrawUI(bool hasFocus) override;

    void OnCreateScene(ZEntitySceneContext* entitySceneContext, const ZString& streamingState);
    void OnClearScene(ZEntitySceneContext* entitySceneContext, bool fullyUnloadScene);

private:
    void OnFrameUpdate(const SGameUpdateEvent& updateEvent);

    float m_elapsedSeconds;
    bool isLevelTimerEnabled;
};

DECLARE_MOD(LevelTimer)