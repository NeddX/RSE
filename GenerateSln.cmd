@echo off
if exist RSE.sln (
 call EraseSln.cmd
)
cmake -G "Visual Studio 17"