#ifndef __cplusplus
#error Esta libreria fue diseneada exclusivamente para C++
#endif // __cplusplus

#ifndef DSYGRAPH_HPP
#define DSYGRAPH_HPP

#define M_PI 3.14159265358979323846

/* Librerias base */
#include <chrono>
#include <cmath>
#include <vector>
#include <graphics.h>
#include <windows.h>

using namespace std;

// Espacio de nombre dsy
namespace dsy {
    /* Variables globales */
    int __drawtext_color = 15;
    int __draw_color;
    int __draw_backColor;
    int __draw_fillColor = 15;
    int __draw_fillStyle = SOLID_FILL;

    /* Modos de limpieza de la ventana */
    typedef enum {
        WINDOW_CLEAR_TOP,
        WINDOW_CLEAR_LEFT,
        WINDOW_CLEAR_RIGHT,
        WINDOW_CLEAR_BOTTOM,
        WINDOW_CLEAR_FULL
    } WindowClearMode;

    /* Orientacion del texto */
    typedef enum {
        VERTICAL_TEXT,
        HORIZONTAL_TEXT
    } TextOrientation;

    /* Justificadores de texto horizontal */
    typedef enum {
        HORI_LEFT_TEXT,
        HORI_CENTER_TEXT,
        HORI_RIGHT_TEXT
    } TextJustifyHorizontal;

    /* Justificadores de texto vertical */
    typedef enum {
        VERT_BOTTOM_TEXT,
        VERT_CENTER_TEXT,
        VERT_TOP_TEXT
    } TextJustifyVertical;

    /* Fuentes de texto disponibles */
    typedef enum {
        DEFAULT_FONT,
        TRIPLEX_FONT,
        SMALL_FONT,
        SANS_SERIF_FONT,
        GOTHIC_FONT,
        SCRIPT_FONT,
        SIMPLEX_FONT,
        TRIPLEX_SCR_FONT,
        COMPLEX_FONT,
        EUROPEAN_FONT,
        BOLD_FONT
    } TextFontsNames;

    /* Codigo del mouse */
    enum MouseKeys {
        MOUSE_KEY_LEFT = WM_LBUTTONDOWN,
        MOUSE_KEY_MIDDLE = WM_MBUTTONDOWN,
        MOUSE_KEY_RIGHT = WM_RBUTTONDOWN
    };

    /* Codigo de las teclas */
    enum KeyboardKeys {
        KEYBOARD_KEY_BACKSPACE = 8,            /* Retroceso */
        KEYBOARD_KEY_TAB = 9,                  /* Tabulacion */
        KEYBOARD_KEY_ENTER = 13,               /* Entrar */
        KEYBOARD_KEY_ESCAPE = 27,              /* Escape */
        KEYBOARD_KEY_SPACE = 32,               /* Espacio */
        KEYBOARD_KEY_EXCLAMATION_MARK = 33,    /* '!' */
        KEYBOARD_KEY_DOUBLE_QUOTE = 34,        /* '"' */
        KEYBOARD_KEY_HASH = 35,                /* '#' */
        KEYBOARD_KEY_DOLLAR_SIGN = 36,         /* '$' */
        KEYBOARD_KEY_PERCENT = 37,             /* '%' */
        KEYBOARD_KEY_AMPERSAND = 38,           /* '&' */
        KEYBOARD_KEY_SINGLE_QUOTE = 39,        /* ''' */
        KEYBOARD_KEY_LEFT_PARENTHESIS = 40,    /* '(' */
        KEYBOARD_KEY_RIGHT_PARENTHESIS = 41,   /* ')' */
        KEYBOARD_KEY_ASTERISK = 42,            /* '*' */
        KEYBOARD_KEY_PLUS = 43,                /* '+' */
        KEYBOARD_KEY_COMMA = 44,               /* ',' */
        KEYBOARD_KEY_MINUS = 45,               /* '-' */
        KEYBOARD_KEY_PERIOD = 46,              /* '.' */
        KEYBOARD_KEY_SLASH = 47,               /* '/' */
        KEYBOARD_KEY_0 = 48,                   /* '0' */
        KEYBOARD_KEY_1 = 49,                   /* '1' */
        KEYBOARD_KEY_2 = 50,                   /* '2' */
        KEYBOARD_KEY_3 = 51,                   /* '3' */
        KEYBOARD_KEY_4 = 52,                   /* '4' */
        KEYBOARD_KEY_5 = 53,                   /* '5' */
        KEYBOARD_KEY_6 = 54,                   /* '6' */
        KEYBOARD_KEY_7 = 55,                   /* '7' */
        KEYBOARD_KEY_8 = 56,                   /* '8' */
        KEYBOARD_KEY_9 = 57,                   /* '9' */
        KEYBOARD_KEY_COLON = 58,               /* ':' */
        KEYBOARD_KEY_SEMICOLON = 59,           /* ';' */
        KEYBOARD_KEY_LESS_THAN = 60,           /* '<' */
        KEYBOARD_KEY_EQUAL = 61,               /* '=' */
        KEYBOARD_KEY_GREATER_THAN = 62,        /* '>' */
        KEYBOARD_KEY_QUESTION_MARK = 63,       /* '?' */
        KEYBOARD_KEY_AT = 64,                  /* '@' */
        
