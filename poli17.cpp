/* Importar las librerias */
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <json.hpp>
#include "dsygraph.hpp"
#include "dsyutil.hpp"

#define PROGRAM_TITLE "poligamia"
#define PROGRAM_TITLE_FIGURE "poligamia Figure Viewer"

/* Usar el espacio de nombres std, dsy, json */
using namespace std;
using namespace dsy;
using ordered_json = nlohmann::ordered_json;

/* Variables globales */
Point default_win_size(640, 480);
RGBColor default_border_color(18, 226, 216); // Celeste
RGBColor default_points_color(255, 128, 0); // Naranja
RGBColor default_line_color(255, 255, 255); // Blanco
bool can_connect = false; // Botón C
bool can_line_appear = true; // Botón L
bool can_show_points = true; // Botón P
bool can_triangulate = false; // Botón T
bool show_points_quantity = true;
bool show_triangles_quantity = true;
bool show_mouse_coordinates = true;
Window win(default_win_size, PROGRAM_TITLE);
Figure figurits;

/* Funciones principales */
void main_window();
void load_config();
void save_config(bool console = true);
void info_window();
void printFigureData();

/* Funciones extras para manejo de archivos JSON */
ordered_json load_json(const char* file);
void save_json(const char* file, ordered_json content);

/* Funciones extras para manejar datos de las figuras */
Figure getFigureFromFile(const char* file);
void saveFigureFromFile(const char* file, Figure figura);

/* Funciones del algoritmo de la oreja (triangulacion) */
vector<Triangle> triangulateEarClipping(vector<Point>& polygon);
bool isPointInTriangle(const Point& p, const Point& a, const Point& b, const Point& c);
bool isCCW(const Point& a, const Point& b, const Point& c);
bool isEar(const vector<Point>& polygon, size_t i);

/* Funciones para calcular el area y el perimetro del poligono */
double distancia(const Point &a, const Point &b);
double getPolyArea(const vector<Triangle> &triangles);
double getPolyPerimetro(const vector<Triangle> &triangles);

/* Funciones extras de dibujo */
void center_filled_rectangle(int cX, int cY, int radX, int radY);

/* Funciones de validacion */
bool exists(char val, size_t size, char data[]);
bool validInt(const String &value);

/* Funcion principal */
int main() {
    system("cls"); // Limpiar pantalla

    // Introduccion del programa
    cout<<String::format("Bienvenido a poligamia! \n\n");

    // Pedir eleccion del usuario
    String opcion;
    while (true)
    {
        cout<<"Opciones: "<<endl<<endl;
        cout<<"1: Iniciar modo grafico"<<endl;
        cout<<"2: Cambiar configuracion"<<endl;
        cout<<"3: Informacion"<<endl;
        
        figurits = getFigureFromFile("saved_figure.json");
        if (!figurits.empty()) {
            cout<<"4: Informacion de la figura"<<endl;
        }

        cout<<"Otro: Salir"<<endl;
        cout<<"Ingrese el numero de la opcion a elegir: ";
        cin>>opcion;

        if (validInt(opcion))
        {
            if (opcion.equals("1")) {
                cout<<"Iniciando modo grafico..."<<endl;
                delay(500);

                // Preparando el inicio de la ventana
                load_config(); // Cargar datos del json
                main_window(); // Iniciar el modo grafico
            }
            else if (opcion.equals("2")) {
                cout<<"Entrando a configuracion..."<<endl;
                delay(1250);
                save_config();
            }
            else if (opcion.equals("3")) {
                cout<<"Cargando informacion..."<<endl;
                delay(750);
                info_window();
            }
            else if (opcion.equals("4") && !figurits.empty()) {
                cout<<"Cargando informacion de la ultima figura..."<<endl;
                delay(750);
                load_config(); // Cargar datos del json
                printFigureData(); // Cargar modo grafico
            }
            else {
                cout<<"Vuelva pronto!"<<endl;
                delay(750);
                break;
            }
        }
        else {
            cout<<"Ups, no has ingresado un numero entero valido!"<<endl;
            delay(750);
            system("cls");
        }
    }

    return 0;
}

