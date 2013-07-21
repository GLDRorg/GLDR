#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <functional>

//#define BASELINE_OGL

#ifndef BASELINE_OGL
#include <glload/wgl_all.hpp>
#include <glload/wgl_load.hpp>
#include <glload/gl_4_3.hpp>
#include <glload/gl_load.hpp>
#endif

namespace oglw {

    class WindowException : public virtual std::exception {
        std::string _what;
    public:
        WindowException(std::string const& what) : _what(what) { }
        const char* what() { return _what.c_str(); }
    };

    class WindowCreateException : public virtual WindowException {
        public: WindowCreateException(std::string const& what) : WindowException(what) { }
    };
    class WindowDestroyException : public virtual WindowException {
        public: WindowDestroyException(std::string const& what) : WindowException(what) { }
    };

    struct KeyInfo {
        unsigned key;
    };

    struct MouseInfo {
        enum class Button {
            None = 0, Left, Right, Middle,
        };

        int x, y;
        Button button;
    };

    class OpenGLWindowBase
    {
    protected:
        bool isActive;

    public:
        std::function<void(void)> displayFunc;

        std::function<void(unsigned, unsigned)> resizeCallback;
        std::function<void(KeyInfo) > keydownCallback;
        std::function<void(KeyInfo) > keyupCallback;
        std::function<void(bool) > activateCallback;
        std::function<void(MouseInfo)> mousemoveCallback;
        std::function<void(MouseInfo)> mouseupCallback;
        std::function<void(MouseInfo)> mousedownCallback;

        bool active() const { return isActive; }
    };

#ifdef _WIN32

    class WinAPIOGLWindow : public OpenGLWindowBase {
    protected:
        HDC m_hDC;            // Private GDI Device Context
        HGLRC m_hRC;            // Permanent Rendering Context
        HWND  m_hWnd;            // Holds Our Window Handle
        HINSTANCE m_hInstance;        // Holds The Instance Of The Application
        bool m_Fullscreen;

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        void kill();

        static MouseInfo mouseInfoFromMsgParam(WPARAM wParam, LPARAM lParam){
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            MouseInfo::Button b;
            switch (wParam) {
                case MK_LBUTTON: b = MouseInfo::Button::Left; break;
                case MK_RBUTTON: b = MouseInfo::Button::Right; break;
                case MK_MBUTTON: b = MouseInfo::Button::Middle; break;
            }
            return MouseInfo { x, y, b };
        }

    public:
        WinAPIOGLWindow(std::string const& title = "OpenGL window",
            unsigned width = 800,
            unsigned height = 600,
            unsigned char bits = 32,
            bool fullscreen = false
            );

        void close() {
            PostQuitMessage(0);
        }

        void display() {
            if (displayFunc)
                displayFunc();
            SwapBuffers(m_hDC);
        }

        bool enableFullScreen(unsigned width, unsigned height, unsigned bits ) {
            DEVMODE dmScreenSettings;                                // Device Mode
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    // Makes Sure Memory's Cleared
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);        // Size Of The Devmode Structure
            dmScreenSettings.dmPelsWidth = width;                // Selected Screen Width
            dmScreenSettings.dmPelsHeight = height;                // Selected Screen Height
            dmScreenSettings.dmBitsPerPel = bits;                    // Selected Bits Per Pixel
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                return false;
            }
            else
                return true;
        }

        bool process() {
            MSG msg;
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))    // Is There A Message Waiting?
            {
                if (msg.message == WM_QUIT) {
                    return false;
                }
                TranslateMessage(&msg);                // Translate The Message
                DispatchMessage(&msg);                // Dispatch The Message
            }
            return true;
        }

        /*void signalErrorMessage(const std::string& message, const std::string& caption = std::string()) {
            MessageBox(m_hWnd, message.c_str(), caption.c_str(), MB_OK | MB_ICONEXCLAMATION);
            throw std::runtime_error(message);
        }*/

        ~WinAPIOGLWindow() {
            try {
                kill();
            }
            catch (WindowException&) {
                // TODO: error reporting here
            }
            catch (...) {
            }
        }
    };

    typedef WinAPIOGLWindow Window;