        /* Teclas alfabéticas */
        KEYBOARD_KEY_A = 97,                   /* 'A' */
        KEYBOARD_KEY_B = 98,                   /* 'B' */
        KEYBOARD_KEY_C = 99,                   /* 'C' */
        KEYBOARD_KEY_D = 100,                   /* 'D' */
        KEYBOARD_KEY_E = 101,                   /* 'E' */
        KEYBOARD_KEY_F = 102,                   /* 'F' */
        KEYBOARD_KEY_G = 103,                   /* 'G' */
        KEYBOARD_KEY_H = 104,                   /* 'H' */
        KEYBOARD_KEY_I = 105,                   /* 'I' */
        KEYBOARD_KEY_J = 106,                   /* 'J' */
        KEYBOARD_KEY_K = 107,                   /* 'K' */
        KEYBOARD_KEY_L = 108,                   /* 'L' */
        KEYBOARD_KEY_M = 109,                   /* 'M' */
        KEYBOARD_KEY_N = 110,                   /* 'N' */
        KEYBOARD_KEY_O = 111,                   /* 'O' */
        KEYBOARD_KEY_P = 112,                   /* 'P' */
        KEYBOARD_KEY_Q = 113,                   /* 'Q' */
        KEYBOARD_KEY_R = 114,                   /* 'R' */
        KEYBOARD_KEY_S = 115,                   /* 'S' */
        KEYBOARD_KEY_T = 116,                   /* 'T' */
        KEYBOARD_KEY_U = 117,                   /* 'U' */
        KEYBOARD_KEY_V = 118,                   /* 'V' */
        KEYBOARD_KEY_W = 119,                   /* 'W' */
        KEYBOARD_KEY_X = 120,                   /* 'X' */
        KEYBOARD_KEY_Y = 121,                   /* 'Y' */
        KEYBOARD_KEY_Z = 122,                   /* 'Z' */
        KEYBOARD_KEY_LEFT_BRACKET = 91,        /* '[' */
        KEYBOARD_KEY_BACKSLASH = 92,           /* '\' */
        KEYBOARD_KEY_RIGHT_BRACKET = 93,       /* ']' */
        KEYBOARD_KEY_CARET = 94,               /* '^' */
        KEYBOARD_KEY_UNDERSCORE = 95,          /* '_' */
        KEYBOARD_KEY_GRAVE_ACCENT = 96,        /* '`' */
        
        /* Caracteres de control */
        KEYBOARD_KEY_DELETE = 127,             /* Tecla de suprimir */
        
        /* Teclas de función */
        KEYBOARD_KEY_F1 = 133,
        KEYBOARD_KEY_F2 = 134,
        KEYBOARD_KEY_F3 = 135,
        KEYBOARD_KEY_F4 = 136,
        KEYBOARD_KEY_F5 = 137,
        KEYBOARD_KEY_F6 = 138,
        KEYBOARD_KEY_F7 = 139,
        KEYBOARD_KEY_F8 = 140,
        KEYBOARD_KEY_F9 = 141,
        KEYBOARD_KEY_F10 = 142,
        KEYBOARD_KEY_F11 = 143,
        KEYBOARD_KEY_F12 = 144,
        
        /* Teclas de flecha */
        KEYBOARD_KEY_ARROW_UP = 72,
        KEYBOARD_KEY_ARROW_DOWN = 80,
        KEYBOARD_KEY_ARROW_LEFT = 75,
        KEYBOARD_KEY_ARROW_RIGHT = 77
    };

    /* Usar estructuras de graphics.h */
    struct fillsettingstype __GRAPHICS__FILL_STYLE;

    /*############ PROTOTIPOS DE CLASES ############*/
    // Clase para manejar los colores RGB
    class RGBColor {
        private:
        bool nullrgb;
        byte red, green, blue;

        public:
        RGBColor();
        RGBColor(int color);
        RGBColor(byte r, byte g, byte b);
        static int getRGB(byte r, byte g, byte b);
        int getRGB() const;
        byte getRedValue() const;
        byte getGreenValue() const;
        byte getBlueValue() const;
        void setRGB(int color);
        void setRGB(byte r, byte g, byte b);
        bool itsNULL() const;
    };

    // Clase para manejar puntos (x, y)
    class Point {
        private:
        int x, y;
        
        public:
        Point(); // Constructor vacio
        Point(int px, int py);
        int getX() const;
        int getY() const;
        void setX(int px);
        void setY(int py);
    };

    // Clase Triangulo
    class Triangle {
        private:
        Point a, b, c;

        public:
        Triangle();
        Triangle(Point a, Point b, Point c);
        Point getPointA() const;
        Point getPointB() const;
        Point getPointC() const;
        void setPointA(Point a);
        void setPointB(Point b);
        void setPointC(Point c);
    };

