@echo off
cls
REM Compilar Poligamia
echo Compilando...
g++ -g source/poli17.cpp source/resource.o -o build/poli17.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
echo El programa poligamia fue compilado exitosamente!
@echo on