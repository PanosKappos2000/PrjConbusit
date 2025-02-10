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

    InputSystemContext* InputSystemContext::s_pState = nullptr;

    InputSystemContext::InputSystemContext() 
    {
        s_pState = this;
    }

    InputSystemContext::~InputSystemContext() 
    {
        s_pState = nullptr;
    }

    void UpdateInput(double deltaTime) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        // Copy current states to previous states
        memcpy(&(pState->previousKeyboard), &pState->currentKeyboard, sizeof(KeyboardState));
        memcpy(&pState->previousMouse, &pState->currentMouse, sizeof(MouseState));
    }

    void InputProcessKey(BlitKey key, uint8_t bPressed) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        // Check If the key has not already been flagged as the value of bPressed
        if (pState->currentKeyboard.keys[static_cast<size_t>(key)] != bPressed) 
        {
            // Change the state to bPressed
            pState->currentKeyboard.keys[static_cast<size_t>(key)] = bPressed;

            // Fire off an event for immediate processing after saving the data of the input to the event context
            EventContext context;
            context.data.ui16[0] = static_cast<uint16_t>(key);
            FireEvent(bPressed ? BlitEventType::KeyPressed : BlitEventType::KeyReleased, nullptr, context);
        }
    }

    void InputProcessButton(MouseButton button, uint8_t bPressed) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        // If the state changed, fire an event.
        if (pState->currentMouse.buttons[static_cast<size_t>(button)] != bPressed) 
        {
            pState->currentMouse.buttons[static_cast<size_t>(button)] = bPressed;
            // Fire the event.
            EventContext context;
            context.data.ui16[0] = static_cast<uint16_t>(button);
            FireEvent(bPressed ? BlitEventType::MouseButtonPressed : BlitEventType::MouseButtonPressed, nullptr, context);
        }
    }

    void InputProcessMouseMove(int16_t x, int16_t y) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        // Only process if actually different
        if (pState->currentMouse.x != x || pState->currentMouse.y != y) 
        {
            // The context holds the difference between the new and old mouse position, since that is more useful now
            EventContext context;
            context.data.si16[0] = x - pState->currentMouse.x;
            context.data.si16[1] = y - pState->currentMouse.y;
            
            pState->currentMouse.x = x;
            pState->currentMouse.y = y;

            FireEvent(BlitEventType::MouseMoved, nullptr, context);
        }
    }
    
    void InputProcessMouseWheel(int8_t zDelta) 
    {
        //No internal state to update, simply fires an event
        EventContext context;
        context.data.ui8[0] = zDelta;
        FireEvent(BlitEventType::MouseWheel, nullptr, context);
    }

    uint8_t GetCurrentKeyState(BlitKey key) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        return pState->currentKeyboard.keys[static_cast<size_t>(key)];
    }

    uint8_t GetPreviousKeyState(BlitKey key) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        return pState->currentKeyboard.keys[static_cast<size_t>(key)];
    }

    
    uint8_t GetCurrentMouseButtonState(MouseButton button) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        return pState->currentMouse.buttons[static_cast<size_t>(button)];
    }

    uint8_t GetPreviousMouseButtonState(MouseButton button)
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        return pState->previousMouse.buttons[static_cast<size_t>(button)];
    }

    void GetMousePosition(int32_t* x, int32_t* y) 
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        *x = pState->previousMouse.x;
        *y = pState->currentMouse.y;
    }
    void GetPreviousMousePosition(int32_t* x, int32_t* y)
    {
        InputSystemContext* pState = InputSystemContext::GetState();
        *x = pState->previousMouse.x;
        *y = pState->previousMouse.y;
    }
}