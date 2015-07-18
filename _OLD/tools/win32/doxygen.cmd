@echo off
(type "%1" & echo DOT_FONTNAME=Arial & echo DOT_FONTPATH=C:\WINDOWS\FONTS & echo DOT_PATH=tools/win32) ^
    | %~dp0doxygen.exe -