void main_window() {
    // Limpiar todo el modo gráfico
    win.initWindow();
    win.clearWindow();

    /* Variables controladoras */
    int key = 0, curPoints = -1;
    int trianPTS = 0; // Para obtener las teclas presionadas
    Point selectionLimit[] = {Point(10, 10), Point(win.getWindowSizeX() - 10, win.getWindowSizeY() - 10)}; // Para el borde

    /* Arreglo de puntos y triangulos */
    vector<Point> polygon;
    vector<Triangle> trian;
    Point old(-1, -1);

    /* Controlador del mouse y texto */
    Point mousePos, oldMousePos(-1, -1);

    while (true) {
        /* Capturar posición del mouse */
        mousePos = Mouse::getMousePos();

        /* Lógicas */
        if (Mouse::mouseKeyClicked(MOUSE_KEY_LEFT)) {
            if (old.getX() != mousePos.getX() || old.getY() != mousePos.getY()) {
                /* Poner los puntos dentro de los limites del borde */
                if ((mousePos.getX() > selectionLimit[0].getX() && mousePos.getY() > selectionLimit[0].getY()) && (mousePos.getX() < selectionLimit[1].getX() && mousePos.getY() < selectionLimit[1].getY()))
                {
                    polygon.push_back(mousePos);
                    old = mousePos;
                }
            }
        }
        
        if (kbhit()) {
            key = getch();
            
            /* Si se presiona la tecla escape, salir */
            if (key == KEYBOARD_KEY_ESCAPE) {
                break;
            }
            
            /*## Control de botones ##*/
            /* Si se presiona, se controla la conexion de los puntos */
            if (key == KEYBOARD_KEY_C) {
                can_connect = !can_connect;
            }

            /* Si se presiona, se controla la conexio de lineas */
            if (key == KEYBOARD_KEY_L) {
                can_line_appear = !can_line_appear;
            }

            /* Si se presiona, se controla la observacion de puntos */
            if (key == KEYBOARD_KEY_P) {
                can_show_points = !can_show_points;
            }

            /* Si se presiona, se guarda el dato de la figura */
            if (key == KEYBOARD_KEY_S)
            {
                if (polygon.size() >= 3)
                {
                    saveFigureFromFile("saved_figure.json", Figure(polygon, "default"));
                    win.clearWindow();
                    DrawingText::configureMargin(CENTER_TEXT, CENTER_TEXT);
                    DrawingText::drawText(win.getWindowSizeX() / 2, win.getWindowSizeY() / 2,"Guardando la figura...");
                    DrawingText::configureMargin(LEFT_TEXT, LEFT_TEXT);
                    delay(1500);

                    /* Esperar que se termine de presionar las teclas */
                    while (true) {
                        if (kbhit()) {
                            char crt = getch();
                        }
                        else {
                            break;
                        }
                    }
                }
            }

            /* Si se presiona, se controla la generacion de triangulacion */
            if (key == KEYBOARD_KEY_T) {
                can_triangulate = !can_triangulate;
                trianPTS = 0;
            }

            /* Si se presiona, se elimina el ultimo punto */
            if (key == KEYBOARD_KEY_BACKSPACE)
            {
                if (polygon.size() > 0)
                {
                    polygon.pop_back();
                    trianPTS = polygon.size() < 3 ? 0 : trianPTS;
                }
            }

            // Limpiar pantalla
            win.clearWindow();
        }

        if (can_show_points) {
            /* Dibujar círculos para simular puntos */
            Draw::setColor(default_points_color); // Establecer el color
            for (size_t i = 0; i < polygon.size(); i++) {
                Point tmp = polygon[i];
                Draw::circle(tmp.getX(), tmp.getY(), 2);
            }
        }
        
        if (can_line_appear) {
            Draw::setColor(default_line_color); // Establecer el color
            if (polygon.size() > 1)
            {
                Draw::drawPoly(polygon, can_connect);
            }
            else if (polygon.size() == 1)
            {
                Draw::circle(polygon[0].getX(), polygon[0].getY(), 1);
            }

            if (can_connect && can_triangulate) {
                /* Ejecutar si los puntos cambian */
                if (curPoints != polygon.size()) {
                    curPoints = polygon.size();
                    trian = triangulateEarClipping(polygon);
                }

                /* Dibujar si la longitud no es 0 */
                if (trian.size() > 0) {
                    Draw::setColor(default_line_color); // Establecer el color
                    Draw::drawPoly(trian, can_connect);
                    trianPTS = trian.size();
                }
            }
        }
        
        /* Dibujar contorno decorativo de la ventana (RGB / CELESTE) */
        RGBColor rgb_values = Draw::generateRGB(3.9);
        default_border_color.itsNULL() ? Draw::setColor(rgb_values) : Draw::setColor(default_border_color); // Establecer el color del borde
        Draw::rectangle(selectionLimit[0].getX(), selectionLimit[0].getY(), selectionLimit[1].getX(), selectionLimit[1].getY());

        /* Dibujar el texto */
        DrawingText::drawText(20, 20, String::format("Puntos: %d", polygon.size()).toArray());
        DrawingText::drawText(20, 37, String::format("Triangulos: %d", trianPTS).toArray());
        DrawingText::drawText(20, 54, String::format("Coordenadas del mouse: (%d, %d)", mousePos.getX(), mousePos.getY()).toArray());

        /* Limpiar pantalla al mover el mouse */
        if (mousePos.getX() != oldMousePos.getX() || mousePos.getY() != oldMousePos.getY())
        {
            oldMousePos = mousePos;
            win.clearWindow();
        }
    }

    win.closeWindow();
    system("cls"); // Limpiar la consola al salir
}

