if not exist build md build
cmake -G "Visual Studio 16 2019" -A x64 -H. -Bbuild
pause