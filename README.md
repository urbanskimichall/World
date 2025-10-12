# kot_project

Minimal C++ project using CMake.

Build (PowerShell):

```powershell
mkdir build; cd build; cmake .. -G "Visual Studio 16 2019"; cmake --build . --config Release
```

Or with Ninja (if installed):

```powershell
mkdir build; cd build; cmake .. -G Ninja; cmake --build .
```

Run:

```powershell
# from build/Release or build depending on generator
./kot_app
```

Run tests:

```powershell
ctest -C Release
```