/* Funcion para cargar la configuracion del programa */
void load_config() {
    /* Preparar todas las variables para el cargado del archivo */
    ordered_json fileData = load_json("poligamia.json");
    Point winsize;
    RGBColor border_color, points_color, line_color;
    bool csp, ctl, cc, ct, spq, stq, smc;

    /* Intentar obtener todos los valores y cargarlos */
    try
    {
        /* Cargar los datos del archivo json */
        winsize = Point(fileData["config_data"]["win_size"][0], fileData["config_data"]["win_size"][1]);
        if (fileData["config_data"]["poly_data"]["border_color"] == NULL) {
            border_color = RGBColor();
        }
        else {
            border_color = RGBColor(fileData["config_data"]["poly_data"]["border_color"][0], fileData["config_data"]["poly_data"]["border_color"][1], fileData["config_data"]["poly_data"]["border_color"][2]);
        }
        points_color = RGBColor(fileData["config_data"]["poly_data"]["points_color"][0], fileData["config_data"]["poly_data"]["points_color"][1], fileData["config_data"]["poly_data"]["points_color"][2]);
        line_color = RGBColor(fileData["config_data"]["poly_data"]["lines_color"][0], fileData["config_data"]["poly_data"]["lines_color"][1], fileData["config_data"]["poly_data"]["lines_color"][2]);

        /* Obtener las opciones que se usan en el programa */
        csp = fileData["config_data"]["information"]["can_show_points"];
        ctl = fileData["config_data"]["information"]["can_trace_lines"];
        cc = fileData["config_data"]["information"]["can_connect"];
        ct = fileData["config_data"]["information"]["can_triangulate"];
        spq = fileData["config_data"]["information"]["show_points_quantity"];
        stq = fileData["config_data"]["information"]["show_triangles_quantity"];
        smc = fileData["config_data"]["information"]["show_mouse_coordinates"];

        /* Guardar los datos en las variables del programa */
        default_win_size = winsize;
        default_border_color = border_color;
        default_line_color = line_color;
        default_points_color = points_color;
        can_show_points = csp;
        can_line_appear = ctl;
        can_connect = cc;
        can_triangulate = ct;
        show_points_quantity = spq;
        show_triangles_quantity = stq;
        show_mouse_coordinates = smc;
    }
    catch(...) {
        // Crear datos nuevos en caso de algun error
        save_config(false);
    }

    /* Asignar el tamanio de la ventana y el titulo */
    win.modifyValues(default_win_size, PROGRAM_TITLE);
}

