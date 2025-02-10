#pragma once

#include "Window/events.h"

namespace Conbusit
{
    uint8_t ActivateGameControls();

    uint8_t WindowShutdownEvent(Core::BlitEventType type, void* pSender, void* pListener, Core::EventContext data);
}