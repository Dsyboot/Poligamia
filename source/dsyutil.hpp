#ifndef __cplusplus
#error Esta libreria fue diseneada exclusivamente para C++
#endif // __cplusplus

/*######## DEFINICION DE LA LIBRERIA ########*/
#ifndef DSYUTIL_HPP
#define DSYUTIL_HPP

/* Librerias a utilizar */
#include <cmath> // Para manejar matematicas
#include <string> // Manipulacion para los char*
#include <cctype> // Para manipular conversiones de tipos
#include <cstdarg> // Manipulacion de funciones con argumentos n

using namespace std;

// Espacio de nombre dsy
namespace dsy {
	// Clase matematica
	class Math {
		public:
		// Variables
		static const double E;
		static const double PI;
		
		// Metodos comunes de math.h
		static double abs(double num);
		static double acos(double x);
		static double asin(double x);
		static double atan(double x);
		static double atan2(double y, double x);
		static double ceil(double x);
		static double cos(double x);
		static double cosh(double x);
		static double exp(double x);
		static double floor(double x);
		static double log(double x);
		static double log(double x, int n);
		static double log10(double x);
		static int mod(int x, int y);
		static double mod(double x, double y);
		static double pow(double x, double y);
		static double sin(double x);
		static double sinh(double x);
		static double sqrt(double x);
		static double tan(double x);
		static double tanh(double x);
		
		// Metodos secundarios
		double hypot(double a, double b);
		static bool pair(int num);
		static int min(int num, int num2);
		static int max(int num, int num2);
		static double min(double num, double num2);
		static double max(double num, double num2);
	};

	// Clase String :0 (limite de 65535 de caracteres)
	class String {
		private:
		char* data;
		size_t len;
		
		public:
		// Metodos base del String
		String(const char* value = "");
		String(const String &value);
		size_t lenght() const;
		operator char*() const;
		String& operator = (const char* value);
		String& operator += (const char* value);
		String& operator += (const String &value);
		char getCharAt(size_t index) const;
		friend ostream& operator<<(ostream& os, const String& str);
		friend istream& operator>>(istream& is, String& str);
		
		// Metodos complejos del String
		char* toArray() const;
		static String format(const char* secuence, ...);
		bool equals(const char* value) const;
		bool equals(const String &value) const;
		bool contains(const char* value) const;
		bool contains(const String &value) const;
		bool startsWith(const char* value) const;
		bool startsWith(const String &value) const;
		bool endsWith(const char* value) const;
		bool endsWith(const String &value) const;
		static String toLowerCase(String &value);
		static String toUpperCase(String &value);
		
		// Metodos toString para casi todos los tipos de datos
		static String toString(int value); //%d
		static String toString(short value);
		static String toString(long value);
		static String toString(unsigned int value); //%u
		static String toString(unsigned short value);
		static String toString(unsigned long value);
		static String toString(char value); //%c
		static String toString(const char* value); //%s
		static String toString(const String &value); //%s
		static String toString(float value); //%f
		static String toString(double value); //%lf
		static String toString(long double value); //%Lf
		static String toString(void* value); //%p
		static String toString(bool value); // "true" | "false"
		
		// Metodo destructor
		~String();
	};

	// Clase que proporciona el tamanio de tipos de datos
	class Types {
		public:
		// Valores máximos y mínimos para tipos de datos con signo de 16/32 bits
		static const float FLT_MAX;
		static const float FLT_MIN;
		static const double DBL_MAX;
		static const double DBL_MIN;
	};

	// Una de las clases mas importantes
	class Convert {
		public:
		static int toInt(const String &data);
		static short toShort(const String &data);
		static long toLong(const String &data);
		static unsigned int toUnsignedInt(const String &data);
		static unsigned short toUnsignedShort(const String &data);
		static unsigned long toUnsignedLong(const String &data);
		static float toFloat(const String &data);
		static double toDouble(const String &data);
		static long double toLongDouble(const String &data);
		static bool toBool(const String &data);
	};

	/*######## FUNCIONALIDAD DE PROTOTIPOS ########*/
	/*#### CLASE MATH ####*/
	// Constantes
	const double Math::E = 2.71828182845904523536;
	const double Math::PI = 3.14159265358979323846;

	// Metodos
	double Math::abs(double num) {
		return num >= 0.0 ? num : num * -1;
	}

	double Math::acos(double x) {
		return ::acos(x);
	}

	double Math::asin(double x) {
		return ::asin(x);
	}

	double Math::atan(double x) {
		return ::atan(x);
	}

	double Math::atan2(double y, double x) {
		return ::atan2(y, x);
	}

	double Math::ceil(double x) {
		return ::ceil(x);
	}

	double Math::cos(double x) {
		return ::cos(x);
	}

	double Math::cosh(double x) {
		return ::cosh(x);
	}

	double Math::exp(double x) {
		return ::exp(x);
	}

