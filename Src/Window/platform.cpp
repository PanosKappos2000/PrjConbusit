#include "platform.h"
#include "events.h"


namespace Platform
{
    #ifdef _MSC_VER

        #include <windows.h>
        #include <Windows.h>
        #include <windowsx.h>
        #include <WinUser.h>
        #include <windowsx.h>

        struct PlatformContext
        {
            HINSTANCE winInstance;
            HWND winWindow;
        };

        inline PlatformContext inl_context;

        inline double clockFrequency;
        inline LARGE_INTEGER startTime;

        LRESULT CALLBACK Win32ProcessMessage(HWND winWindow, uint32_t msg, WPARAM w_param, LPARAM l_param);

        uint8_t WindowSystemInit(const char* appName, int32_t x, int32_t y, uint32_t width, uint32_t height)
        {
            inl_context.winInstance = GetModuleHandleA(0);

            HICON icon = LoadIcon(inl_context.winInstance, IDI_APPLICATION);
            tagWNDCLASSA wc;
            memset(&wc, 0, sizeof(wc));
            wc.style = CS_DBLCLKS;

            // Pass the function that will get called when events get triggered
            wc.lpfnWndProc = Win32ProcessMessage;

            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = inl_context.winInstance;
            wc.hIcon = icon;
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.hbrBackground = nullptr;

            wc.lpszClassName = "BlitzenWindowClass";

            if(!RegisterClassA(&wc))
            {
                MessageBoxA(inl_context.winWindow, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
                return 0;
            }

            // Set the window's client size
            uint32_t clientX = x;
            uint32_t clientY = y;
            uint32_t clientWidth = width;
            uint32_t clientHeight = height;

            // Set the size of the window to be the same as the client momentarily
            uint32_t windowX = clientX;
            uint32_t windowY = clientY;
            uint32_t windowWidth = clientWidth;
            uint32_t windowHeight = clientHeight;

            uint32_t windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
            uint32_t windowExStyle = WS_EX_APPWINDOW;

            windowStyle |= WS_MAXIMIZEBOX;
            windowStyle |= WS_MINIMIZEBOX;
            windowStyle |= WS_THICKFRAME;

            RECT borderRect = {0, 0, 0, 0};
            AdjustWindowRectEx(&borderRect, windowStyle, 0, windowExStyle);

            // Set the true size of the window
            windowX += borderRect.left;
            windowY += borderRect.top;
            windowWidth += borderRect.right - borderRect.left;
            windowHeight -= borderRect.top - borderRect.bottom;

            //Create the window
            HWND handle = CreateWindowExA(windowExStyle, "BlitzenWindowClass", appName, windowStyle, windowX, windowY, windowWidth, windowHeight, 0, 0,
            inl_context.winInstance, 0);

            // Only assign the handle if it was actually created, otherwise the application should fail
            if(!handle)
            {
                MessageBoxA(nullptr, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
                return 0;
            }
            else
            {
                inl_context.winWindow = handle;
            }

            //Tell the window to show
            uint8_t shouldActivate = 1;
            int32_t show = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
            ShowWindow(inl_context.winWindow, show);

            // Clock setup, similar thing to glfwGetTime
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            clockFrequency = 1.0 / static_cast<double>(frequency.QuadPart);// The quad part is just a 64 bit integer
            QueryPerformanceCounter(&startTime);

            return 1;
        }

        void PlatformShutdown()
        {
            if(inl_context.winWindow)
            {
                DestroyWindow(inl_context.winWindow);
            }
        }

        uint8_t PumpMessages()
        {
            MSG message;
            while(PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            return 1;
        }

        double PlatformGetAbsoluteTime()
        {
            LARGE_INTEGER nowTime;
            QueryPerformanceCounter(&nowTime);
            return static_cast<double>(nowTime.QuadPart) * clockFrequency;
        }

        LRESULT CALLBACK Win32ProcessMessage(HWND winWindow, uint32_t msg, WPARAM w_param, LPARAM l_param)
        {
            switch(msg)
            {
                case WM_ERASEBKGND:
                {
                    // Notify the OS that erasing will be handled by the application to prevent flicker
                    return 1;
                }

                case WM_CLOSE:
                {
                    Core::EventContext context{};
                    Core::FireEvent(Core::BlitEventType::EngineShutdown, nullptr, context);
                    return 1;
                }

                case WM_DESTROY:
                {
                    PostQuitMessage(0);
                    return 0;
                }
                case WM_SIZE:
                {
                    // Get the updated size.
                    RECT rect;
                    GetClientRect(winWindow, &rect);
                    uint32_t width = rect.right - rect.left;
                    uint32_t height = rect.bottom - rect.top;
                    /*BlitzenCore::EventContext context;
                    context.data.ui32[0] = width;
                    context.data.ui32[1] = height;
                    BlitzenCore::FireEvent(BlitzenCore::BlitEventType::WindowResize, nullptr, context);*/
                    break;
                }
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYUP: 
                {
                    // Key pressed/released
                    uint8_t pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                    /*BlitzenCore::BlitKey key = static_cast<BlitzenCore::BlitKey>(w_param);
                    BlitzenCore::InputProcessKey(key, pressed);*/
                    break;
                } 
                case WM_MOUSEMOVE: 
                {
                    // Mouse move
                    int32_t mouseX = GET_X_LPARAM(l_param);
                    int32_t mouseY = GET_Y_LPARAM(l_param);
                    //BlitzenCore::InputProcessMouseMove(mouseX, mouseY);
                    break;
                } 
                case WM_MOUSEWHEEL: 
                {
                    int32_t zDelta = GET_WHEEL_DELTA_WPARAM(w_param);
                    if (zDelta != 0) 
                    {
                        // Flatten the input to an OS-independent (-1, 1)
                        zDelta = (zDelta < 0) ? -1 : 1;
                        //BlitzenCore::InputProcessMouseWheel(zDelta);
                        break;
                    }
                }
                case WM_LBUTTONDOWN:
                case WM_MBUTTONDOWN:
                case WM_RBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_MBUTTONUP:
                case WM_RBUTTONUP: 
                {
                    uint8_t bPressed = (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN);
                    /*BlitzenCore::MouseButton button = BlitzenCore::MouseButton::MaxButtons;
                    switch(msg)
                    {
                        case WM_LBUTTONDOWN:
                        case WM_LBUTTONUP:
                        {
                            button = BlitzenCore::MouseButton::Left;
                            break;
                        }
                        case WM_RBUTTONDOWN:
                        case WM_RBUTTONUP:
                        {
                            button = BlitzenCore::MouseButton::Right;
                            break;
                        }
                        case WM_MBUTTONDOWN:
                        case WM_MBUTTONUP:
                        {
                            button = BlitzenCore::MouseButton::Middle;
                            break;
                        }
                    }
                    if(button != BlitzenCore::MouseButton::MaxButtons)
                        BlitzenCore::InputProcessButton(button, bPressed);*/
                    break;
                } 
            }
            // For any events that were not included above, windows can go ahead and handle them like it normally would
            return DefWindowProcA(winWindow, msg, w_param, l_param); 
        }
    #endif
}