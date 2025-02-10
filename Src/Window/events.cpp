#include "events.h"

namespace Core
{
    EventSystemContext* EventSystemContext::s_pEventSystem = nullptr;

    EventSystemContext::EventSystemContext()
    {
        s_pEventSystem = this;
    }

    EventSystemContext::~EventSystemContext()
    {
        s_pEventSystem = nullptr;
    }

    uint8_t RegisterEvent(BlitEventType type, void* pListener, pfnOnEvent eventCallback) 
    {
        Blitcl::DynamicArray<RegisteredEvent>& events = EventSystemContext::GetContext()->eventTypes[static_cast<size_t>(type)];

        // If no event of this type has been created before, reserve the maximum expected space for events of this type
        if(events.GetSize() == 0) 
        {
            events.Reserve(EventSystemContext::GetContext()->maxExpectedEvents[static_cast<size_t>(type)]);
        }

        for(size_t i = 0; i < events.GetSize(); ++i) 
        {
            // A listener should not have multiple event callbacks for the same type of event
            if(events[i].pListener == pListener) 
            {
                return 0;
            }
        }

        // If at this point, no duplicate was found. Proceed with registration.
        RegisteredEvent event;
        event.pListener = pListener;
        event.callback = eventCallback;
        events.PushBack(event);
        return 1;
    }

    uint8_t UnregisterEvent(BlitEventType type, void* pListener, pfnOnEvent eventCallback) 
    {
        Blitcl::DynamicArray<RegisteredEvent>& events = EventSystemContext::GetContext()->eventTypes[static_cast<size_t>(type)];
        
        if(events.GetSize() == 0) 
        {
            return 0;
        }

        for(size_t i = 0; i < events.GetSize(); ++i) 
        {
            // Find the listener and remove the function pointer
            if(events[i].pListener == pListener && events[i].callback == eventCallback) 
            {
                events.RemoveAtIndex(i);
                return 1;
            }
        }

        // Not found
        return 0;
    }

    uint8_t FireEvent(BlitEventType type, void* pSender, EventContext context)
    {
        Blitcl::DynamicArray<RegisteredEvent>& events = EventSystemContext::GetContext()->eventTypes[static_cast<size_t>(type)];
        
        if(events.GetSize() == 0) 
        {
            return 0;
        }
        
        for(size_t i = 0; i < events.GetSize(); ++i) 
        {
            RegisteredEvent& event = events[i];
            if(event.callback && event.callback(type, pSender, event.pListener, context)) 
            {
                return 1;
            }
        }

        return 0;
    }
}