	double Math::floor(double x) {
		return ::floor(x);
	}

	double Math::log(double x) {
		return ::log(x);
	}

	double Math::log(double x, int n) {
		if (n > 1) {
			return log(x) / log(n);
		}
		
		return -1;
	}

	double Math::log10(double x) {
		return log(x, 10);
	}

	int Math::mod(int x, int y) {
		return x % y;
	}

	double Math::mod(double x, double y) {
		return ::fmod(x, y);
	}

	double Math::pow(double x, double y) {
		return ::pow(x, y);
	}

	double Math::sin(double x) {
		return ::sin(x);
	}

	double Math::sinh(double x) {
		return ::sinh(x);
	}

	double Math::sqrt(double x) {
		return ::sqrt(x);
	}

	double Math::tan(double x) {
		return ::tan(x);
	}

	double Math::tanh(double x) {
		return ::tanh(x);
	}

	double Math::hypot(double a, double b) {
		return sqrt(pow(a, 2) + pow(b, 2));
	}

	bool Math::pair(int num) {
		return num % 2 == 0 ? true : false;
	}

	int Math::min(int num, int num2) {
		return num < num2 ? num : num2;
	}

	int Math::max(int num, int num2) {
		return num > num2 ? num : num2;
	}

	double Math::min(double num, double num2) {
		return num < num2 ? num : num2;
	}

	double Math::max(double num, double num2) {
		return num > num2 ? num : num2;
	}

	/*#### CLASE STRING ####*/
	/* Metodo constructor */
	String::String(const char* value) {
		if (value) {
			this->len = strlen(value);
			this->data = new char[len + 1];
			strcpy(data, value);
		}
		else {
			this->len = 0;
			this->data = new char[1];
			this->data[0] = '\0';
		}
	}

	/* Metodo constructor usando otro String */
	String::String(const String &value) {
		this->len = value.len;
		this->data = new char[len + 1];
		strcpy(data, value.data);
	}

	/* Metodo para devolver la longitud */
	size_t String::lenght() const {
		return len;
	}

	/* Metodo para devolver el dato interno */
	String::operator char*() const {
		return data;
	}

	/* Metodo que permite usar = para asignar valores */
	String& String::operator = (const char* value) {
		delete[] this->data;
		if (value) {
			this->len = strlen(value);
			this->data = new char[len + 1];
			strcpy(data, value);
		}
		else {
			this->len = 0;
			this->data = new char[1];
			this->data[0] = '\0';
		}
		return *this;
	}

	/* Metodo para permitir concatenar char* */
	String& String::operator+=(const char* value) {
		char* newData = new char[len + strlen(value) + 1];
		strcpy(newData, data);
		strcat(newData, value);
		delete[] data;
		
		data = newData;
		len += strlen(value);
		return *this;
	}


	/* Metodo para permitir concatenar otro String */
	String& String::operator += (const String &value) {
		char* newData = new char[len + value.len + 1];
		strcpy(newData, data);
		strcat(newData, value.data);
		delete[] data;
		
		data = newData;
		len += value.len;
		return *this;
	}

	/* Metodo para iterar un string */
	char String::getCharAt(size_t index) const {
		if (index < len) {
			return data[index];
		}
		else {
			return '\0';
		}
	}

	/* Agregar compatibilidad con cout */
	ostream& operator<<(ostream& os, const String& str) {
		os << str.data;
		return os;
	}

	/* Agregar compatibilidad con cin */
	istream& operator>>(istream& is, String& str) {
		char temp[2048]; // Maximo 2048
		is >> temp;
		
		delete[] str.data;
		str.len = strlen(temp);
		str.data = new char[str.len + 1];
		strcpy(str.data, temp);
		
		return is;
	}

	/*### METODOS COMPLEJOS ###*/
	/* Metodo para pasar el String a char* */
	char* String::toArray() const {
		return data;
	}

	/* Metodo para devolver una cadena formateada */
	String String::format(const char* secuence, ...) {
		va_list args;
		va_start(args, secuence);
		
		char buffer[2048]; // Maximo 2048
		vsprintf(buffer, secuence, args);
		
		va_end(args);
		String value = buffer;
		return value;
	}

	/* Metodo para comparar si una cadena es igual a otra */
	bool String::equals(const char* value) const {
		return strcmp(data, value) == 0 ? true : false;
	}

	/* Metodo para comparar si una cadena es igual a otra */
	bool String::equals(const String &value) const {
		return this->equals(value.toArray());
	}

	/* Metodo para comparar si una cadena contiene a otra */
	bool String::contains(const char* value) const {
		return strstr(data, value) != NULL ? true : false;
	}

	/* Metodo para comparar si una cadena contiene a otra */
	bool String::contains(const String &value) const {
		return this->contains(value.toArray());
	}

	/* Metodo para comparar si una cadena inicia con algo */
	bool String::startsWith(const char* value) const {
		int size = strlen(value);
		
		if (size > len) {
			return false;
		}
		else {
			return strncmp(data, value, size) == 0 ? true : false;
		}
	}