#else
#error "This OS is not supported."
#endif

    WinAPIOGLWindow::WinAPIOGLWindow(std::string const& title,
        unsigned width,
        unsigned height,
        unsigned char bits,
        bool fullscreen)
        : m_Fullscreen(fullscreen)
    {
        try {
            unsigned        PixelFormat;            // Holds The Results After Searching For A Match
            WNDCLASS    wc;                        // Windows Class Structure
            DWORD        dwExStyle;                // Window Extended Style
            DWORD        dwStyle;                // Window Style
            RECT        WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values
            WindowRect.left = (long) 0;            // Set Left Value To 0
            WindowRect.right = (long) width;        // Set Right Value To Requested Width
            WindowRect.top = (long) 0;                // Set Top Value To 0
            WindowRect.bottom = (long) height;        // Set Bottom Value To Requested Height

            m_hInstance = GetModuleHandle(NULL);                // Grab An Instance For Our Window
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;    // Redraw On Size, And Own DC For Window.
            wc.lpfnWndProc = (WNDPROC) WndProc;                    // WndProc Handles Messages
            wc.cbClsExtra = 0;                                    // No Extra Window Data
            wc.cbWndExtra = 0;                                    // No Extra Window Data
            wc.hInstance = m_hInstance;                            // Set The Instance
            wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);            // Load The Default Icon
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);            // Load The Arrow Pointer
            wc.hbrBackground = NULL;                                    // No Background Required For GL
            wc.lpszMenuName = NULL;                                    // We Don't Want A Menu
            wc.lpszClassName = "OpenGL";                                // Set The Class Name

            if (!RegisterClass(&wc))                                    // Attempt To Register The Window Class
            {
                MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
                throw std::runtime_error("Failed To Register The Window Class.");
            }

            if (m_Fullscreen)                                                // Are We Still In Fullscreen Mode?
            {
                enableFullScreen(width, height, bits);
                dwExStyle = WS_EX_APPWINDOW;                                // Window Extended Style
                dwStyle = WS_POPUP;                                        // Windows Style
                ShowCursor(FALSE);                                        // Hide Mouse Pointer
            }
            else
            {
                dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;            // Window Extended Style
                dwStyle = WS_OVERLAPPEDWINDOW;                            // Windows Style
            }

            AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // Adjust Window To True Requested Size

            // Create The Window
            if (!(m_hWnd = CreateWindowEx(dwExStyle,                            // Extended Style For The Window
                "OpenGL",                            // Class Name
                title.c_str(),                                // Window Title
                dwStyle |                            // Defined Window Style
                WS_CLIPSIBLINGS |                    // Required Window Style
                WS_CLIPCHILDREN,                    // Required Window Style
                0, 0,                                // Window Position
                WindowRect.right - WindowRect.left,    // Calculate Window Width
                WindowRect.bottom - WindowRect.top,    // Calculate Window Height
                NULL,                                // No Parent Window
                NULL,                                // No Menu
                m_hInstance,                            // Instance
                this)))
            {
                throw WindowCreateException("Window Creation Error");
            }

            static PIXELFORMATDESCRIPTOR pfd =                // pfd Tells Windows How We Want Things To Be
            {
                sizeof(PIXELFORMATDESCRIPTOR),                // Size Of This Pixel Format Descriptor
                1,                                            // Version Number
                PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
                PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
                PFD_DOUBLEBUFFER,                            // Must Support Double Buffering
                PFD_TYPE_RGBA,                                // Request An RGBA Format
                bits,                                        // Select Our Color Depth
                0, 0, 0, 0, 0, 0,                            // Color Bits Ignored
                0,                                            // No Alpha Buffer
                0,                                            // Shift Bit Ignored
                0,                                            // No Accumulation Buffer
                0, 0, 0, 0,                                    // Accumulation Bits Ignored
                16,                                            // 16Bit Z-Buffer (Depth Buffer)  
                8,                                            // No Stencil Buffer
                0,                                            // No Auxiliary Buffer
                PFD_MAIN_PLANE,                                // Main Drawing Layer
                0,                                            // Reserved
                0, 0, 0                                        // Layer Masks Ignored
            };
            if (!(m_hDC = GetDC(m_hWnd))) { // Did We Get A Device Context?
                throw WindowCreateException("Can't Create A GL Device Context.");
            }
            if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd))) { // Did Windows Find A Matching Pixel Format?
                throw WindowCreateException("Can't Find A Suitable PixelFormat.");
            }
            if (!SetPixelFormat(m_hDC, PixelFormat, &pfd)) { // Are We Able To Set The Pixel Format?
                throw WindowCreateException("Can't Set The PixelFormat.");
            }

            HGLRC TempContext;
            if (!(TempContext = wglCreateContext(m_hDC))) { // Are We Able To Get A Rendering Context?
                throw WindowCreateException("Can't Create A GL Rendering Context.");
            }
            if (!wglMakeCurrent(m_hDC, TempContext)) { // Try To Activate The Rendering Context{
                throw WindowCreateException("Can't Activate The GL Rendering Context.");
            }

            if (!glload::LoadFunctions()) {
                throw WindowCreateException("Can't load glload functions");
            }

#ifndef BASELINE_OGL
            int attribs[] =
            {
                wgl::CONTEXT_MAJOR_VERSION_ARB, 4,
                wgl::CONTEXT_MINOR_VERSION_ARB, 3,
                wgl::CONTEXT_FLAGS_ARB, wgl::CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | wgl::CONTEXT_DEBUG_BIT_ARB,
                wgl::CONTEXT_PROFILE_MASK_ARB, wgl::CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };
            if (!wgl::exts::var_ARB_create_context) {
                m_hRC = TempContext;
            }
            else {
                m_hRC = wgl::CreateContextAttribsARB(m_hDC,0, attribs);
                wglMakeCurrent(NULL,NULL);
                wglDeleteContext(TempContext);
                wglMakeCurrent(m_hDC, m_hRC);
            }
