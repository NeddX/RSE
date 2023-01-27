@echo off
del RSE.sln
del ZERO_CHECK.vcxproj
del ZERO_CHECK.vcxproj.filters
del ALL_BUILD.vcxproj
del ALL_BUILD.vcxproj.filters
del cmake_install.cmake
del CMakeCache.txt
rmdir /S /Q "x64"
rmdir /S /Q "CMakeFiles"