    // Clase Rect para manejar limites, cajas
    class Rect {
        private:
        Point a, b, c, d;

        public:
        Rect();
        Rect(int x1, int y1, int x2, int y2);
        Rect(Point limitI, Point limitF);
        void setLimits(int x1, int y1, int x2, int y2);
        void setLimits(Point limitI, Point limitF);
        Point getPointA() const;
        Point getPointB() const;
        Point getPointC() const;
        Point getPointD() const;
        void setPointA(Point a);
        void setPointB(Point b);
        void setPointC(Point c);
        void setPointD(Point d);
    };

    // Clase Figure para manejar figuras (Base para HitBox [en un futuro])
    class Figure {
        private:
        vector<Point> pointData;
        string name;
        bool isEmpty;

        public:
        Figure();
        Figure(vector<Point> pointData, const char* name);
        vector<Point> getPoints() const;
        string getName() const;
        bool empty() const;
        void free();
    };

    // Clase Ventana
    class Window {
        private:
        /* Variables privadas */
        bool initialized;
        int sizeX, sizeY;
        string title;

        public:
        Window(); // Declaracion del constructor (createWindow();)
        Window(int width, int height, const char *title = "Window"); // Declaración del constructor (createWindow();)
        Window(Point size, const char *title = "Window"); // Declaración del constructor (createWindow();)
        void modifyValues(int width, int height, const char *title = "Window");
        void modifyValues(Point size, const char *title = "Window");
        void initWindow();
        void clearWindow(bool clearAll = false);
        void clearWindowZone(Point limitI, Point limitF, RGBColor color, bool inverted = false, WindowClearMode cleanMode = WINDOW_CLEAR_FULL);
        void clearWindowZone(Rect limits, RGBColor color, bool inverted = false, WindowClearMode cleanMode = WINDOW_CLEAR_FULL);
        void closeWindow();
        bool hasWindowInitialized() const;
        int getWindowCodeError() const;
        char* getWindowError(int code) const;
        Point getWindowSize() const;
        int getWindowSizeX() const;
        int getWindowSizeY() const;
        int getWindowMaxColor() const;
        void ticks(int tick);
        ~Window(); // Declaración del destructor
    };

    // Clase de dibujo
    class Draw {
        public:
        /* Funciones controladoras de estilos y colores de dibujado (base)*/
        static void setBkColor(int col);
        static void setColor(int col);
        static void setFillColor(int col);
        static void setFillStyle(int type);
        static void setFillStyle(int type, int col);

        /* Funciones controladoras de estilos y colores de dibujado (RGB)*/
        static void setBkColor(RGBColor color);
        static void setColor(RGBColor color);
        static void setFillColor(RGBColor color);
        static void setFillStyle(int type, RGBColor color);

        /* Funciones para obtener los colores */
        static int getBkColor();
        static int getColor();
        static int getFillColor();
        static int getFillStyle();
        static RGBColor generateRGB(double speed = 1.0);
        
        /* Funciones de dibujos primitivos*/
        static void line(int x1, int y1, int x2, int y2);
        static void circle(int cX, int cY, int radius);
        static void bar(int x1, int y1, int x2, int y2);
        static void bar3D(int x1, int y1, int x2, int y2, int mX, int mY);
        static void rectangle(int xI, int yI, int xF, int yF);
        static void rectangle(Rect rect);
        static void arc(int cX, int cY, int angI, int angF, int radius);
        static void putPixel(int x, int y, int color);
        static int getPixel(int x, int y);
        static void arclipse(int cX, int cY, int angI, int angF, int radX, int radY);
        static void ellipse(int cX, int cY, int radX, int radY);
        static void drawPoly(int size, int *array, bool autoconnect = false);
        static void fillCircle(int cX, int cY, int radius);
        static void fillRectangle(int xI, int yI, int xF, int yF);
        static void fillRectangle(Rect rect);
        static void fillEllipse(int cX, int cY, int radX, int radY);
        static void fillPoly(int size, int *array);
        
        /* Funciones que usan BasicArray */
        static void drawPoly(const vector<Point> &value, bool autoconnect = false);
        static void drawPoly(const vector<Triangle> &value, bool autoconnect = false);
        static void fillPoly(const vector<Point> &value);
        static void fillPoly(const vector<Triangle> &value);
    };

    // Clase para controlar los dibujos de texto
    class DrawingText {
        public:
        static void drawText(int x, int y, const char* value);
        static void configureFont(TextFontsNames font, TextOrientation orientation, int size);
        static void configureColor(int col);
        static void configureMargin(TextJustifyHorizontal horizontal, TextJustifyVertical vertical);
    };

    // Clase para manejar Imagenes
    class Image {
        public:
        static void load_image(const char* file, int startX, int startY, int endX, int endY);
        static void load_image(const char* file, Point start, Point end);
        static void load_image(const char* file, Rect limits);
        static void save_image(const char* fileName, int startX, int startY, int endX, int endY);
        static void save_image(const char* fileName, Point start, Point end);
        static void save_image(const char* fileName, Rect limits);
    };

