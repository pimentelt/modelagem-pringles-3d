@echo off
setlocal
cd /d "%~dp0"

set "RAYLIB_GCC=C:\msys64\ucrt64\bin\gcc.exe"
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

if not exist "%RAYLIB_GCC%" (
    echo ERRO: nao encontrei o GCC do MSYS2 em:
    echo %RAYLIB_GCC%
    echo Instale o MSYS2 UCRT64 com GCC e raylib, ou ajuste o caminho neste arquivo.
    pause
    exit /b 1
)

echo Compilando o projeto...
"%RAYLIB_GCC%" main.c pringles.c formiga.c -o pringles-dev.exe -std=c11 -Wall -Wextra -IC:\msys64\ucrt64\include -LC:\msys64\ucrt64\lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

if errorlevel 1 (
    echo.
    echo A compilacao falhou. Confira as mensagens acima.
    pause
    exit /b 1
)

echo.
echo Compilacao concluida. Iniciando a visualizacao...
pringles-dev.exe

echo.
echo O programa foi encerrado.
pause