/* Funcion para guardar la configuracion del programa */
void save_config(bool console) {
    /* Preparar todas las variables para el guardado del archivo */
    ordered_json fileData = load_json("poligamia.json");
    Point winsize;
    RGBColor border_color, points_color, line_color;
    bool csp, ctl, cc, ct, spq, stq, smc;
    String opcion;

    /* Cargar las variables con los valores del json si es que no esta vacio */
    try
    {
        /* Cargar los datos del archivo json */
        winsize = Point(fileData["config_data"]["win_size"][0], fileData["config_data"]["win_size"][1]);
        if (fileData["config_data"]["poly_data"]["border_color"] == NULL) {
            border_color = RGBColor();
        }
        else {
            border_color = RGBColor(fileData["config_data"]["poly_data"]["border_color"][0], fileData["config_data"]["poly_data"]["border_color"][1], fileData["config_data"]["poly_data"]["border_color"][2]);
        }
        points_color = RGBColor(fileData["config_data"]["poly_data"]["points_color"][0], fileData["config_data"]["poly_data"]["points_color"][1], fileData["config_data"]["poly_data"]["points_color"][2]);
        line_color = RGBColor(fileData["config_data"]["poly_data"]["lines_color"][0], fileData["config_data"]["poly_data"]["lines_color"][1], fileData["config_data"]["poly_data"]["lines_color"][2]);

        /* Obtener las opciones que se usan en el programa */
        csp = fileData["config_data"]["information"]["can_show_points"];
        ctl = fileData["config_data"]["information"]["can_trace_lines"];
        cc = fileData["config_data"]["information"]["can_connect"];
        ct = fileData["config_data"]["information"]["can_triangulate"];
        spq = fileData["config_data"]["information"]["show_points_quantity"];
        stq = fileData["config_data"]["information"]["show_triangles_quantity"];
        smc = fileData["config_data"]["information"]["show_mouse_coordinates"];
    }
    catch(...) {
        /* Si hay un error, usar los valores de fabrica del programa */
        winsize = default_win_size;
        border_color = default_border_color;
        points_color = default_points_color;
        line_color = default_line_color;
        csp = can_show_points;
        ctl = can_line_appear;
        cc = can_connect;
        ct = can_triangulate;
        spq = show_points_quantity;
        stq = show_triangles_quantity;
        smc = show_mouse_coordinates;
    }
    
    if (console) {
        /* Mostrar el menu de las opciones */
        while (true)
        {
            /* Imprimir informacion que puede ser guardada */
            system("cls");
            cout<<"#### Configuracion del programa ####"<<endl<<endl;
            cout<<"1: Tamanio de la ventana"<<endl;
            cout<<"2: Color del borde"<<endl;
            cout<<"3: Color de los puntos"<<endl;
            cout<<"4: Color de las lineas"<<endl;
            cout<<"Otro: Volver atras"<<endl;
            cout<<">>> ";
            cin>>opcion;

            /* Dependiendo la opcion realizar algo */
            if (validInt(opcion))
            {
                /* Window size */
                if (opcion.equals("1"))
                {
                    /* Variables temporales */
                    String tmp_x, tmp_y;

                    /* Dejarle elegir al usuario */
                    system("cls");
                    while (true)
                    {
                        cout<<String::format("Datos de la ventana actuales: (%d, %d)\n\n", winsize.getX(), winsize.getY());
                        cout<<"Por favor ingrese el valor de x: (valor horizontal)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_x; // Guardar el string temporal
                        cout<<"Por favor ingrese el valor de y: (valor vertical)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_y; // Guardar el string temporal

                        if (validInt(tmp_x) && validInt(tmp_y))
                        {
                            /* Convertir a entero */
                            int size_x = Convert::toInt(tmp_x);
                            int size_y = Convert::toInt(tmp_y);

                            /* Aplicar formato y guardar */
                            size_x = size_x >= 320 ? size_x : 640;
                            size_y = size_y >= 240 ? size_y : 480;
                            winsize = Point(size_x, size_y);

                            /* Mostrar mensaje final */
                            cout<<String::format("Se ha guardado la ventana con estos nuevos datos: (%d, %d)\n", size_x, size_y);
                            system("pause"); /* Esperar una tecla */

                            break;
                        }
                        else {
                            cout<<"Ups, no has ingresado un numero entero valido!"<<endl;
                            delay(750);
                            system("cls");
                        }
                    }
                }
                /* Border color */
                else if (opcion.equals("2"))
                {
                    /* Variables temporales */
                    String input, tmp_r, tmp_g, tmp_b;

                    /* Limpiar pantalla y preguntar datos */
                    system("cls");
                    while (true)
                    {
                        cout<<String::format("Color del borde actual: %s\n\n", border_color.itsNULL() ? "RGB" : String::format("(%d, %d, %d)", border_color.getRedValue(), border_color.getGreenValue(), border_color.getBlueValue()).toArray());
                        cout<<"Ingrese 'NULL', 'RGB', o 'VAL' para ingresar el color del borde"<<endl;
                        cout<<">>> ";
                        cin>>input;

                        if (input.equals("NULL") || input.equals("RGB")) {
                            border_color = RGBColor(); // Color nulo

                            /* Mostrar mensaje final */
                            cout<<String::format("Se ha guardado el color del borde con este nuevos dato: RGB\n");
                            system("pause"); /* Esperar una tecla */
                            break;
                        }
                        else if (input.equals("VAL")) {
                            cout<<"Ingrese el color rojo (0-255)"<<endl;
                            cout<<">>> ";
                            cin>>tmp_r;

                            cout<<"Ingrese el color verde (0-255)"<<endl;
                            cout<<">>> ";
                            cin>>tmp_g;

                            cout<<"Ingrese el color azul (0-255)"<<endl;
                            cout<<">>> ";
                            cin>>tmp_b;

                            /* Verificar que los datos sean numeros validos */
                            if (validInt(tmp_r) && validInt(tmp_g) && validInt(tmp_b))
                            {
                                int red = Convert::toInt(tmp_r);
                                int green = Convert::toInt(tmp_g);
                                int blue = Convert::toInt(tmp_b);

                                /* Verificar que los numeros esten en el rango correcto */
                                if ((red >= 0 && red <= 255) && (green >= 0 && green <= 255) && (blue >= 0 && blue <= 255))
                                {
                                    border_color = RGBColor(red, green, blue);

                                    /* Mostrar mensaje final */
                                    cout<<String::format("Se ha guardado el color del borde con estos nuevos datos: (%d, %d, %d)\n", red, green, blue);
                                    system("pause"); /* Esperar una tecla */
                                    break;
                                }
                                else {
                                    cout<<"El rango RGB debe de estar entre 0 y 255!"<<endl;
                                    system("pause");
                                    system("cls");
                                }
                            }
                            else {
                                cout<<"Ups, no has ingresado un numero entero valido!"<<endl;
                                delay(750);
                                system("cls");
                            }
                        }
                        else {
                            cout<<"Los valores deben de ser los mismos en mayusculas!"<<endl;
                            system("pause");
                            system("cls");
                        }
                    }
                }
                /* Points color */
                else if (opcion.equals("3"))
                {
                    /* Variables temporales */
                    String tmp_r, tmp_g, tmp_b;

                    /* Limpiar pantalla y preguntar datos */
                    system("cls");
                    while (true)
                    {
                        cout<<String::format("Color de los puntos actual: (%d, %d, %d)\n\n", points_color.getRedValue(), points_color.getGreenValue(), points_color.getBlueValue());
                        cout<<"Ingrese el color rojo (0-255)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_r;

                        cout<<"Ingrese el color verde (0-255)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_g;

                        cout<<"Ingrese el color azul (0-255)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_b;

                        /* Verificar que los datos sean numeros validos */
                        if (validInt(tmp_r) && validInt(tmp_g) && validInt(tmp_b))
                        {
                            int red = Convert::toInt(tmp_r);
                            int green = Convert::toInt(tmp_g);
                            int blue = Convert::toInt(tmp_b);

                            /* Verificar que los numeros esten en el rango correcto */
                            if ((red >= 0 && red <= 255) && (green >= 0 && green <= 255) && (blue >= 0 && blue <= 255))
                            {
                                points_color = RGBColor(red, green, blue);

                                /* Mostrar mensaje final */
                                cout<<String::format("Se ha guardado el color de los puntos con estos nuevos datos: (%d, %d, %d)\n", red, green, blue);
                                system("pause"); /* Esperar una tecla */
                                break;
                            }
                            else {
                                cout<<"El rango RGB debe de estar entre 0 y 255!"<<endl;
                                system("pause");
                                system("cls");
                            }
                        }
                        else {
                            cout<<"Ups, no has ingresado un numero entero valido!"<<endl;
                            system("pause");
                            system("cls");
                        }
                    }
                }
                /* Lines color */
                else if (opcion.equals("4"))
                {
                    /* Variables temporales */
                    String tmp_r, tmp_g, tmp_b;

                    /* Limpiar pantalla y preguntar datos */
                    system("cls");
                    while (true)
                    {
                        cout<<String::format("Color de los puntos actual: (%d, %d, %d)\n\n", line_color.getRedValue(), line_color.getGreenValue(), line_color.getBlueValue());
                        cout<<"Ingrese el color rojo (0-255)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_r;

                        cout<<"Ingrese el color verde (0-255)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_g;

                        cout<<"Ingrese el color azul (0-255)"<<endl;
                        cout<<">>> ";
                        cin>>tmp_b;

                        /* Verificar que los datos sean numeros validos */
                        if (validInt(tmp_r) && validInt(tmp_g) && validInt(tmp_b))
                        {
                            int red = Convert::toInt(tmp_r);
                            int green = Convert::toInt(tmp_g);
                            int blue = Convert::toInt(tmp_b);

                            /* Verificar que los numeros esten en el rango correcto */
                            if ((red >= 0 && red <= 255) && (green >= 0 && green <= 255) && (blue >= 0 && blue <= 255))
                            {
                                line_color = RGBColor(red, green, blue);

                                /* Mostrar mensaje final */
                                cout<<String::format("Se ha guardado el color de las lienas con estos nuevos datos: (%d, %d, %d)\n", red, green, blue);
                                system("pause"); /* Esperar una tecla */
                                break;
                            }
                            else {
                                cout<<"El rango RGB debe de estar entre 0 y 255!"<<endl;
                                system("pause");
                                system("cls");
                            }
                        }
                        else {
                            cout<<"Ups, no has ingresado un numero entero valido!"<<endl;
                            system("pause");
                            system("cls");
                        }
                    }
                }
                else {
                    cout<<"Regresando y guardando..."<<endl;
                    delay(750);
                    system("cls");
                    break;
                }
                
            }
            else {
                cout<<"Ups, no has ingresado un numero entero valido!"<<endl;
                delay(750);
                system("cls");
            }
        }
    }

    /* Guardar absolutamente todos los datos y configurar el formato del json y la ventana */
    fileData["format_version"] = "2.0.0";
    fileData["config_data"]["win_size"] = {winsize.getX(), winsize.getY()};
    fileData["config_data"]["win_title"] = PROGRAM_TITLE;

    /* Configurar los colores de los poligonos */
    if (border_color.itsNULL()) {
        fileData["config_data"]["poly_data"]["border_color"] = NULL;
    }
    else {
        fileData["config_data"]["poly_data"]["border_color"] = {border_color.getRedValue(), border_color.getGreenValue(), border_color.getBlueValue()};
    }

    fileData["config_data"]["poly_data"]["lines_color"] = {line_color.getRedValue(), line_color.getGreenValue(), line_color.getBlueValue()};
    fileData["config_data"]["poly_data"]["points_color"] = {points_color.getRedValue(), points_color.getGreenValue(), points_color.getBlueValue()};

    /* Configurar las opciones que se usan en el programa */
    fileData["config_data"]["information"]["can_show_points"] = csp;
    fileData["config_data"]["information"]["can_trace_lines"] = ctl;
    fileData["config_data"]["information"]["can_connect"] = cc;
    fileData["config_data"]["information"]["can_triangulate"] = ct;
    fileData["config_data"]["information"]["show_points_quantity"] = spq;
    fileData["config_data"]["information"]["show_triangles_quantity"] = stq;
    fileData["config_data"]["information"]["show_mouse_coordinates"] = smc;

    /* Guardar el archivo */
    save_json("poligamia.json", fileData);
}