    // Clase para controlar el mouse
    class Mouse {
        public:
        static bool mouseKeyClicked(MouseKeys key);
        static Point getMousePos();
    };

    // Clase para dibujar una grid en una determinada posicion
    class Grid {
        public:
        static void drawGrid(int startX, int startY, int endX, int endY, RGBColor color = RGBColor(68, 68, 68), double spacing = 5.05, bool showX = true, bool showY = true);
        static void drawGrid(Point start, Point end, RGBColor color = RGBColor(68, 68, 68), double spacing = 5.05, bool showX = true, bool showY = true);
        static void drawGrid(Rect limits, RGBColor color = RGBColor(68, 68, 68), double spacing = 5.05, bool showX = true, bool showY = true);
    };

    /* ######## CLASE RGBCOLOR ######## */
    /* Metodo constructor vacio y definido */
    RGBColor::RGBColor() : red(0), green(0), blue(0), nullrgb(true) {}
    RGBColor::RGBColor(int color) : red(RED_VALUE(color)), green(GREEN_VALUE(color)), blue(BLUE_VALUE(color)) {}
    RGBColor::RGBColor(byte r, byte g, byte b) : red(r), green(g), blue(b), nullrgb(false) {}

    /* Metodo para obtener el entero de una sentencia RGB */
    int RGBColor::getRGB(byte r, byte g, byte b) {
        return RGB(r, g, b);
    }

     /* Metodo para obtener el entero de una sentencia RGB definida */
    int RGBColor::getRGB() const {
        return RGB(red, green, blue);
    }

    /* Metodo para establecer los valores RGB internos */
    void RGBColor::setRGB(byte r, byte g, byte b) {
        this->red = r;
        this->green = g;
        this->blue = b;
        this->nullrgb = false;
    }

    /* Metodo para establecer los valores RGB internos */
    void RGBColor::setRGB(int color) {
        this->red = RED_VALUE(color);
        this->green = GREEN_VALUE(color);
        this->blue = BLUE_VALUE(color);
    }

    /* Metodo para obtener el color rojo */
    byte RGBColor::getRedValue() const {
        return red;
    }

    /* Metodo para obtener el color verde */
    byte RGBColor::getGreenValue() const{
        return green;
    }

    /* Metodo para obtener el color azul */
    byte RGBColor::getBlueValue() const {
        return blue;
    }

    /* Funcion para saber si el objeto es nulo */
    bool RGBColor::itsNULL() const {
        return nullrgb;
    }

    /* ######## CLASE POINT ######## */
    /* Metodo constructor vacio y definido */
    Point::Point() : x(0), y(0) {}
    Point::Point(int px, int py) : x(px), y(py) {}

    /* Metodo para obtener la coordenada x */
    int Point::getX() const {
        return x;
    }

    /* Metodo para obtener la coordenada y */
    int Point::getY() const {
        return y;
    }

    /* Metodo para establecer la coordenada x */
    void Point::setX(int px) {
        this->x = px;
    }

    /* Metodo para establecer la coordenada x */
    void Point::setY(int py) {
        this->y = py;
    }