#else
            //m_hRC = TempContext;
#endif

            ShowWindow(m_hWnd, SW_SHOW);                        // Show The Window
            SetForegroundWindow(m_hWnd);                        // Slightly Higher Priority
            SetFocus(m_hWnd);                                    // Sets Keyboard Focus To The Window
        }
        catch (WindowCreateException const& e) {
            kill();
        }
    }
    

    void WinAPIOGLWindow::kill(void)                    // Properly kill The Window
    {
        try {
            if (m_Fullscreen) {
                // Are We In Fullscreen Mode?
                ChangeDisplaySettings(NULL, 0);                    // If So Switch Back To The Desktop
                ShowCursor(TRUE);                                // Show Mouse Pointer
            }

            if (m_hRC) {
                // Do We Have A Rendering Context?
                if (!wglMakeCurrent(NULL, NULL)) {
                    m_hRC = nullptr;
                    throw WindowDestroyException("Release Of DC And RC Failed.");
                }

                if (!wglDeleteContext(m_hRC)) { // Are We Able To Delete The RC?
                    m_hRC = nullptr;
                    throw WindowDestroyException("Release Rendering Context Failed.");
                }
            }

            if (m_hDC && !ReleaseDC(m_hWnd, m_hDC)) {                   // Are We Able To Release The DC
                m_hDC = nullptr;
                throw WindowDestroyException("Release Device Context Failed.");
            }

            if (m_hWnd && !DestroyWindow(m_hWnd)) {
                m_hWnd = nullptr;
                throw WindowDestroyException("Could Not Release hWnd.");
            }

            if (!UnregisterClass("OpenGL", m_hInstance)) {
                m_hInstance = nullptr;
                throw WindowDestroyException("Could Not Unregister Class.");
            }
        }
        catch (WindowException & w) {
            throw;
        }
    }

    LRESULT CALLBACK WinAPIOGLWindow::WndProc(HWND    hWnd,            // Handle For This Window
        UINT    uMsg,            // Message For This Window
        WPARAM    wParam,            // Additional Message Information
        LPARAM    lParam)            // Additional Message Information
    {
        WinAPIOGLWindow* window = static_cast<decltype(window)>(reinterpret_cast<void*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)));

        switch (uMsg)                                    // Check For Windows Messages
        {
        case WM_CREATE:
            {
                CREATESTRUCT *lpcs = reinterpret_cast<CREATESTRUCT*>(lParam);
                WinAPIOGLWindow* win = reinterpret_cast<WinAPIOGLWindow*>(lpcs->lpCreateParams);

                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
            }
            return 0;

        case WM_ACTIVATE:                            // Watch For Window Activate Message
            {
                if (!HIWORD(wParam))                    // Check Minimization State
                    window->isActive = true;                        // Program Is Active
                else
                    window->isActive = false;                        // Program Is No Longer Active

                if (window->activateCallback)
                    window->activateCallback(window->isActive);

                return 0;                                // Return To The Message Loop
            }

        case WM_SYSCOMMAND:                            // Intercept System Commands
            {
                /*switch (wParam)                            // Check System Calls
                {
                case SC_SCREENSAVE:                    // Screensaver Trying To Start?
                case SC_MONITORPOWER:                // Monitor Trying To Enter Powersave?
                return 0;                            // Prevent From Happening
                }*/
                break;                                    // Exit
            }

        case WM_CLOSE:                                // Did We Receive A Close Message?
            {
                PostQuitMessage(0);                        // Send A Quit Message
                return 0;                                // Jump Back
            }

        case WM_KEYDOWN:                            // Is A Key Being Held Down?
            {
                if (window->keyupCallback)
                    window->keyupCallback(KeyInfo { wParam });
                return 0;
            }
        case WM_KEYUP:                                // Has A Key Been Released?
            {
                if (window->keydownCallback)
                    window->keydownCallback(KeyInfo { wParam });
                return 0;                                // Jump Back
            }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            {
                if (window->mousedownCallback){
                    window->mousedownCallback(mouseInfoFromMsgParam(wParam, lParam));
                }
                return 0;
            }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            {
                if (window->mouseupCallback){
                    window->mouseupCallback(mouseInfoFromMsgParam(wParam, lParam));
                }
                return 0;
            }
        case WM_MOUSEMOVE:
            {
                if (window->mousemoveCallback){
                    window->mousemoveCallback(mouseInfoFromMsgParam(wParam, lParam));
                }
                return 0;
            }

        case WM_SIZE:                                // Resize The OpenGL Window
            {
                unsigned Width = LOWORD(lParam);
                unsigned Height = HIWORD(wParam);
                if (window->resizeCallback)
                    window->resizeCallback(Width, Height);
                return 0;                                // Jump Back
            }
        }

        // Pass All Unhandled Messages To DefWindowProc
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

}