/* Funcion para mostrar la informacion del programa */
void info_window() {
    system("cls");

    cout<<"Poligamia\n\n";
    cout<<"Programa hecho en C++ 17\n";
    cout<<"Desarolladores: \n\n- Diego Isai Melara Flores\n- Kamila Dasilma Valle Castillo\n- Kimberlys Flores Lourdes Rostran\n\n";
    cout<<"Orientado a: Geometria Computacional\n";
    cout<<"Docente: Rhene Gonzalo Hernandez Berroteran\n";
    cout<<"Notas: \n\nEl programa puede contener algunos errores visuales o en la triangulacion, ya que el algoritmo utilizado fue el de earClipping, un algoritmo que intenta buscar 'orejas' para triangular, en ciertos casos puede simplemente no funcionar, por favor comprenda que no todows los algoritmos son perfectos, ademas, este es el mas rapido y eficiente cuando se habla de la memoria\n\n";

    system("pause");
    system("cls");
}

/* Funcion para mostrar los datos de la figura */
void printFigureData() {
    /* Iniciar ventana grafica de informacion */
    Point size(default_win_size.getX(), default_win_size.getY() + 55);
    win.modifyValues(size, PROGRAM_TITLE_FIGURE);
    win.initWindow();

    /* Obtener los datos */
    vector<Point> pointData = figurits.getPoints();
    vector<Triangle> triangData = triangulateEarClipping(pointData);
    bool trian = triangData.size() == 0 ? false : true;
    Point selectionLimit[] = {Point(10, 10), Point(default_win_size.getX() - 10, default_win_size.getY() - 10)}; // Para el borde

    /* Calcular el area y el perimetro de la figura */
    double area = getPolyArea(triangData);
    double perimetro = getPolyPerimetro(triangData);

    /* Bucle simple para el programa */
    while (true)
    {
        /* Dibujar la figura */
        Draw::setColor(default_line_color);
        Draw::drawPoly(pointData, true);

        /* Dibujar contorno decorativo de la ventana (RGB / CELESTE) */
        RGBColor rgb_values = Draw::generateRGB(3.9);
        default_border_color.itsNULL() ? Draw::setColor(rgb_values) : Draw::setColor(default_border_color); // Establecer el color del borde
        Draw::rectangle(selectionLimit[0].getX(), selectionLimit[0].getY(), selectionLimit[1].getX(), selectionLimit[1].getY());

        /* Dibujar el texto */
        DrawingText::configureMargin(LEFT_TEXT, RIGHT_TEXT);
        DrawingText::drawText(20, default_win_size.getY() + 5, String::format("Puntos: %d", pointData.size()).toArray());
        DrawingText::drawText(20, default_win_size.getY() + 30, String::format("Triangulable: %s", String::toString(trian).toArray()).toArray());
        
        if (trian) {
            DrawingText::configureMargin(RIGHT_TEXT, RIGHT_TEXT);
            DrawingText::drawText(default_win_size.getX() - 20, default_win_size.getY() + 5, String::format("Area de la figura: %.2f", area).toArray());
            DrawingText::drawText(default_win_size.getX() - 20, default_win_size.getY() + 30, String::format("Perimetro de la figura: %.2f", perimetro).toArray());
        }

        /* Obtener las teclas */
        if (kbhit())
        {
            int key = getch(); // Obtener la tecla presionada

            /* Verificar si se presiona escape para salir */
            if (key == KEYBOARD_KEY_ESCAPE)
            {
                break;
            }
        }
    }

    /* Cerrar la ventana grafica para finalizar */
    win.closeWindow();
    system("cls"); // Limpiar pantalla
}

