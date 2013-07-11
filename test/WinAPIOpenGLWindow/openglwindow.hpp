#pragma once

#include <windows.h>
#include <functional>

#define BASELINE_OGL

#ifndef BASELINE_OGL
#include <glload/wgl_all.hpp>
#include <glload/wgl_load.hpp>
#endif

class OpenGLWindowBase
{
protected:
    std::function<void(unsigned, unsigned)> resizeCallback;
    std::function<void(int)> keydownCallback;
    std::function<void(int) > keyupCallback;
    std::function<void(bool)> activateCallback;

    bool isActive;

public:
    bool active() const { return isActive; }

    void setResizeCb(decltype(resizeCallback) cb) {
        resizeCallback = std::move(cb);
    }
    void setKeydownCb(decltype(keydownCallback) cb) {
        keydownCallback = std::move(cb);
    }
    void setKeyupCb(decltype(keyupCallback) cb) {
        keyupCallback = std::move(cb);
    }
    void setActivateCb(decltype(keyupCallback) cb) {
        keyupCallback = std::move(cb);
    }
};

#ifdef WIN32

class WinAPIOGLWindow : public OpenGLWindowBase {
protected:
    HDC            m_hDC;            // Private GDI Device Context
    HGLRC        m_hRC;            // Permanent Rendering Context
    HWND        m_hWnd;            // Holds Our Window Handle
    HINSTANCE    m_hInstance;        // Holds The Instance Of The Application
    bool        m_Fullscreen;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void kill();

public:
    WinAPIOGLWindow(std::string const& title = "OpenGL window",
        unsigned width = 800,
        unsigned height = 600,
        unsigned bits = 32,
        bool fullscreen = false
        );

    void close() {
        PostQuitMessage(0);
    }

    void display() {
        SwapBuffers(m_hDC);
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

    ~WinAPIOGLWindow() {
        kill();
    }
};

typedef WinAPIOGLWindow OpenGLWindow;

#else
#error "This OS is not supported."
#endif

WinAPIOGLWindow::WinAPIOGLWindow(std::string const& title,
                                 unsigned width,
                                 unsigned height,
                                 unsigned bits,
                                 bool fullscreen)
{
    GLuint        PixelFormat;            // Holds The Results After Searching For A Match
    WNDCLASS    wc;                        // Windows Class Structure
    DWORD        dwExStyle;                // Window Extended Style
    DWORD        dwStyle;                // Window Style
    RECT        WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left=(long)0;            // Set Left Value To 0
    WindowRect.right=(long)width;        // Set Right Value To Requested Width
    WindowRect.top=(long)0;                // Set Top Value To 0
    WindowRect.bottom=(long)height;        // Set Bottom Value To Requested Height

    m_Fullscreen = fullscreen;            // Set The Global Fullscreen Flag

    m_hInstance         = GetModuleHandle(NULL);                // Grab An Instance For Our Window
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;    // Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc      = (WNDPROC) WndProc;                    // WndProc Handles Messages
    wc.cbClsExtra       = 0;                                    // No Extra Window Data
    wc.cbWndExtra       = 0;                                    // No Extra Window Data
    wc.hInstance        = m_hInstance;                            // Set The Instance
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);            // Load The Default Icon
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);            // Load The Arrow Pointer
    wc.hbrBackground    = NULL;                                    // No Background Required For GL
    wc.lpszMenuName     = NULL;                                    // We Don't Want A Menu
    wc.lpszClassName    = "OpenGL";                                // Set The Class Name

    if (!RegisterClass(&wc))                                    // Attempt To Register The Window Class
    {
        MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Failed To Register The Window Class.");
    }
    
    if (m_Fullscreen)                                                // Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;                                // Device Mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));    // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);        // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth    = width;                // Selected Screen Width
        dmScreenSettings.dmPelsHeight    = height;                // Selected Screen Height
        dmScreenSettings.dmBitsPerPel    = bits;                    // Selected Bits Per Pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
            if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
            {
                m_Fullscreen=FALSE;        // Windowed Mode Selected.  Fullscreen = FALSE
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
                throw std::runtime_error("Window not instantiated because of user choice");
            }
        }
    }

    if (m_Fullscreen)                                                // Are We Still In Fullscreen Mode?
    {
        dwExStyle=WS_EX_APPWINDOW;                                // Window Extended Style
        dwStyle=WS_POPUP;                                        // Windows Style
        ShowCursor(FALSE);                                        // Hide Mouse Pointer
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;            // Window Extended Style
        dwStyle=WS_OVERLAPPEDWINDOW;                            // Windows Style
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // Adjust Window To True Requested Size

    // Create The Window
    if (!(m_hWnd=CreateWindowEx(    dwExStyle,                            // Extended Style For The Window
                                "OpenGL",                            // Class Name
                                title.c_str(),                                // Window Title
                                dwStyle |                            // Defined Window Style
                                WS_CLIPSIBLINGS |                    // Required Window Style
                                WS_CLIPCHILDREN,                    // Required Window Style
                                0, 0,                                // Window Position
                                WindowRect.right-WindowRect.left,    // Calculate Window Width
                                WindowRect.bottom-WindowRect.top,    // Calculate Window Height
                                NULL,                                // No Parent Window
                                NULL,                                // No Menu
                                m_hInstance,                            // Instance
                                this)))                                
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Window Creation Error.");
    }

    static PIXELFORMATDESCRIPTOR pfd=                // pfd Tells Windows How We Want Things To Be
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
    
    if (!(m_hDC=GetDC(m_hWnd)))                            // Did We Get A Device Context?
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Can't Create A GL Device Context.");
    }

    if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))    // Did Windows Find A Matching Pixel Format?
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Can't Find A Suitable PixelFormat.");
    }

    if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))        // Are We Able To Set The Pixel Format?
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Can't Set The PixelFormat.");
    }

    HGLRC TempContext;
    if (!(TempContext = wglCreateContext(m_hDC)))                // Are We Able To Get A Rendering Context?
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Can't Create A GL Rendering Context.");
    }

    if(!wglMakeCurrent(m_hDC,TempContext))                    // Try To Activate The Rendering Context
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        throw std::runtime_error("Can't Activate The GL Rendering Context.");
    }

    /*GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        MessageBox(NULL, "GLEW is not initialized!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
    }*/
    glload::LoadFunctions();

