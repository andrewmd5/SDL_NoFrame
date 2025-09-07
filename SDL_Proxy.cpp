#include <windows.h>

#pragma comment(lib, "user32.lib")

#define SDL_NOFRAME 0x00000020

struct SDL_Surface {
    unsigned int flags;
    void* format;
    int w, h;
    unsigned short pitch;
    void* pixels;
};


typedef SDL_Surface* (__cdecl *SDL_SetVideoMode_t)(int width, int height, int bpp, unsigned int flags);
typedef int (__cdecl *SDL_WM_ToggleFullScreen_t)(SDL_Surface* surface);
typedef void (__cdecl *SDL_WM_SetCaption_t)(const char* title, const char* icon);


SDL_SetVideoMode_t Real_SDL_SetVideoMode = NULL;
SDL_WM_ToggleFullScreen_t Real_SDL_WM_ToggleFullScreen = NULL;
SDL_WM_SetCaption_t Real_SDL_WM_SetCaption = NULL;

HMODULE hOriginalSDL = NULL;

HWND g_hGameWindow = NULL;
BOOL g_forceFrameless = TRUE;

// Export all other SDL functions directly to the original DLL
#pragma comment(linker, "/export:SDL_AddTimer=SDL_original.SDL_AddTimer")
#pragma comment(linker, "/export:SDL_AllocRW=SDL_original.SDL_AllocRW")
#pragma comment(linker, "/export:SDL_AudioDriverName=SDL_original.SDL_AudioDriverName")
#pragma comment(linker, "/export:SDL_AudioInit=SDL_original.SDL_AudioInit")
#pragma comment(linker, "/export:SDL_AudioQuit=SDL_original.SDL_AudioQuit")
#pragma comment(linker, "/export:SDL_BuildAudioCVT=SDL_original.SDL_BuildAudioCVT")
#pragma comment(linker, "/export:SDL_CDClose=SDL_original.SDL_CDClose")
#pragma comment(linker, "/export:SDL_CDEject=SDL_original.SDL_CDEject")
#pragma comment(linker, "/export:SDL_CDName=SDL_original.SDL_CDName")
#pragma comment(linker, "/export:SDL_CDNumDrives=SDL_original.SDL_CDNumDrives")
#pragma comment(linker, "/export:SDL_CDOpen=SDL_original.SDL_CDOpen")
#pragma comment(linker, "/export:SDL_CDPause=SDL_original.SDL_CDPause")
#pragma comment(linker, "/export:SDL_CDPlay=SDL_original.SDL_CDPlay")
#pragma comment(linker, "/export:SDL_CDPlayTracks=SDL_original.SDL_CDPlayTracks")
#pragma comment(linker, "/export:SDL_CDResume=SDL_original.SDL_CDResume")
#pragma comment(linker, "/export:SDL_CDStatus=SDL_original.SDL_CDStatus")
#pragma comment(linker, "/export:SDL_CDStop=SDL_original.SDL_CDStop")
#pragma comment(linker, "/export:SDL_ClearError=SDL_original.SDL_ClearError")
#pragma comment(linker, "/export:SDL_CloseAudio=SDL_original.SDL_CloseAudio")
#pragma comment(linker, "/export:SDL_CondBroadcast=SDL_original.SDL_CondBroadcast")
#pragma comment(linker, "/export:SDL_CondSignal=SDL_original.SDL_CondSignal")
#pragma comment(linker, "/export:SDL_CondWait=SDL_original.SDL_CondWait")
#pragma comment(linker, "/export:SDL_CondWaitTimeout=SDL_original.SDL_CondWaitTimeout")
#pragma comment(linker, "/export:SDL_ConvertAudio=SDL_original.SDL_ConvertAudio")
#pragma comment(linker, "/export:SDL_ConvertSurface=SDL_original.SDL_ConvertSurface")
#pragma comment(linker, "/export:SDL_CreateCond=SDL_original.SDL_CreateCond")
#pragma comment(linker, "/export:SDL_CreateCursor=SDL_original.SDL_CreateCursor")
#pragma comment(linker, "/export:SDL_CreateMutex=SDL_original.SDL_CreateMutex")
#pragma comment(linker, "/export:SDL_CreateRGBSurface=SDL_original.SDL_CreateRGBSurface")
#pragma comment(linker, "/export:SDL_CreateRGBSurfaceFrom=SDL_original.SDL_CreateRGBSurfaceFrom")
#pragma comment(linker, "/export:SDL_CreateSemaphore=SDL_original.SDL_CreateSemaphore")
#pragma comment(linker, "/export:SDL_CreateThread=SDL_original.SDL_CreateThread")
#pragma comment(linker, "/export:SDL_CreateYUVOverlay=SDL_original.SDL_CreateYUVOverlay")
#pragma comment(linker, "/export:SDL_Delay=SDL_original.SDL_Delay")
#pragma comment(linker, "/export:SDL_DestroyCond=SDL_original.SDL_DestroyCond")
#pragma comment(linker, "/export:SDL_DestroyMutex=SDL_original.SDL_DestroyMutex")
#pragma comment(linker, "/export:SDL_DestroySemaphore=SDL_original.SDL_DestroySemaphore")
#pragma comment(linker, "/export:SDL_DisplayFormat=SDL_original.SDL_DisplayFormat")
#pragma comment(linker, "/export:SDL_DisplayFormatAlpha=SDL_original.SDL_DisplayFormatAlpha")
#pragma comment(linker, "/export:SDL_DisplayYUVOverlay=SDL_original.SDL_DisplayYUVOverlay")
#pragma comment(linker, "/export:SDL_EnableKeyRepeat=SDL_original.SDL_EnableKeyRepeat")
#pragma comment(linker, "/export:SDL_EnableUNICODE=SDL_original.SDL_EnableUNICODE")
#pragma comment(linker, "/export:SDL_Error=SDL_original.SDL_Error")
#pragma comment(linker, "/export:SDL_EventState=SDL_original.SDL_EventState")
#pragma comment(linker, "/export:SDL_FillRect=SDL_original.SDL_FillRect")
#pragma comment(linker, "/export:SDL_Flip=SDL_original.SDL_Flip")
#pragma comment(linker, "/export:SDL_FreeCursor=SDL_original.SDL_FreeCursor")
#pragma comment(linker, "/export:SDL_FreeRW=SDL_original.SDL_FreeRW")
#pragma comment(linker, "/export:SDL_FreeSurface=SDL_original.SDL_FreeSurface")
#pragma comment(linker, "/export:SDL_FreeWAV=SDL_original.SDL_FreeWAV")
#pragma comment(linker, "/export:SDL_FreeYUVOverlay=SDL_original.SDL_FreeYUVOverlay")
#pragma comment(linker, "/export:SDL_GL_GetAttribute=SDL_original.SDL_GL_GetAttribute")
#pragma comment(linker, "/export:SDL_GL_GetProcAddress=SDL_original.SDL_GL_GetProcAddress")
#pragma comment(linker, "/export:SDL_GL_LoadLibrary=SDL_original.SDL_GL_LoadLibrary")
#pragma comment(linker, "/export:SDL_GL_Lock=SDL_original.SDL_GL_Lock")
#pragma comment(linker, "/export:SDL_GL_SetAttribute=SDL_original.SDL_GL_SetAttribute")
#pragma comment(linker, "/export:SDL_GL_SwapBuffers=SDL_original.SDL_GL_SwapBuffers")
#pragma comment(linker, "/export:SDL_GL_Unlock=SDL_original.SDL_GL_Unlock")
#pragma comment(linker, "/export:SDL_GL_UpdateRects=SDL_original.SDL_GL_UpdateRects")
#pragma comment(linker, "/export:SDL_GetAppState=SDL_original.SDL_GetAppState")
#pragma comment(linker, "/export:SDL_GetAudioStatus=SDL_original.SDL_GetAudioStatus")
#pragma comment(linker, "/export:SDL_GetClipRect=SDL_original.SDL_GetClipRect")
#pragma comment(linker, "/export:SDL_GetCursor=SDL_original.SDL_GetCursor")
#pragma comment(linker, "/export:SDL_GetError=SDL_original.SDL_GetError")
#pragma comment(linker, "/export:SDL_GetEventFilter=SDL_original.SDL_GetEventFilter")
#pragma comment(linker, "/export:SDL_GetGammaRamp=SDL_original.SDL_GetGammaRamp")
#pragma comment(linker, "/export:SDL_GetKeyName=SDL_original.SDL_GetKeyName")
#pragma comment(linker, "/export:SDL_GetKeyRepeat=SDL_original.SDL_GetKeyRepeat")
#pragma comment(linker, "/export:SDL_GetKeyState=SDL_original.SDL_GetKeyState")
#pragma comment(linker, "/export:SDL_GetModState=SDL_original.SDL_GetModState")
#pragma comment(linker, "/export:SDL_GetMouseState=SDL_original.SDL_GetMouseState")
#pragma comment(linker, "/export:SDL_GetRGB=SDL_original.SDL_GetRGB")
#pragma comment(linker, "/export:SDL_GetRGBA=SDL_original.SDL_GetRGBA")
#pragma comment(linker, "/export:SDL_GetRelativeMouseState=SDL_original.SDL_GetRelativeMouseState")
#pragma comment(linker, "/export:SDL_GetThreadID=SDL_original.SDL_GetThreadID")
#pragma comment(linker, "/export:SDL_GetTicks=SDL_original.SDL_GetTicks")
#pragma comment(linker, "/export:SDL_GetVideoInfo=SDL_original.SDL_GetVideoInfo")
#pragma comment(linker, "/export:SDL_GetVideoSurface=SDL_original.SDL_GetVideoSurface")
#pragma comment(linker, "/export:SDL_GetWMInfo=SDL_original.SDL_GetWMInfo")
#pragma comment(linker, "/export:SDL_Has3DNow=SDL_original.SDL_Has3DNow")
#pragma comment(linker, "/export:SDL_Has3DNowExt=SDL_original.SDL_Has3DNowExt")
#pragma comment(linker, "/export:SDL_HasAltiVec=SDL_original.SDL_HasAltiVec")
#pragma comment(linker, "/export:SDL_HasMMX=SDL_original.SDL_HasMMX")
#pragma comment(linker, "/export:SDL_HasMMXExt=SDL_original.SDL_HasMMXExt")
#pragma comment(linker, "/export:SDL_HasRDTSC=SDL_original.SDL_HasRDTSC")
#pragma comment(linker, "/export:SDL_HasSSE=SDL_original.SDL_HasSSE")
#pragma comment(linker, "/export:SDL_HasSSE2=SDL_original.SDL_HasSSE2")
#pragma comment(linker, "/export:SDL_Init=SDL_original.SDL_Init")
#pragma comment(linker, "/export:SDL_InitSubSystem=SDL_original.SDL_InitSubSystem")
#pragma comment(linker, "/export:SDL_JoystickClose=SDL_original.SDL_JoystickClose")
#pragma comment(linker, "/export:SDL_JoystickEventState=SDL_original.SDL_JoystickEventState")
#pragma comment(linker, "/export:SDL_JoystickGetAxis=SDL_original.SDL_JoystickGetAxis")
#pragma comment(linker, "/export:SDL_JoystickGetBall=SDL_original.SDL_JoystickGetBall")
#pragma comment(linker, "/export:SDL_JoystickGetButton=SDL_original.SDL_JoystickGetButton")
#pragma comment(linker, "/export:SDL_JoystickGetHat=SDL_original.SDL_JoystickGetHat")
#pragma comment(linker, "/export:SDL_JoystickIndex=SDL_original.SDL_JoystickIndex")
#pragma comment(linker, "/export:SDL_JoystickName=SDL_original.SDL_JoystickName")
#pragma comment(linker, "/export:SDL_JoystickNumAxes=SDL_original.SDL_JoystickNumAxes")
#pragma comment(linker, "/export:SDL_JoystickNumBalls=SDL_original.SDL_JoystickNumBalls")
#pragma comment(linker, "/export:SDL_JoystickNumButtons=SDL_original.SDL_JoystickNumButtons")
#pragma comment(linker, "/export:SDL_JoystickNumHats=SDL_original.SDL_JoystickNumHats")
#pragma comment(linker, "/export:SDL_JoystickOpen=SDL_original.SDL_JoystickOpen")
#pragma comment(linker, "/export:SDL_JoystickOpened=SDL_original.SDL_JoystickOpened")
#pragma comment(linker, "/export:SDL_JoystickUpdate=SDL_original.SDL_JoystickUpdate")
#pragma comment(linker, "/export:SDL_KillThread=SDL_original.SDL_KillThread")
#pragma comment(linker, "/export:SDL_Linked_Version=SDL_original.SDL_Linked_Version")
#pragma comment(linker, "/export:SDL_ListModes=SDL_original.SDL_ListModes")
#pragma comment(linker, "/export:SDL_LoadBMP_RW=SDL_original.SDL_LoadBMP_RW")
#pragma comment(linker, "/export:SDL_LoadFunction=SDL_original.SDL_LoadFunction")
#pragma comment(linker, "/export:SDL_LoadObject=SDL_original.SDL_LoadObject")
#pragma comment(linker, "/export:SDL_LoadWAV_RW=SDL_original.SDL_LoadWAV_RW")
#pragma comment(linker, "/export:SDL_LockAudio=SDL_original.SDL_LockAudio")
#pragma comment(linker, "/export:SDL_LockSurface=SDL_original.SDL_LockSurface")
#pragma comment(linker, "/export:SDL_LockYUVOverlay=SDL_original.SDL_LockYUVOverlay")
#pragma comment(linker, "/export:SDL_LowerBlit=SDL_original.SDL_LowerBlit")
#pragma comment(linker, "/export:SDL_MapRGB=SDL_original.SDL_MapRGB")
#pragma comment(linker, "/export:SDL_MapRGBA=SDL_original.SDL_MapRGBA")
#pragma comment(linker, "/export:SDL_MixAudio=SDL_original.SDL_MixAudio")
#pragma comment(linker, "/export:SDL_NumJoysticks=SDL_original.SDL_NumJoysticks")
#pragma comment(linker, "/export:SDL_OpenAudio=SDL_original.SDL_OpenAudio")
#pragma comment(linker, "/export:SDL_PauseAudio=SDL_original.SDL_PauseAudio")
#pragma comment(linker, "/export:SDL_PeepEvents=SDL_original.SDL_PeepEvents")
#pragma comment(linker, "/export:SDL_PollEvent=SDL_original.SDL_PollEvent")
#pragma comment(linker, "/export:SDL_PumpEvents=SDL_original.SDL_PumpEvents")
#pragma comment(linker, "/export:SDL_PushEvent=SDL_original.SDL_PushEvent")
#pragma comment(linker, "/export:SDL_Quit=SDL_original.SDL_Quit")
#pragma comment(linker, "/export:SDL_QuitSubSystem=SDL_original.SDL_QuitSubSystem")
#pragma comment(linker, "/export:SDL_RWFromConstMem=SDL_original.SDL_RWFromConstMem")
#pragma comment(linker, "/export:SDL_RWFromFP=SDL_original.SDL_RWFromFP")
#pragma comment(linker, "/export:SDL_RWFromFile=SDL_original.SDL_RWFromFile")
#pragma comment(linker, "/export:SDL_RWFromMem=SDL_original.SDL_RWFromMem")
#pragma comment(linker, "/export:SDL_ReadBE16=SDL_original.SDL_ReadBE16")
#pragma comment(linker, "/export:SDL_ReadBE32=SDL_original.SDL_ReadBE32")
#pragma comment(linker, "/export:SDL_ReadBE64=SDL_original.SDL_ReadBE64")
#pragma comment(linker, "/export:SDL_ReadLE16=SDL_original.SDL_ReadLE16")
#pragma comment(linker, "/export:SDL_ReadLE32=SDL_original.SDL_ReadLE32")
#pragma comment(linker, "/export:SDL_ReadLE64=SDL_original.SDL_ReadLE64")
#pragma comment(linker, "/export:SDL_RegisterApp=SDL_original.SDL_RegisterApp")
#pragma comment(linker, "/export:SDL_RemoveTimer=SDL_original.SDL_RemoveTimer")
#pragma comment(linker, "/export:SDL_SaveBMP_RW=SDL_original.SDL_SaveBMP_RW")
#pragma comment(linker, "/export:SDL_SemPost=SDL_original.SDL_SemPost")
#pragma comment(linker, "/export:SDL_SemTryWait=SDL_original.SDL_SemTryWait")
#pragma comment(linker, "/export:SDL_SemValue=SDL_original.SDL_SemValue")
#pragma comment(linker, "/export:SDL_SemWait=SDL_original.SDL_SemWait")
#pragma comment(linker, "/export:SDL_SemWaitTimeout=SDL_original.SDL_SemWaitTimeout")
#pragma comment(linker, "/export:SDL_SetAlpha=SDL_original.SDL_SetAlpha")
#pragma comment(linker, "/export:SDL_SetClipRect=SDL_original.SDL_SetClipRect")
#pragma comment(linker, "/export:SDL_SetColorKey=SDL_original.SDL_SetColorKey")
#pragma comment(linker, "/export:SDL_SetColors=SDL_original.SDL_SetColors")
#pragma comment(linker, "/export:SDL_SetCursor=SDL_original.SDL_SetCursor")
#pragma comment(linker, "/export:SDL_SetError=SDL_original.SDL_SetError")
#pragma comment(linker, "/export:SDL_SetEventFilter=SDL_original.SDL_SetEventFilter")
#pragma comment(linker, "/export:SDL_SetGamma=SDL_original.SDL_SetGamma")
#pragma comment(linker, "/export:SDL_SetGammaRamp=SDL_original.SDL_SetGammaRamp")
#pragma comment(linker, "/export:SDL_SetModState=SDL_original.SDL_SetModState")
#pragma comment(linker, "/export:SDL_SetModuleHandle=SDL_original.SDL_SetModuleHandle")
#pragma comment(linker, "/export:SDL_SetPalette=SDL_original.SDL_SetPalette")
#pragma comment(linker, "/export:SDL_SetTimer=SDL_original.SDL_SetTimer")
#pragma comment(linker, "/export:SDL_ShowCursor=SDL_original.SDL_ShowCursor")
#pragma comment(linker, "/export:SDL_SoftStretch=SDL_original.SDL_SoftStretch")
#pragma comment(linker, "/export:SDL_ThreadID=SDL_original.SDL_ThreadID")
#pragma comment(linker, "/export:SDL_UnloadObject=SDL_original.SDL_UnloadObject")
#pragma comment(linker, "/export:SDL_UnlockAudio=SDL_original.SDL_UnlockAudio")
#pragma comment(linker, "/export:SDL_UnlockSurface=SDL_original.SDL_UnlockSurface")
#pragma comment(linker, "/export:SDL_UnlockYUVOverlay=SDL_original.SDL_UnlockYUVOverlay")
#pragma comment(linker, "/export:SDL_UnregisterApp=SDL_original.SDL_UnregisterApp")
#pragma comment(linker, "/export:SDL_UpdateRect=SDL_original.SDL_UpdateRect")
#pragma comment(linker, "/export:SDL_UpdateRects=SDL_original.SDL_UpdateRects")
#pragma comment(linker, "/export:SDL_UpperBlit=SDL_original.SDL_UpperBlit")
#pragma comment(linker, "/export:SDL_VideoDriverName=SDL_original.SDL_VideoDriverName")
#pragma comment(linker, "/export:SDL_VideoInit=SDL_original.SDL_VideoInit")
#pragma comment(linker, "/export:SDL_VideoModeOK=SDL_original.SDL_VideoModeOK")
#pragma comment(linker, "/export:SDL_VideoQuit=SDL_original.SDL_VideoQuit")
#pragma comment(linker, "/export:SDL_WM_GetCaption=SDL_original.SDL_WM_GetCaption")
#pragma comment(linker, "/export:SDL_WM_GrabInput=SDL_original.SDL_WM_GrabInput")
#pragma comment(linker, "/export:SDL_WM_IconifyWindow=SDL_original.SDL_WM_IconifyWindow")
#pragma comment(linker, "/export:SDL_WM_SetIcon=SDL_original.SDL_WM_SetIcon")
#pragma comment(linker, "/export:SDL_WaitEvent=SDL_original.SDL_WaitEvent")
#pragma comment(linker, "/export:SDL_WaitThread=SDL_original.SDL_WaitThread")
#pragma comment(linker, "/export:SDL_WarpMouse=SDL_original.SDL_WarpMouse")
#pragma comment(linker, "/export:SDL_WasInit=SDL_original.SDL_WasInit")
#pragma comment(linker, "/export:SDL_WriteBE16=SDL_original.SDL_WriteBE16")
#pragma comment(linker, "/export:SDL_WriteBE32=SDL_original.SDL_WriteBE32")
#pragma comment(linker, "/export:SDL_WriteBE64=SDL_original.SDL_WriteBE64")
#pragma comment(linker, "/export:SDL_WriteLE16=SDL_original.SDL_WriteLE16")
#pragma comment(linker, "/export:SDL_WriteLE32=SDL_original.SDL_WriteLE32")
#pragma comment(linker, "/export:SDL_WriteLE64=SDL_original.SDL_WriteLE64")
#pragma comment(linker, "/export:SDL_getenv=SDL_original.SDL_getenv")
#pragma comment(linker, "/export:SDL_iconv=SDL_original.SDL_iconv")
#pragma comment(linker, "/export:SDL_iconv_close=SDL_original.SDL_iconv_close")
#pragma comment(linker, "/export:SDL_iconv_open=SDL_original.SDL_iconv_open")
#pragma comment(linker, "/export:SDL_iconv_string=SDL_original.SDL_iconv_string")
#pragma comment(linker, "/export:SDL_lltoa=SDL_original.SDL_lltoa")
#pragma comment(linker, "/export:SDL_mutexP=SDL_original.SDL_mutexP")
#pragma comment(linker, "/export:SDL_mutexV=SDL_original.SDL_mutexV")
#pragma comment(linker, "/export:SDL_putenv=SDL_original.SDL_putenv")
#pragma comment(linker, "/export:SDL_revcpy=SDL_original.SDL_revcpy")
#pragma comment(linker, "/export:SDL_snprintf=SDL_original.SDL_snprintf")
#pragma comment(linker, "/export:SDL_strdup=SDL_original.SDL_strdup")
#pragma comment(linker, "/export:SDL_strlcat=SDL_original.SDL_strlcat")
#pragma comment(linker, "/export:SDL_strlcpy=SDL_original.SDL_strlcpy")
#pragma comment(linker, "/export:SDL_strtoull=SDL_original.SDL_strtoull")
#pragma comment(linker, "/export:SDL_ulltoa=SDL_original.SDL_ulltoa")
#pragma comment(linker, "/export:SDL_vsnprintf=SDL_original.SDL_vsnprintf")