/* Funcion para cargar un archivo json */
ordered_json load_json(const char *file) {
    try
    {
        ifstream input(file);
        ordered_json data;

        if (input.is_open())
        {
            input>>data;
            input.close();
        }

        return data;
    }
    catch(...)
    {
        return ordered_json();
    }
}

/* Funcion para guardar un archivo json */
void save_json(const char *file, ordered_json content) {
    ofstream output(file);

    if (output.is_open())
    {
        output<<content.dump(4);
        output.close();
    }
}

/* Funcion para obtener los datos de la figura */
Figure getFigureFromFile(const char *file) {
    ordered_json fileData = load_json(file);
    Figure figura;

    if (!fileData.empty())
    {
        /* Intentar almacenar los datos de los puntos */
        try
        {
            /* Obtener las coordenadas de los puntos */
            vector<Point> data;
            int len = fileData["figure_data"]["points"]["len"];

            for (size_t i = 0; i < len; i++)
            {
                int x = fileData["figure_data"]["points"]["x"][i];
                int y = fileData["figure_data"]["points"]["y"][i];
                data.push_back(Point(x, y));
            }

            /* Obtener el nombre de la figura y guardar en el obejto */
            string name = fileData["figure_data"]["name"];
            figura = Figure(data, name.data());
        }
        catch(...) {}
    }

    return figura;
}