#ifndef BASELINE_OGL
    int attribs[] =
    {
        wgl::CONTEXT_MAJOR_VERSION_ARB, 3,
        wgl::CONTEXT_MINOR_VERSION_ARB, 3,
        wgl::CONTEXT_FLAGS_ARB, wgl::CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        0
    };
    if(wgl::upported("WGL_ARB_create_context") == 1)
    {
        m_hRC = wglCreateContextAttribsARB(m_hDC,0, attribs);
        wglMakeCurrent(NULL,NULL);
        wglDeleteContext(TempContext);
        wglMakeCurrent(m_hDC, m_hRC);
    }
    else
    {    //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        m_hRC = TempContext;
    }
#endif

    ShowWindow(m_hWnd,SW_SHOW);                        // Show The Window
    SetForegroundWindow(m_hWnd);                        // Slightly Higher Priority
    SetFocus(m_hWnd);                                    // Sets Keyboard Focus To The Window
    //ReSizeGLScene(width, height);                    // Set Up Our Perspective GL Screen

    /*if (!InitGL())                                    // Initialize Our Newly Created GL Window
    {
        kill();                                // Reset The Display
        MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }*/
}

void OpenGLWindow::kill(void)                    // Properly kill The Window
{
    if (m_Fullscreen)                                        // Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL,0);                    // If So Switch Back To The Desktop
        ShowCursor(TRUE);                                // Show Mouse Pointer
    }

    if (m_hRC)                                            // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL))                    // Are We Able To Release The DC And RC Contexts?
        {
            MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(m_hRC))                        // Are We Able To Delete The RC?
        {
            MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        m_hRC=NULL;                                        // Set RC To NULL
    }

    if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))                    // Are We Able To Release The DC
    {
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        m_hDC=NULL;                                        // Set DC To NULL
    }

    if (m_hWnd && !DestroyWindow(m_hWnd))                    // Are We Able To Destroy The Window?
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        m_hWnd=NULL;                                        // Set hWnd To NULL
    }

    if (!UnregisterClass("OpenGL", m_hInstance))            // Are We Able To Unregister Class
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        m_hInstance=NULL;                                    // Set hInstance To NULL
    }
}

/*    This Code Creates Our OpenGL Window.  Parameters Are:                    *
 *    title            - Title To Appear At The Top Of The Window                *
 *    width            - Width Of The GL Window Or Fullscreen Mode                *
 *    height            - Height Of The GL Window Or Fullscreen Mode            *
 *    bits            - Number Of Bits To Use For Color (8/16/24/32)            *
 *    fullscreenflag    - Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)    */
 

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
                window->keyupCallback(wParam);
            return 0;
        }

    case WM_KEYUP:                                // Has A Key Been Released?
        {
            if (window->keydownCallback)
                window->keydownCallback(wParam);
            return 0;                                // Jump Back
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


