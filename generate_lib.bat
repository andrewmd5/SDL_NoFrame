@echo off
setlocal enabledelayedexpansion

REM Set the path to the original SDL.dll
set "ORIGINAL_SDL=SDL\1.2\SDL.dll"

REM Generate SDL_original.lib from SDL.dll in SDL\1.2 folder
echo Generating SDL_original.lib from %ORIGINAL_SDL%...

REM Check if the original SDL.dll exists in the subfolder
if not exist "%ORIGINAL_SDL%" (
    echo ERROR: %ORIGINAL_SDL% not found!
    exit /b 1
)

REM Copy the original SDL.dll to SDL_original.dll if not already done
if not exist SDL_original.dll (
    echo Copying %ORIGINAL_SDL% to SDL_original.dll...
    copy "%ORIGINAL_SDL%" SDL_original.dll
)

REM Export the symbols using /OUT for cleaner output
echo Extracting exports from SDL_original.dll...
dumpbin /EXPORTS /OUT:exports.txt SDL_original.dll

REM Create the DEF file header
echo Creating DEF file...
echo LIBRARY SDL_original > SDL_original.def
echo EXPORTS >> SDL_original.def

REM Parse exports - the format after "ordinal hint" header is:
REM    ordinal hint RVA      name
REM We want the 4th column (name) from lines that start with spaces and numbers
set "ordinal=0"
for /f "usebackq tokens=*" %%a in ("exports.txt") do (
    set "line=%%a"
   
    REM Check if line starts with spaces followed by a number (export entry)
    echo !line! | findstr /R "^[ ][ ]*[0-9]" >nul
    if !errorlevel! equ 0 (
        REM Extract the 4th token (function name)
        for /f "tokens=4" %%b in ("!line!") do (
            set "funcname=%%b"
            if not "!funcname!"=="" (
                REM Skip if it's "Summary" or other non-function text
                if not "!funcname!"=="Summary" (
                    set /a ordinal+=1
                    echo     !funcname! @!ordinal! >> SDL_original.def
                )
            )
        )
    )
)

REM Generate the LIB file
echo Generating import library...
lib /DEF:SDL_original.def /OUT:SDL_original.lib /MACHINE:X86

REM Clean up
del exports.txt

echo Done! SDL_original.lib has been created from %ORIGINAL_SDL%.
echo.
echo Now you can compile your proxy with:
echo cl /LD /MT SDL_Proxy.cpp /Fe:SDL.dll /link /MACHINE:X86 /DEF:hooks.def

endlocal