/* Funcion para guardar los datos de la figura */
void saveFigureFromFile(const char* file, Figure figura) {
    ordered_json fileData;

    /* Guardar todos los datos de la figura */
    vector<Point> pointData = figura.getPoints();

    fileData["format_version"] = "2.0.0";
    fileData["figure_data"]["name"] = figura.getName();
    fileData["figure_data"]["points"]["len"] = pointData.size();

    /* Guardar uno por uno todos los valores de x, y */
    for (size_t i = 0; i < pointData.size(); i++)
    {
        fileData["figure_data"]["points"]["x"][i] = pointData[i].getX();
        fileData["figure_data"]["points"]["y"][i] = pointData[i].getY();
    }

    /* Guardar el archivo json */
    save_json(file, fileData);
    figura.free();
}

/* Funcion para triangular la figura */
vector<Triangle> triangulateEarClipping(vector<Point>& polygon) {
    vector<Triangle> triangles;

    // Un polígono debe tener al menos 3 vértices
    if (polygon.size() < 3) {
        return triangles;
    }

    // Clonar el polígono para no modificar el original
    vector<Point> polyClone = polygon;

    while (polyClone.size() > 3) {
        bool earFound = false;
        for (size_t i = 0; i < polyClone.size(); ++i) {
            if (isEar(polyClone, i)) {
                size_t prev = (i == 0) ? polyClone.size() - 1 : i - 1;
                size_t next = (i == polyClone.size() - 1) ? 0 : i + 1;

                // Crear un triángulo con los puntos prev, i y next
                triangles.push_back(Triangle(polyClone[prev], polyClone[i], polyClone[next]));

                // Eliminar el vértice de la oreja del polígono
                polyClone.erase(polyClone.begin() + i);

                earFound = true;
                break;
            }
        }

        if (!earFound) {
            // Si no se encuentra ninguna oreja, el polígono es inválido
            return vector<Triangle>(); // Devolver vector vacío
        }
    }

    // Añadir el triángulo final
    triangles.push_back(Triangle(polyClone[0], polyClone[1], polyClone[2]));

    return triangles;
}

