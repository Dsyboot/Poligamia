@echo off
cls
REM Compilar Poligamia
echo Compilando...
g++ -g poli17.cpp resource.o -o poli17.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
echo El programa poligamia fue compilado exitosamente!
@echo on