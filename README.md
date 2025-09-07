# SDL_NoFrame

SDL_NoFrame is a small proxy library for SDL 1.2 based games (such as M.U.G.E.N) that forces them to run in native borderless mode. When used in combination with [Borderless Gaming](https://store.steampowered.com/app/388080/Borderless_Gaming/), you can achieve true borderless fullscreen gaming.

![SDL_NoFrame in action](./demo.gif)

## What it does

This proxy DLL intercepts SDL window creation calls and removes the window frame, making any SDL 1.2 game run borderless. This is particularly useful for older games that don't have built-in borderless window options.

## Installation

1. **Download the latest release** from the [Releases](https://github.com/andrewmd5/SDL_NoFrame/releases) page

2. **Backup your original SDL.dll**
   - Navigate to your game's directory
   - Find `SDL.dll` 
   - Rename it to `SDL_original.dll`

3. **Install SDL_NoFrame**
   - Extract the downloaded `SDL.dll` (the proxy) to your game's directory
   - Ensure both `SDL.dll` (proxy) and `SDL_original.dll` (original) are in the same folder

4. **Run your game** - it should now start in borderless mode

## Troubleshooting

### Missing SDL_original.dll Error

If you encounter this error:

```
---------------------------
Hyper DBZ.exe - System Error
---------------------------
The code execution cannot proceed because SDL_original.dll was not found. 
Reinstalling the program may fix this problem. 
---------------------------
OK   
---------------------------
```

**Solution:** Make sure `SDL_original.dll` is placed in the same directory as the game executable.

## Making it Fullscreen with Borderless Gaming

1. Launch your game with SDL_NoFrame installed (game should be borderless)
2. Open Borderless Gaming
3. Create a profile for your game
4. If the game supports resolution changes internally, adjust the resolution to match your monitor
5. If not, use an app container to upscale it - [see this guide](https://steamcommunity.com/sharedfiles/filedetails/?id=3553017752)

## Important Notes

⚠️ **Only use SDL_NoFrame if Borderless Gaming cannot make the game borderless on its own.** Try Borderless Gaming first - if it works, you don't need this proxy.

## Compatibility

- Works with SDL 1.2 (32-bit) based games
- Tested with M.U.G.E.N and various fighting game engines
- Windows only

## Building from Source

If you want to compile the proxy yourself:

1. Install Visual Studio with C++ development tools
2. Run `generate_lib.bat` to create the import library
3. Compile with: `cl /LD /MT SDL_Proxy.cpp /Fe:SDL.dll /link /MACHINE:X86 /DEF:hooks.def`

## License

This project is provided as-is for the purpose of improving gaming experience with older SDL-based games.