/* Determina si un punto esta dentro de un triangulo */
bool isPointInTriangle(const Point& p, const Point& a, const Point& b, const Point& c) {
    float alpha = ((float)(b.getY() - c.getY()) * (p.getX() - c.getX()) + (float)(c.getX() - b.getX()) * (p.getY() - c.getY())) /
                  ((float)(b.getY() - c.getY()) * (a.getX() - c.getX()) + (float)(c.getX() - b.getX()) * (a.getY() - c.getY()));
    float beta = ((float)(c.getY() - a.getY()) * (p.getX() - c.getX()) + (float)(a.getX() - c.getX()) * (p.getY() - c.getY())) /
                 ((float)(b.getY() - c.getY()) * (a.getX() - c.getX()) + (float)(c.getX() - b.getX()) * (a.getY() - c.getY()));
    float gamma = 1.0f - alpha - beta;
    return alpha > 0 && beta > 0 && gamma > 0;
}

/* Determina si tres puntos estan en orden antihorario */
bool isCCW(const Point& a, const Point& b, const Point& c) {
    return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (b.getY() - a.getY()) * (c.getX() - a.getX()) > 0;
}

/* Determina si un vertice es una "oreja" */
bool isEar(const vector<Point>& polygon, size_t i) {
    size_t prev = (i == 0) ? polygon.size() - 1 : i - 1;
    size_t next = (i == polygon.size() - 1) ? 0 : i + 1;

    if (!isCCW(polygon[prev], polygon[i], polygon[next])) {
        return false;
    }

    for (size_t j = 0; j < polygon.size(); ++j) {
        if (j != prev && j != i && j != next && isPointInTriangle(polygon[j], polygon[prev], polygon[i], polygon[next])) {
            return false;
        }
    }

    return true;
}

/* Funcion para obtener la distancia entre dos puntos */
double distancia(const Point &a, const Point &b)
{
    /* Extraer los puntos para la formula */
    int x1 = a.getX(); int y1 = a.getY();
    int x2 = b.getX(); int y2 = b.getY();

    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/* Funcion para obtener el area del poligono */
double getPolyArea(const vector<Triangle> &triangles)
{
    /* Calcular el area dependiendo la cantidad de triangulos */
    double maxArea = 0.0;

    for (size_t i = 0; i < triangles.size(); i++)
    {
        /* Separar las coordenadass para obtener la formula */
        int x1 = triangles[i].getPointA().getX(); int y1 = triangles[i].getPointA().getY();
        int x2 = triangles[i].getPointB().getX(); int y2 = triangles[i].getPointB().getY();
        int x3 = triangles[i].getPointC().getX(); int y3 = triangles[i].getPointC().getY();

        maxArea += 0.5 * abs(x1 * (y2 - y3) + x2 * (y3- y1) + x3 * (y1 - y2));
    }
    
    return maxArea;
}

/* Funcion para obtener el perimetro del poligono */
double getPolyPerimetro(const vector<Triangle> &triangles)
{
    /* Calcular el perimetro dependiendo la cantidad de triangulos */
    double maxPerimetro = 0.0;
    
    for (size_t i = 0; i < triangles.size(); i++)
    {
        /* Separar las puntos para obtener la formula */
        Point ladoA = triangles[i].getPointA();
        Point ladoB = triangles[i].getPointA();
        Point ladoC = triangles[i].getPointC();

        /* Obtener todos los lados */
        double ld1 = distancia(ladoA, ladoB);
        double ld2 = distancia(ladoB, ladoC);
        double ld3 = distancia(ladoC, ladoA);

        maxPerimetro += ld1 + ld2 + ld3;
    }
    
    return maxPerimetro;
}

/*######## Funciones extras de dibujo ########*/
void center_filled_rectangle(int cX, int cY, int radX, int radY) {
    Draw::fillRectangle(cX - radX, cY - radY, cX + radX, cY + radY);
}

/*######## Funciones de validacion ########*/
/* Funcion para saber si existe un valor en una cadena */
bool exists(char val, size_t size, char data[]) {
    for (size_t i = 0; i < size; i++)
    {
        if (val == data[i])
        {
            return true;
        }
    }
    
    return false;
}

/* Funcion para validar si es un entero valido */
bool validInt(const String &value) {
    char numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (size_t i = 0; i < value.lenght(); i++)
    {
        if (!exists(value.getCharAt(i), 10, numbers))
        {
            return false;
        }
    }

    return true;
}