#pragma once
#include "EventData.h"
#include "GameEvent.h"

class EventManager
{
public:
    static GameEvent<const SDLEventData&> onSDLEvent;
};