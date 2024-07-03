/* Importar las librerias */
#include <iostream>
#include <vector>
#include <fstream>
#include <json.hpp>
#include "dsygraph.hpp"
#include "dsyutil.hpp"

#define PROGRAM_TITLE "Poligamia"
#define PROGRAM_TITLE_FIGURE "Poligamia Figure Viewer"

/* Usar el espacio de nombres std, dsy, json */
using namespace std;
using namespace dsy;
using ordered_json = nlohmann::ordered_json;

/* Variables globales */
Point default_win_size(640, 480);
RGBColor default_border_color(18, 226, 216); // Celeste
RGBColor default_points_color(255, 128, 0); // Naranja
RGBColor default_line_color(255, 255, 255); // Blanco
bool can_show_grid = false; // Boton G
bool can_connect = false; // Boton C
bool can_line_appear = true; // Boton L
bool can_show_points = true; // Boton P
bool can_triangulate = false; // Boton T
bool show_points_quantity = true;
bool show_triangles_quantity = true;
bool show_mouse_coordinates = true;
Window win(default_win_size, PROGRAM_TITLE);
Figure figurits;

/* Funciones principales */
void main_window(); // Funcion principal para dibujar
void load_config(); // Funcion para cargar la configuracion del programa
void save_config(bool console = true); // Funcion para guardar la configuracion del programa
void info_window(); // Funcion para mostrar la informacion del programa
void printFigureData(); // Funcion para mostrar los datos de la figura guardada

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
double getPolyArea(const vector<Point> &polygon);
double getPolyPerimetro(const vector<Point> &polygon);

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

                // Preparar el inicio de la ventana
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
    // Cambiar la configuracion de la ventana para obtener mayor espacio
    Point size(default_win_size.getX(), default_win_size.getY() + 30);
    win.modifyValues(size, PROGRAM_TITLE);

    // Limpiar todo el modo gráfico
    win.initWindow();
    win.clearWindow();

    /* Variables controladoras */
    int key = 0, curPoints = -1;
    int trianPTS = 0; // Para obtener las teclas presionadas
    Rect selectionLimit(10, 10, default_win_size.getX() - 10, default_win_size.getY() - 10); // Para el borde
    Rect gridLimits(11, 11, default_win_size.getX() - 11, default_win_size.getY() - 11); // Para el Grid

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
                if ((mousePos.getX() > selectionLimit.getPointA().getX() && mousePos.getY() > selectionLimit.getPointA().getY()) && (mousePos.getX() < selectionLimit.getPointC().getX() && mousePos.getY() < selectionLimit.getPointC().getY()))
                {
                    polygon.push_back(mousePos);
                    old = mousePos;

                    if (can_connect) {
                        win.clearWindowZone(Point(selectionLimit.getPointA().getX() + 1, selectionLimit.getPointA().getY() + 1), Point(selectionLimit.getPointC().getX() - 1, selectionLimit.getPointC().getY() - 1), RGBColor(0));
                    }
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
            /* Si se presiona, se controla la cuadricula */
            if (key == KEYBOARD_KEY_G) {
                can_show_grid = !can_show_grid;
            }

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
                    DrawingText::configureMargin(HORI_CENTER_TEXT, VERT_CENTER_TEXT);
                    DrawingText::drawText(win.getWindowSizeX() / 2, win.getWindowSizeY() / 2,"Guardando la figura...");
                    DrawingText::configureMargin(HORI_LEFT_TEXT, VERT_BOTTOM_TEXT);
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
                    win.clearWindowZone(selectionLimit.getPointA(), Point(selectionLimit.getPointC().getX(), selectionLimit.getPointC().getY() + 1), RGBColor(0), true, WINDOW_CLEAR_BOTTOM);
                }
            }

            // Limpiar centro de la pantalla
            win.clearWindowZone(Point(selectionLimit.getPointA().getX() + 1, selectionLimit.getPointA().getY() + 1), Point(selectionLimit.getPointC().getX() - 1, selectionLimit.getPointC().getY() - 1), RGBColor(0));
        }

        if (can_show_grid) {
            Grid::drawGrid(gridLimits);
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
                    win.clearWindowZone(Point(selectionLimit.getPointA().getX() + 1, selectionLimit.getPointA().getY() + 1), Point(selectionLimit.getPointC().getX() - 1, selectionLimit.getPointC().getY() - 1), RGBColor(0));
                    curPoints = polygon.size();
                    trian = triangulateEarClipping(polygon);
                }

                /* Dibujar si la longitud no es 0 */
                if (trian.size() > 0 && can_triangulate) {
                    Draw::setColor(default_line_color); // Establecer el color
                    Draw::drawPoly(trian, can_connect);
                    trianPTS = trian.size();
                }
            }
        }
        
        /* Dibujar contorno decorativo de la ventana (RGB / CELESTE) */
        RGBColor rgb_values = Draw::generateRGB(3.9);
        default_border_color.itsNULL() ? Draw::setColor(rgb_values) : Draw::setColor(default_border_color); // Establecer el color del borde
        Draw::rectangle(selectionLimit);

        /* Mostrar la cantidad de puntos si esta permitido */
        if (show_points_quantity)
        {
            DrawingText::configureMargin(HORI_LEFT_TEXT, VERT_CENTER_TEXT);
            DrawingText::drawText(20, size.getY() - 15, String::format("Puntos: %d", polygon.size()).toArray());
        }

        /* Mostrar la cantidad de triangulos si esta permitido */
        if (show_triangles_quantity)
        {
            DrawingText::configureMargin(HORI_RIGHT_TEXT, VERT_CENTER_TEXT);
            DrawingText::drawText(size.getX() - 20, size.getY() - 15, String::format("Triangulos: %d", trianPTS).toArray());
        }

        /* Mostrar las coordenas del mouse si esta permitido */
        if (show_mouse_coordinates)
        {
            DrawingText::configureMargin(HORI_CENTER_TEXT, VERT_CENTER_TEXT);
            DrawingText::drawText(size.getX() / 2, size.getY() - 15, String::format("Coordenadas del mouse: (%d, %d)", mousePos.getX(), mousePos.getY()).toArray());
        }

        /* Limpiar pantalla al mover el mouse */
        if (mousePos.getX() != oldMousePos.getX() || mousePos.getY() != oldMousePos.getY())
        {
            oldMousePos = mousePos;
            win.clearWindowZone(selectionLimit.getPointA(), Point(selectionLimit.getPointC().getX(), selectionLimit.getPointC().getY() + 1), RGBColor(0), true, WINDOW_CLEAR_BOTTOM);
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
    bool csp, csg, ctl, cc, ct, spq, stq, smc;

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
        csg = fileData["config_data"]["information"]["can_show_grid"];
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
        can_show_grid = csg;
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
    bool csp, csg, ctl, cc, ct, spq, stq, smc;
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
        csg = fileData["config_data"]["information"]["can_show_grid"];
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
        csg = can_show_grid;
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
            cout<<"5: Mas opciones"<<endl;
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
                            cout<<String::format("Se ha preparado la configuracion de la ventana con estos nuevos datos: (%d, %d)\n", size_x, size_y);
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
                            cout<<String::format("Se ha preparado la configuracion de el color del borde con este nuevos dato: RGB\n");
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
                                    cout<<String::format("Se ha preparado la configuracion de el color del borde con estos nuevos datos: (%d, %d, %d)\n", red, green, blue);
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
                                cout<<String::format("Se ha preparado la configuracion de el color de los puntos con estos nuevos datos: (%d, %d, %d)\n", red, green, blue);
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
                                cout<<String::format("Se ha preparado la configuracion de el color de las lienas con estos nuevos datos: (%d, %d, %d)\n", red, green, blue);
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
                /* More options */
                else if (opcion.equals("5")) {
                    /* Preparar los datos y limpiar pantalla */
                    String input;
                    system("cls");

                    while (true)
                    {
                        cout<<"Configuracion Avanzada\n\n";
                        cout<<"Elija el numero de una opcion:\n";
                        cout<<String::format("1. Mostrar los puntos de la figura (%s)\n", csp ? "Activo" : "Inactivo");
                        cout<<String::format("2. Permitir mostrar la cuadricula (%s)\n", csg ? "Activo" : "Inactivo");
                        cout<<String::format("3. Mostrar las lineas de la figura (%s)\n", ctl ? "Activo" : "Inactivo");
                        cout<<String::format("4. Permitir conexion de las lineas (%s)\n", cc ? "Activo" : "Inactivo");
                        cout<<String::format("5. Mostrar la triangulacion de la figura (%s)\n", ct ? "Activo" : "Inactivo");
                        cout<<String::format("6. Mostrar la cantidad de puntos de la figura (%s)\n", spq ? "Activo" : "Inactivo");
                        cout<<String::format("7. Mostrar la cantidad de triangulos de la figura (%s)\n", stq ? "Activo" : "Inactivo");
                        cout<<String::format("8. Mostrar las coordenadas del mouse (%s)\n", smc ? "Activo" : "Inactivo");
                        cout<<"Otro: Volver atras"<<endl;
                        cout<<">>> ";
                        cin>>input;

                        if (validInt(input))
                        {
                            if (input.equals("1"))
                            {
                                csp = !csp;
                            }
                            else if (input.equals("2"))
                            {
                                csg = !csg;
                            }
                            else if (input.equals("3"))
                            {
                                ctl = !ctl;
                            }
                            else if (input.equals("4"))
                            {
                                cc = !cc;
                            }
                            else if (input.equals("5"))
                            {
                                ct = !ct;
                            }
                            else if (input.equals("6"))
                            {
                                spq = !spq;
                            }
                            else if (input.equals("7"))
                            {
                                stq = !stq;
                            }
                            else if (input.equals("8"))
                            {
                                smc = !smc;
                            }
                            else {
                                cout<<"Preparando la configuracion para guardar...";
                                delay(750);
                                break;
                            }
                        }
                        else {
                            cout<<"No se ha ingresado un numero valido!\n";
                            system("pause");
                        }

                        system("cls");
                    }
                }
                else {
                    cout<<"Guardando los nuevos datos y regresando..."<<endl;
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
    fileData["config_data"]["information"]["can_show_grid"] = csg;
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

    /* Mostrar informacion base */
    cout<<"Poligamia\n\n";
    cout<<"Programa hecho en C++ 17\n";
    cout<<"Desarolladores: \n\n- Diego Isai Melara Flores\n- Kamila Dasilma Valle Castillo\n- Kimberlys Flores Lourdes Rostran\n\n";
    cout<<"Orientado a: Geometria Computacional\n";
    cout<<"Docente: Rhene Gonzalo Hernandez Berroteran\n";

    /* Mostrar informacion de los botones */
    cout<<"Teclas especiales: \n\n";
    cout<<"Boton ESC: Funciona para salir de la ventana sin afectar el programa completamente\n";
    cout<<"Boton DEL: Funciona para borrar el ultimo punto ingresado\n";
    cout<<"Boton G: Sirve para mostrar la cuadricula\n";
    cout<<"Boton P: Sirve para mostrar los puntos de la figura\n";
    cout<<"Boton L: Sirve para mostrar las lineas donde van los puntos de la figura\n";
    cout<<"Boton C: Sirve para conectar los puntos de la figura\n";
    cout<<"Boton T: Sirve para mostrar la triangulacion de la figura (Tener en cuenta que el algoritmo no siempre lo hara)\n";
    cout<<"Boton S: Sirve para guardar la figura actual, lo que sera util para obtener su area y perimetro (Hasta para guardar la imagen!!)\n\n";

    /* Mostrar comentarios extras */
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
    int trianPoints = 0;
    vector<Point> pointData = figurits.getPoints();
    vector<Triangle> triangData = triangulateEarClipping(pointData);
    bool triangulable = triangData.size() == 0 ? false : true;
    Rect selectionLimit(10, 10, default_win_size.getX() - 10, default_win_size.getY() - 10); // Para el borde
    Rect gridLimits(11, 11, default_win_size.getX() - 11, default_win_size.getY() - 11); // Para el Grid

    /* Calcular el area y el perimetro de la figura */
    double area = getPolyArea(pointData);
    double perimetro = getPolyPerimetro(pointData);

    /* Bucle simple para el programa */
    while (true)
    {
        /* Obtener las teclas */
        if (kbhit())
        {
            int key = getch(); // Obtener la tecla presionada

            /* Verificar si se presiona escape para salir */
            if (key == KEYBOARD_KEY_ESCAPE)
            {
                break;
            }

            /* Si se presiona, se controla si se puede mostrar la cuadricula */
            if (key == KEYBOARD_KEY_G) {
                can_show_grid = !can_show_grid;
            }

            /* Si se presiona, se controla la observacion de puntos */
            if (key == KEYBOARD_KEY_P) {
                can_show_points = !can_show_points;
            }

            /* Si se presiona, se controla la generacion de triangulacion */
            if (key == KEYBOARD_KEY_T) {
                trianPoints = 0;
                can_triangulate = !can_triangulate;
            }

            /* Si se presiona, se guardara la figura como si fuese una imagen */
            if (key == KEYBOARD_KEY_S)
            {
                Image::save_image(NULL, selectionLimit); /* Guardar la imagen */
                win.clearWindow();
                DrawingText::configureMargin(HORI_CENTER_TEXT, VERT_CENTER_TEXT);
                DrawingText::drawText(win.getWindowSizeX() / 2, win.getWindowSizeY() / 2,"Generando una imagen...");
                DrawingText::configureMargin(HORI_LEFT_TEXT, VERT_BOTTOM_TEXT);
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

            win.clearWindow();
        }

        if (can_show_grid) {
            Grid::drawGrid(gridLimits);
        }

        /* Dibujar la figura */
        Draw::setColor(default_line_color);
        Draw::drawPoly(pointData, true);

        /* Dibujar puntos si lo permite el usuario */
        if (can_show_points) {
            /* Dibujar círculos para simular puntos */
            Draw::setColor(default_points_color); // Establecer el color
            for (size_t i = 0; i < pointData.size(); i++) {
                Point tmp = pointData[i];
                Draw::circle(tmp.getX(), tmp.getY(), 2);
            }
        }

        /* Triangular la figura si lo permite el usuario */
        if (triangulable && can_triangulate) {
            trianPoints = triangData.size();
            Draw::setColor(default_line_color); // Establecer el color
            Draw::drawPoly(triangData, can_connect);
        }

        /* Dibujar contorno decorativo de la ventana (RGB / CELESTE) */
        RGBColor rgb_values = Draw::generateRGB(3.9);
        default_border_color.itsNULL() ? Draw::setColor(rgb_values) : Draw::setColor(default_border_color); // Establecer el color del borde
        Draw::rectangle(selectionLimit);

        /* Dibujar el texto de la izquierda */
        DrawingText::configureMargin(HORI_LEFT_TEXT, VERT_TOP_TEXT);
        DrawingText::drawText(20, default_win_size.getY() + 5, String::format("Puntos: %d", pointData.size()).toArray());
        DrawingText::drawText(20, default_win_size.getY() + 30, String::format("Triangulos: %d", trianPoints).toArray());

        /* Dibujar el texto de la derecha */
        DrawingText::configureMargin(HORI_RIGHT_TEXT, VERT_TOP_TEXT);
        DrawingText::drawText(default_win_size.getX() - 20, default_win_size.getY() + 5, String::format("Area de la figura: %.2f U^2", area).toArray());
        DrawingText::drawText(default_win_size.getX() - 20, default_win_size.getY() + 30, String::format("Perimetro de la figura: %.2f U^2", perimetro).toArray());
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

/* Funcion para obtener el area del poligono */
double getPolyArea(const vector<Point> &polygon)
{
    /* Preparar los datos para realizar los calculos */
    double maxArea = 0.0;
    int n = polygon.size();

    if (n >= 3)
    {
        /* Iterar los puntos para obtener el calculo del area */
        for (size_t i = 0; i < n; ++i)
        {
            int j = (i + 1) % n;
            maxArea += polygon[i].getX() * polygon[j].getY();
            maxArea -= polygon[j].getX() * polygon[i].getY();
        }
        
        /* Dividir entre dos el resultado final */
        maxArea = Math::abs(maxArea) / 2.0;
    }
    
    return maxArea;
}

/* Funcion para obtener el perimetro del poligono */
double getPolyPerimetro(const vector<Point> &polygon)
{
    /* Preparar los datos para los calculos */
    double maxPerimetro = 0.0;
    int n = polygon.size();

    if (n >= 3)
    {
        /* Iterar los puntos para obtener el perimetro total */
        for (size_t i = 0; i < n; ++i)
        {
            int j = (i + 1) % n;
            double dX = polygon[j].getX() - polygon[i].getX();
            double dY = polygon[j].getY() - polygon[i].getY();

            maxPerimetro += Math::sqrt(dX * dX + dY * dY);
        }
    }
    
    return maxPerimetro;
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