    /* ######### CLASE TRIANGLE ######## */
    /* Metodos constructores */
    Triangle::Triangle() : a(Point(-1, -1)), b(Point(-1, -1)), c(Point(-1, -1)) {}
    Triangle::Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}

    /* Metodo para devolver el primer punto */
    Point Triangle::getPointA() const {
        return a;
    }

    /* Metodo para devolver el segundo punto */
    Point Triangle::getPointB() const {
        return b;
    }

    /* Metodo para devolver el tercer punto */
    Point Triangle::getPointC() const {
        return c;
    }

    /* Metodo para establecer el valor del punto A */
    void Triangle::setPointA(Point a) {
        this->a = a;
    }

    /* Metodo para establecer el valor del punto B */
    void Triangle::setPointB(Point b) {
        this->b = b;
    }

    /* Metodo para establecer el valor del punto C */
    void Triangle::setPointC(Point c) {
        this->c = c;
    }

    /* ######## CLASE RECT ######## */
    /* Metodos constructores */
    Rect::Rect() : a(Point(-1, -1)), b(Point(-1, -1)), c(Point(-1, -1)), d(Point(-1, -1)) {}
    Rect::Rect(int x1, int y1, int x2, int y2) : a(Point(x1, y1)), b(Point(x2, y1)), c(Point(x2, y2)), d(Point(x1, y2)) {}
    Rect::Rect(Point limitI, Point limitF) : a(limitI), b(Point(limitF.getX(), limitI.getY())), c(limitF), d(Point(limitI.getX(), limitF.getY())) {}
    
    /* Metodo para establecer el limite de la recta */
    void Rect::setLimits(int x1, int y1, int x2, int y2) {
        a = Point(x1, y1);
        b = Point(x2, y1);
        c = Point(x2, y2);
        d = Point(x1, y2);
    }

    /* Metodo para establecer el limite de la recta */
    void Rect::setLimits(Point limitI, Point limitF) {
        a = limitI;
        b = Point(limitF.getX(), limitI.getY());
        c = limitF;
        d = Point(limitI.getX(), limitF.getY());
    }
    
    /* Metodo para devolver el primer punto */
    Point Rect::getPointA() const {
        return a;
    }

    /* Metodo para devolver el segundo punto */
    Point Rect::getPointB() const {
        return b;
    }

    /* Metodo para devolver el tercer punto */
    Point Rect::getPointC() const {
        return c;
    }

    /* Metodo para devolver el tercer punto */
    Point Rect::getPointD() const {
        return d;
    }

    /* Metodo para establecer el valor del punto A */
    void Rect::setPointA(Point a) {
        this->a = a;
    }

    /* Metodo para establecer el valor del punto B */
    void Rect::setPointB(Point b) {
        this->b = b;
    }

    /* Metodo para establecer el valor del punto C */
    void Rect::setPointC(Point c) {
        this->c = c;
    }

    /* Metodo para establecer el valor del punto C */
    void Rect::setPointD(Point d) {
        this->d = d;
    }

    /* ######## CLASE FIGURE ######## */
    /* Metodos constructores */
    Figure::Figure() : pointData(vector<Point>()), name("Default"), isEmpty(true) {}
    Figure::Figure(vector<Point> pointData, const char* name) : pointData(pointData), name(name), isEmpty(false) {}

    /* Metodo para obtener los puntos de la figuera */
    vector<Point> Figure::getPoints() const {
        return pointData;
    }

    /* Metodo para obtener el nombre de la figura */
    string Figure::getName() const {
        return name;
    }

    /* Metodo para saber si la figura esta vacia */
    bool Figure::empty() const {
        return isEmpty;
    }

    /* Metodo para liberar la figura a los datos de fabrica */
    void Figure::free() {
        pointData = vector<Point>();
        name = "default";
        isEmpty = true;
    }

    /* ######### CLASE WINDOW ######## */
    /* Asignar funciones a los metodos constructores */
    Window::Window() : sizeX(640), sizeY(480), title("Window") {}
    Window::Window(int width, int height, const char *title) : sizeX(width), sizeY(height), title(title) {}
    Window::Window(Point size, const char *title) : sizeX(size.getX()), sizeY(size.getY()), title(title) {}

    /* Metodo para modificar los valores de la ventana de forma segura */
    void Window::modifyValues(int width, int height, const char *title) {
        sizeX = width;
        sizeY = height;
        this->title = string(title);
    }

    /* Metodo para modificar los valores de la ventana de forma segura */
    void Window::modifyValues(Point size, const char *title) {
        sizeX = size.getX();
        sizeY = size.getY();
        this->title = string(title);
    }

    /* Función para inicializar la ventana */
    void Window::initWindow() {
        initwindow(sizeX, sizeY, title.c_str(), -1, -1);
    }

    /* Funcion para limpiar la ventana */
    void Window::clearWindow(bool clearAll) {
        cleardevice();
        
        if (clearAll) {
            int black = RGB(0, 0, 0);
            int white = RGB(255, 255, 255);

            setbkcolor(black);
            setcolor(white);
            setfillstyle(SOLID_FILL, white);
        }
    }

    /* Funcion para limpiar una zona de la pantalla */
    void Window::clearWindowZone(Point limitI, Point limitF, RGBColor color, bool inverted, WindowClearMode cleanMode) {
        int ori_color = Draw::getColor();
        int ori_color2 = Draw::getFillColor();
        int ori_style = Draw::getFillStyle();

        /* Establecer el color a pintar */
        Draw::setColor(color);
        Draw::setFillStyle(SOLID_FILL, color);
        
        if (!inverted) {
            /* Limpiar la zona de la pantalla */
            Draw::fillRectangle(limitI.getX(), limitI.getY(), limitF.getX(), limitF.getY());
        }
        else {
            /* Obtener las dimensiones externas */
            int x = getWindowSizeX(); int y = getWindowSizeY();

            if (cleanMode == WINDOW_CLEAR_TOP || cleanMode == WINDOW_CLEAR_FULL) {
                Draw::fillPoly(vector<Point>({Point(0, 0), Point(x, 0), Point(x, limitI.getY()), Point(0, limitI.getY())}));
            }
            else if (cleanMode == WINDOW_CLEAR_LEFT || cleanMode == WINDOW_CLEAR_FULL) {
                Draw::fillPoly(vector<Point>({Point(0, 0), Point(limitI.getX(), 0), Point(limitI.getX(), y), Point(0, y)}));
            }
            else if (cleanMode == WINDOW_CLEAR_RIGHT || cleanMode == WINDOW_CLEAR_FULL) {
                Draw::fillPoly(vector<Point>({Point(limitF.getX(), 0), Point(x, 0), Point(x, y), Point(limitF.getX(), y)}));
            }
            else if (cleanMode == WINDOW_CLEAR_BOTTOM || cleanMode == WINDOW_CLEAR_FULL) {
                Draw::fillPoly(vector<Point>({Point(0, limitF.getY()), Point(x, limitF.getY()), Point(x, y), Point(0, y)}));
            }
        }

        /* Restaurar colores originales */
        Draw::setColor(ori_color);
        Draw::setFillStyle(ori_style, ori_color2);
    }

    /* Funcion para limpiar una zona de la pantalla */
    void Window::clearWindowZone(Rect limits, RGBColor color, bool inverted, WindowClearMode cleanMode) {
        clearWindowZone(limits.getPointA(), limits.getPointC(), color, inverted, cleanMode);
    }

    /* Funcion para cerrar la ventana */
    void Window::closeWindow() {
        this->initialized = false;
        closegraph(); /* Esto es todo lo que hace */
    }

    /* Funcion para saber si la ventana ya esta iniciada */
    bool Window::hasWindowInitialized() const {
        return initialized;
    }

    /* Funcion para obtener un codigo de error */
    int Window::getWindowCodeError() const {
        return graphresult();
    }

    /* Funcion para obtener un mensaje de error en los gráficos */
    char* Window::getWindowError(int code) const {
        return grapherrormsg(code);
    }

    /* Funcion para obtener el tamanio total de la pantalla */
    Point Window::getWindowSize() const {
        return Point(getmaxx(), getmaxy());
    }

    /* Funcion para obtener el ancho de la pantalla */
    int Window::getWindowSizeX() const {
        return getmaxx();
    }

    /* Funcion para obtener el alto de la pantalla */
    int Window::getWindowSizeY() const {
        return getmaxy();
    }

    /* Funcion para obtener el color maximo de la pantalla */
    int Window::getWindowMaxColor() const {
        return getmaxcolor() + 1;
    }

    /* Funcion para limitar los ticks de un bucle */
    void Window::ticks(int tick) {
        delay((int)(1000 / tick));
    }

    /* Metodo destructor */
    Window::~Window() {
        this->closeWindow();
    }

    /* ######### CLASE DRAW ######## */
    /* Funcion para establecer el color de fondo */
    void Draw::setBkColor(int col) {
        __draw_backColor = col;
        setbkcolor(col);
    }

    /* Funcion para establecer el color de dibujado */
    void Draw::setColor(int col) {
        __draw_color = col;
        setcolor(col);
    }

    /* Funcion para establecer el color de llenado de la pantalla */
    void Draw::setFillColor(int col) {
        __draw_fillColor = col;
        setfillstyle(__draw_fillStyle, col);
    }

    /* Funcion para establecer el tipo de llenado de la pantalla */
    void Draw::setFillStyle(int type) {
        __draw_fillStyle = type;
        setfillstyle(type, __draw_fillColor);
    }

    /* Funcion para establecer el tipo de llenado de la pantalla */
    void Draw::setFillStyle(int type, int col) {
        __draw_fillColor = col;
        __draw_fillStyle = type;
        setfillstyle(type, col);
    }

    /* Metodo para establecer el color de fondo */
    void Draw::setBkColor(RGBColor color) {
        setBkColor(color.getRGB());
    }

    /* Metodo para establecer el color de las lineas */
    void Draw::setColor(RGBColor color) {
        setColor(color.getRGB());
    }

    /* Metodo para establecer el color del llenado de las figuras */
    void Draw::setFillColor(RGBColor color) {
        setFillColor(color.getRGB());
    }

    /* Metodo para establecer el tipo y color del llenado de las figuras */
    void Draw::setFillStyle(int type, RGBColor color) {
        setFillStyle(type, color.getRGB());
    }

    /* Funcion para obtener el color de fondo */
    int Draw::getBkColor() {
        __draw_backColor = ::getbkcolor();
        return __draw_backColor;
    }

    /* Funcion para obtener el color de las lineas */
    int Draw::getColor() {
        __draw_color = ::getcolor();
        return __draw_color;
    }

    /* Funcion para obtener el color del relleno de figuras */
    int Draw::getFillColor() {
        ::getfillsettings(&__GRAPHICS__FILL_STYLE);
        __draw_fillColor = __GRAPHICS__FILL_STYLE.color;
        return __draw_fillColor;
    }

    /* Funcion para obtener el estilo del fondo */
    int Draw::getFillStyle() {
        getfillsettings(&__GRAPHICS__FILL_STYLE);
        __draw_fillStyle = __GRAPHICS__FILL_STYLE.pattern;
        return __draw_fillStyle;
    }

    /* Funcion para generar colores random RGB (Funcion hecha en Python hace tiempo transcrita a C++)*/
    RGBColor Draw::generateRGB(double speed) {
        /* Usar chrono para calcular el tiempo */
        using namespace std::chrono;

        auto now = system_clock::now();
        auto duration = now.time_since_epoch();
        double tm = duration_cast<milliseconds>(duration).count() / 1000.0;

        // Preparar los calculos
        double fr = 0.1 * speed;
        double fg = 0.05 * speed;
        double fb = 0.03 * speed;

        // Calcular los valores rgb
        int r = static_cast<int>(sin(2 * M_PI * fr * tm) * 127 + 128);
        int g = static_cast<int>(sin(2 * M_PI * fg * tm) * 127 + 128);
        int b = static_cast<int>(sin(2 * M_PI * fb * tm) * 127 + 128);

        return RGBColor(r, g, b);
    }

    /* ######### Funciones de dibujado ########## */
    /* Funcion para crear una linea */
    void Draw::line(int x1, int y1, int x2, int y2) {
        ::line(x1, y1, x2, y2);
    }

    /* Funcion para crear un circulo */
    void Draw::circle(int cX, int cY, int radius) {
        ::circle(cX, cY, radius);
    }

    /* Funcion para dibujar una barra */
    void Draw::bar(int x1, int y1, int x2, int y2) {
        ::bar(x1, y1, x2, y2);
    }

    /* Funcion para dibujar una barra 3D */
    void Draw::bar3D(int x1, int y1, int x2, int y2, int mX, int mY) {
        ::bar3d(x1, y1, x2, y2, mX, mY);
    }

    /* Funcion para crear un rectangulo */
    void Draw::rectangle(int xI, int yI, int xF, int yF) {
        ::rectangle(xI, yI, xF, yF);
    }

    /* Funcion para crear un rectangulo */
    void Draw::rectangle(Rect rect) {
        rectangle(rect.getPointA().getX(), rect.getPointA().getY(), rect.getPointC().getX(), rect.getPointC().getY());
    }

    /* Funcion para crear un arco */
    void Draw::arc(int cX, int cY, int angI, int angF, int radius) {
        ::arc(cX, cY, angI, angF, radius);
    }

    /* Funcion para poner un pixel */
    void Draw::putPixel(int x, int y, int color) {
        ::putpixel(x, y, color);
    }

    /* Funcion para obtener el color de un pixel */
    int Draw::getPixel(int x, int y) {
        return ::getpixel(x, y);
    }

    /* Funcion para crear un arco elipse */
    void Draw::arclipse(int cX, int cY, int angI, int angF, int radX, int radY) {
        ::ellipse(cX, cY, angI, angF, radX, radY);
    }

    /* Funcion para crear una elipse */
    void Draw::ellipse(int cX, int cY, int radX, int radY) {
        ::ellipse(cX, cY, 0, 360, radX, radY);
    }

    /* Funcion para crear un poligono */
    void Draw::drawPoly(int size, int *array, bool autoconnect) {
        ::drawpoly(size, array);

        if (autoconnect) {
            line(array[0], array[1], array[(size * 2) - 2], array[(size * 2) - 1]); /* Usando un poco de logica */
        }
    }

    /* Funcion para crear un circulo relleno */
    void Draw::fillCircle(int cX, int cY, int radius) {
        ::fillellipse(cX, cY, radius, radius);
    }

    /* Funcion para crear un rectangulo relleno */
    void Draw::fillRectangle(int xI, int yI, int xF, int yF) {
        int data[8];
        data[0] = xI; data[2] = xF; data[4] = xF; data[6] = xI;
        data[1] = yI; data[3] = yI; data[5] = yF; data[7] = yF;
        
        ::fillpoly(4, data);
    }

    /* Funcion para crear un rectangulo relleno */
    void Draw::fillRectangle(Rect rect) {
        fillRectangle(rect.getPointA().getX(), rect.getPointA().getY(), rect.getPointC().getX(), rect.getPointC().getY());
    }

    /* Funcion para crear una elipse rellena */
    void Draw::fillEllipse(int cX, int cY, int radX, int radY) {
        ::fillellipse(cX, cY, radX, radY);
    }

    /* Funcion para crear un poligono relleno */
    void Draw::fillPoly(int size, int *array) {
        ::fillpoly(size, array);
    }

    /* Funcion para dibujar un poligono usando BasicArray */
    void Draw::drawPoly(const vector<Point> &value, bool autoconnect) {
        int size = value.size() * 2;
        int *data = new int[size];
        
        int index = 0;
        for (int i = 0; i < size; i+=2) {
            Point temp = value[index];
            data[i] = temp.getX();
            data[i+1] = temp.getY();
            index++;
        }
        
        drawPoly((size / 2), data, autoconnect);
        delete[] data;
    }

    /* Funcion para dibujar un poligono usando BasicArray y Triangle */
    void Draw::drawPoly(const vector<Triangle> &value, bool autoconnect) {
        for (size_t i = 0; i < value.size(); i++)
        {
            Triangle temp = value[i];
            vector<Point> tmp({temp.getPointA(), temp.getPointB(), temp.getPointC()});

            drawPoly(tmp, autoconnect);
        }
    }

    /* Funcion para dibujar un poligono relleno usando BasicArray */
    void Draw::fillPoly(const vector<Point> &value) {
        int size = value.size() * 2;
        int *data = new int[size];
        
        int index = 0;
        for (int i = 0; i < size; i+=2) {
            Point temp = value[index];
            data[i] = temp.getX();
            data[i+1] = temp.getY();
            index++;
        }
        
        ::fillpoly((size / 2), data);
        delete[] data;
    }

    /* Funcion para dibujar un poligono relleno usando BasicArray y Triangle */
    void Draw::fillPoly(const vector<Triangle> &value) {
        for (size_t i = 0; i < value.size(); i++)
        {
            Triangle temp = value[i];
            vector<Point> tmp;
            tmp.push_back(temp.getPointA());
            tmp.push_back(temp.getPointB());
            tmp.push_back(temp.getPointC());

            fillPoly(tmp);
            tmp.clear();
        }
    }

    /* ######### CLASE DRAWTEXT ######## */
    /* Metodo para agregar un texto a generar */
    void DrawingText::drawText(int x, int y, const char* value) {
        // Guardar color original, coordenadas, y longitud de texto
        int ori_color = Draw::getColor();
        
        // Crear el texto
        Draw::setColor(__drawtext_color); // Usar color configurado
        outtextxy(x, y, (char*)value);
        Draw::setColor(ori_color); // Reponer original
    }

    /* Metodo para configurar el fondo del texto */
    void DrawingText::configureFont(TextFontsNames font, TextOrientation orientation, int size) {
        ::settextstyle(font, orientation, size);
    }

    /* Metodo para configurar el color del texto */
    void DrawingText::configureColor(int col) {
        __drawtext_color = col;
    }

    /* Metodo para configurar el margen del texto */
    void DrawingText::configureMargin(TextJustifyHorizontal horizontal, TextJustifyVertical vertical) {
        ::settextjustify(horizontal, vertical);
    }

    /* ######## CLASE IMAGE ########*/
    /* Metodo para cargar una imagen */
    void Image::load_image(const char* file, int startX, int startY, int endX, int endY) {
        readimagefile(file, startX, startY, endX, endY);
    }

    /* Metodo para cargar una imagen */
    void Image::load_image(const char *file, Point start, Point end) {
        load_image(file, start.getX(), start.getY(), end.getX(), end.getY());
    }

    /* Metodo para cargar una imagen */
    void Image::load_image(const char *file, Rect limits) {
        load_image(file, limits.getPointA(), limits.getPointC());
    }

    /* Metodo para guardar un espacio de la ventana en un archivo */
    void Image::save_image(const char *fileName, int startX, int startY, int endX, int endY) {
        writeimagefile(fileName, startX, startY, endX, endY);
    }

    /* Metodo para guardar un espacio de la ventana en un archivo */
    void Image::save_image(const char *fileName, Point start, Point end) {
        save_image(fileName, start.getX(), start.getY(), end.getX(), end.getY());
    }

    /* Metodo para guardar un espacio de la ventana en un archivo */
    void Image::save_image(const char *fileName, Rect limits) {
        save_image(fileName, limits.getPointA(), limits.getPointC());
    }

    /* ######### CLASE MOUSE ######## */
    /* Metodo para saber si un evento se esta ejecutando */
    bool Mouse::mouseKeyClicked(MouseKeys key) {
        bool result = ismouseclick(key);
        clearmouseclick(key); // Limpiar el click anterior
        return result;
    }

    /* Funcion para obtener la posición X del mouse */
    Point Mouse::getMousePos() {
        return Point(mousex(), mousey());
    }

    /* ######## CLASE GRID ######## */
    /* Metodo para dibujar la grid */
    void Grid::drawGrid(int startX, int startY, int endX, int endY, RGBColor color, double spacing, bool showX, bool showY) {
        // Guardar el color original
        int ori_color = Draw::getColor();

        // Establecer el color de dibujo
        spacing = abs(spacing) * 2.95;
        Draw::setColor(color);

        // Dibujar las líneas horizontales
        if (showX) {
            for (double i = startX; i <= endX; i += spacing) {
                int x1 = (int)i; int y1 = startY;
                int x2 = x1; int y2 = endY;

                Draw::line(x1, y1, x2, y2);
            }
        }

        // Dibujar las líneas verticales
        if (showY) {
            for (double i = startY; i <= endY; i += spacing) {
                int x1 = startX; int y1 = (int)i;
                int x2 = endX; int y2 = y1;

                Draw::line(x1, y1, x2, y2);
            }
        }

        // Restablecer el color de dibujo original
        Draw::setColor(ori_color);
    }

    /* Metodo para dibujar la grid */
    void Grid::drawGrid(Point start, Point end, RGBColor color, double spacing, bool showX, bool showY) {
        drawGrid(start.getX(), start.getY(), end.getX(), end.getY(), color, spacing, showX, showY);
    }

    /* Metodo para dibujar la grid */
    void Grid::drawGrid(Rect limits, RGBColor color, double spacing, bool showX, bool showY) {
        drawGrid(limits.getPointA(), limits.getPointC(), color, spacing, showX, showY);
    }
}

#endif // DSYGRAPH_HPP