	/* Metodo para comparar si una cadena inicia con algo */
	bool String::startsWith(const String &value) const {
		return this->startsWith(value.toArray());
	}

	/* Metodo para comparar si una cadena finaliza con algo */
	bool String::endsWith(const char* value) const {
		int size = strlen(value);
		
		if (size > len) {
			return false;
		}
		else {
			return strcmp(data + len - size, value) == 0 ? true : false;
		}
	}

	/* Metodo para comparar si una cadena finaliza con algo */
	bool String::endsWith(const String &value) const {
		return this->endsWith(value.toArray());
	}

	/* Metodo para convertir a caracteres minusculas */
	String String::toLowerCase(String &value) {
		String result;

		for (size_t i = 0; i < value.lenght(); i++)
		{
			char character = value.getCharAt(i);
			character = tolower(character);

			result += String::toString(character);
		}

		return result;
	}

	/* Metodo para convertir a caracteres mayusculas */
	String String::toUpperCase(String &value) {
		String result;

		for (size_t i = 0; i < value.lenght(); i++)
		{
			char character = value.getCharAt(i);
			character = toupper(character);

			result += String::toString(character);
		}

		return result;
	}

	/*### METODOS TOSTRING() ###*/
	String String::toString(int value) { //%d
		return String::format("%d", value);
	}

	String String::toString(short value) {
		return String::format("%d", value);
	}

	String String::toString(long value) {
		return String::format("%d", value);
	}

	String String::toString(unsigned int value) { //%u
		return String::format("%u", value);
	}

	String String::toString(unsigned short value) {
		return String::format("%u", value);
	}

	String String::toString(unsigned long value) {
		return String::format("%u", value);
	}

	String String::toString(char value) { //%c
		return String::format("%c", value);
	}

	String String::toString(const char* value) { //%s
		return String::format("'%s'", value);
	}

	String String::toString(const String &value) { //%s
		return String::format("'%s'", value.toArray());
	}

	String String::toString(float value) { //%f
		return String::format("%f", value);
	}

	String String::toString(double value) { //%lf
		return String::format("%lf", value);
	}

	String String::toString(long double value) { //%Lf
		return String::format("%Lf", value);
	}

	String String::toString(void* value) { //%p
		return String::format("%p", value);
	}

	String String::toString(bool value) { // "true" | "false"
		return value ? "true" : "false";
	}

	/* Metodo destructor */
	String::~String() {
		delete[] this->data;
	}

	/*#### CLASE TYPES ####*/
	// Valores maximos y minimos para tipos de datos con signo de 16/32 bits
	const float Types::FLT_MAX = 3.40282347e+38F;
	const float Types::FLT_MIN = 1.17549435e-38F;
	const double Types::DBL_MAX = 1.7976931348623157e+308;
	const double Types::DBL_MIN = 2.2250738585072014e-308;

	/*#### CLASE CONVERT ####*/
	/* Metodo para convertir string a int */
	int Convert::toInt(const String &data) {
		try {
			int value = stoi(data.toArray());
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1;
	}

	/* Metodo para convertir string a short */
	short Convert::toShort(const String &data) {
		try {
			short value = static_cast<short> (stoi(data.toArray()));
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1;
	}

	/* Metodo para convertir string a long */
	long Convert::toLong(const String &data) {
		try {
			long value = stol(data.toArray());
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1L;
	}

	/* Metodo para convertir string a unsigned int */
	unsigned int Convert::toUnsignedInt(const String &data) {
		try {
			unsigned int value = static_cast<unsigned int> (stoul(data.toArray()));
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return 1;
	}

	/* Metodo para convertir string a unsigned short */
	unsigned short Convert::toUnsignedShort(const String &data) {
		try {
			unsigned short value = static_cast<unsigned short> (stoul(data.toArray()));
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1;
	}

	/* Metodo para convertir string a unsigned long */
	unsigned long Convert::toUnsignedLong(const String &data) {
		try {
			unsigned long value = stoul(data.toArray());
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1L;
	}

	/* Metodo para convertir string a float */
	float Convert::toFloat(const String &data) {
		try {
			float value = stof(data.toArray());
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1.0f;
	}

	/* Metodo para convertir string a double */
	double Convert::toDouble(const String &data) {
		try {
			double value = stod(data.toArray());
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1.0;
	}

	/* Metodo para convertir string a long double */
	long double Convert::toLongDouble(const String &data) {
		try {
			long double value = stold(data.toArray());
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return -1.0L;
	}

	/* Metodo para convertir string a bool */
	bool Convert::toBool(const String &data) {
		try {
			bool value = stoi(data.toArray()) != 0;
			return value;
		}
		catch (const invalid_argument &e) {}
		catch (const out_of_range &e) {}

		return false;
	}
}

#endif // DSYUTIL_HPP