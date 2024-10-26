@echo off
setlocal enabledelayedexpansion

echo [*] Data files to be compressed:
for %%f in (data\*.txt) do (
    echo  - %%f
)

set compressed_counter=0

echo.
echo [*] Compressing data files...
for %%f in (data\*.txt) do (
    echo.
    echo - Compressing %%f...
    .\bin\huffman.exe -c "%%f" >nul
    if errorlevel 1 (
        echo   ! ERROR: Compression failed for %%f
    ) else (
        echo   + OK: Compression successful for %%f
        set /a compressed_counter+=1
    )
)

echo.
echo [*] Compression summary:
for /f %%c in ('dir /b data\*.txt ^| find /c /v ""') do set total_files=%%c
echo  - Total files compressed: !compressed_counter! / !total_files!

exit /b 0