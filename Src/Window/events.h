#pragma once

#include <cstdint>
#include "blitcl.h"

#define BLIT_ENGINE_SHUTDOWN_EXPECTED_EVENTS 1
#define BLIT_KEY_PRESSED_EXPECTED_EVENTS 100
#define BLIT_KEY_RELEASED_EXPECTED_EVENTS 100
#define BLIT_MOUSE_BUTTON_PRESSED_EXPECTED_EVENTS 50
#define BLIT_MOUSE_BUTTON_RELEASED_EXPECTED_EVENTS 50
#define BLIT_MOUSE_MOVED_EXPECTED_EVENTS  10
#define BLIT_MOUSE_WHEEL_EXPECTED_EVENTS  10
#define BLIT_WINDOW_RESIZE_EXPECTED_EVENTS  10

namespace Core
{
    // When an event needs to pass some data, this struct will be used to hide the data inside the union. The listener should know how to uncover the data
    struct EventContext 
    {
        // 128 bytes
        union 
        {
            int64_t si64[2];
            uint64_t ui64[2];
            double f2[2];
            int32_t si32[4];
            uint32_t ui32[4];
            float f[4];
            int16_t si16[8];
            uint16_t ui16[8];
            int8_t si8[16];
            int8_t ui8[16];
            char c[16];
        } data;
    };

    enum class BlitEventType : uint16_t
    {
        // Shuts the application down on the next frame.
        EngineShutdown = 0,
        KeyPressed = 1,
        KeyReleased = 2,
        MouseButtonPressed = 3,
        MouseButtonReleased = 4,
        MouseMoved = 5,
        MouseWheel = 6,
        WindowResize = 7,
        MaxTypes = 8
    };

    typedef uint8_t (*pfnOnEvent)(BlitEventType type, void* pSender, void* pListener, EventContext data);

    struct RegisteredEvent 
    {
        void* pListener;
        pfnOnEvent callback;
    };
    
    #define MAX_MESSAGE_CODES 16384

    struct EventSystemContext 
    {
        Blitcl::DynamicArray<RegisteredEvent> eventTypes[MAX_MESSAGE_CODES];

        uint32_t maxExpectedEvents[static_cast<size_t>(BlitEventType::MaxTypes)] = 
        {
            BLIT_ENGINE_SHUTDOWN_EXPECTED_EVENTS, 
            BLIT_KEY_PRESSED_EXPECTED_EVENTS, 
            BLIT_KEY_RELEASED_EXPECTED_EVENTS, 
            BLIT_MOUSE_BUTTON_PRESSED_EXPECTED_EVENTS,
            BLIT_MOUSE_BUTTON_RELEASED_EXPECTED_EVENTS,
            BLIT_MOUSE_MOVED_EXPECTED_EVENTS,
            BLIT_MOUSE_WHEEL_EXPECTED_EVENTS, 
            BLIT_WINDOW_RESIZE_EXPECTED_EVENTS
        };

        EventSystemContext();

        ~EventSystemContext();

        static EventSystemContext* s_pEventSystem;

        inline static EventSystemContext* GetContext() { return s_pEventSystem; }
    };

    // Adds a new RegisteredEvent to the event system context event types array
    uint8_t RegisterEvent(BlitEventType type, void* pListener, pfnOnEvent eventCallback);

    uint8_t UnregisterEvent(BlitEventType type, void* pListener, pfnOnEvent eventCallback);

    uint8_t FireEvent(BlitEventType type, void* pSender, EventContext context);
}