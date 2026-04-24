#pragma once

#include <Glacier/SGameUpdateEvent.h>

#include <ModInterface.h>

class HUDPersist : public ModInterface
{
public:
    HUDPersist();
    ~HUDPersist();

    void OnEngineInitialized() override;
    void OnDrawMenu() override;

private:
    void OnFrameUpdate(const SGameUpdateEvent& updateEvent);

    bool isHUDPersistEnabled;
};

DECLARE_MOD(HUDPersist)