// Also link to the original SDL library
#pragma comment(lib, "SDL_original.lib")

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char className[256];
    GetClassNameA(hwnd, className, sizeof(className));
    if (strstr(className, "SDL") != NULL) {
        *(HWND*)lParam = hwnd;
        return FALSE;
    }
    return TRUE;
}

HWND FindSDLWindow() {
    HWND hwnd = FindWindowA("SDL_app", NULL);
    if (!hwnd) {
        EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);
    }
    return hwnd;
}

void InitializeHooks() {
    hOriginalSDL = LoadLibraryA("SDL_original.dll");
    if (hOriginalSDL) {
        Real_SDL_SetVideoMode = (SDL_SetVideoMode_t)GetProcAddress(hOriginalSDL, "SDL_SetVideoMode");
        Real_SDL_WM_ToggleFullScreen = (SDL_WM_ToggleFullScreen_t)GetProcAddress(hOriginalSDL, "SDL_WM_ToggleFullScreen");
        Real_SDL_WM_SetCaption = (SDL_WM_SetCaption_t)GetProcAddress(hOriginalSDL, "SDL_WM_SetCaption");
    }
}

// Our hook functions - exported with the SDL names
extern "C" __declspec(dllexport) SDL_Surface* __cdecl SDL_SetVideoMode(int width, int height, int bpp, unsigned int flags) {
    // Initialize on first call
    if (!Real_SDL_SetVideoMode) {
        InitializeHooks();
    }
    
    if (!Real_SDL_SetVideoMode) {
        return NULL; // Failed to load original function
    }
    
    if (g_forceFrameless && !(flags & 0x80000000)) {
        flags |= SDL_NOFRAME;
    }
    
    // Call the original function
    SDL_Surface* result = Real_SDL_SetVideoMode(width, height, bpp, flags);
    
    if (result) {
        // If not fullscreen, center the window
        if (!(flags & 0x80000000)) {
            g_hGameWindow = FindSDLWindow();
            if (g_hGameWindow) {
                RECT desktop;
                GetWindowRect(GetDesktopWindow(), &desktop);
                int x = (desktop.right - width) / 2;
                int y = (desktop.bottom - height) / 2;
                SetWindowPos(g_hGameWindow, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
            }
        }
    }
    
    return result;
}

extern "C" __declspec(dllexport) int __cdecl SDL_WM_ToggleFullScreen(SDL_Surface* surface) {
    if (!Real_SDL_WM_ToggleFullScreen) {
        InitializeHooks();
    }
    
    if (!Real_SDL_WM_ToggleFullScreen) {
        return 0;
    }
    
    return Real_SDL_WM_ToggleFullScreen(surface);
}

extern "C" __declspec(dllexport) void __cdecl SDL_WM_SetCaption(const char* title, const char* icon) {
    if (!Real_SDL_WM_SetCaption) {
        InitializeHooks();
    }
    
    if (!Real_SDL_WM_SetCaption) {
        return;
    }
    
    Real_SDL_WM_SetCaption(title, icon);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        InitializeHooks();
        break;
    case DLL_PROCESS_DETACH:
        if (hOriginalSDL) {
            FreeLibrary(hOriginalSDL);
        }
        break;
    }
    return TRUE;
}