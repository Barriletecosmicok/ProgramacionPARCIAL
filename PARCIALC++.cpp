#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Libro {
public:
    string titulo;
    string autor;
    int año;
    string editorial;
    string ISBN;
    int paginas;

    Libro(string t, string a, int an, string e, string i, int p)
        : titulo(t), autor(a), año(an), editorial(e), ISBN(i), paginas(p) {}
};

class NodoLibro {
public:
    Libro libro;
    NodoLibro* siguiente;

    NodoLibro(Libro l) : libro(l), siguiente(nullptr) {}
};

class ListaLibros {
private:
    NodoLibro* cabeza;

public:
    ListaLibros() : cabeza(nullptr) {}

    void agregar_libro(Libro libro) {
        NodoLibro* nuevo_nodo = new NodoLibro(libro);
        nuevo_nodo->siguiente = cabeza;
        cabeza = nuevo_nodo;
    }

    void listar_libros() {
        NodoLibro* actual = cabeza;
        while (actual != nullptr) {
            cout << "Título: " << actual->libro.titulo
                 << ", Autor: " << actual->libro.autor
                 << ", Año: " << actual->libro.año
                 << ", Editorial: " << actual->libro.editorial
                 << ", ISBN: " << actual->libro.ISBN
                 << ", Páginas: " << actual->libro.paginas << endl;
            actual = actual->siguiente;
        }
    }

    bool esta_vacia() {
        return cabeza == nullptr;
    }

    void ordenar_libros() {
        if (cabeza == nullptr || cabeza->siguiente == nullptr) return;

        NodoLibro* actual;
        NodoLibro* siguiente;
        bool intercambiado;

        do {
            intercambiado = false;
            actual = cabeza;

            while (actual->siguiente != nullptr) {
                siguiente = actual->siguiente;
                if (actual->libro.titulo > siguiente->libro.titulo) {
                    swap(actual->libro, siguiente->libro);
                    intercambiado = true;
                }
                actual = siguiente;
            }
        } while (intercambiado);
    }

    void guardar_datos() {
        ofstream archivo("biblioteca.txt");
        NodoLibro* actual = cabeza;
        while (actual != nullptr) {
            archivo << actual->libro.titulo << ","
                    << actual->libro.autor << ","
                    << actual->libro.año << ","
                    << actual->libro.editorial << ","
                    << actual->libro.ISBN << ","
                    << actual->libro.paginas << endl;
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void cargar_datos() {
        ifstream archivo("biblioteca.txt");
        string linea;
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string t, a, e, i;
            int an, p;
            getline(ss, t, ',');
            getline(ss, a, ',');
            ss >> an;
            ss.ignore();
            getline(ss, e, ',');
            getline(ss, i, ',');
            ss >> p;
            agregar_libro(Libro(t, a, an, e, i, p));
        }
        archivo.close();
    }

    bool buscar_libro(string criterio, string valor) {
        NodoLibro* actual = cabeza;
        while (actual != nullptr) {
            if ((criterio == "titulo" && actual->libro.titulo == valor) ||
                (criterio == "autor" && actual->libro.autor == valor) ||
                (criterio == "ISBN" && actual->libro.ISBN == valor)) {
                cout << "Libro encontrado: " << actual->libro.titulo << endl;
                return true;
            }
            actual = actual->siguiente;
        }
        cout << "Libro no encontrado." << endl;
        return false;
    }
};

class Solicitud {
public:
    string nombre;
    string dni;
    string libroSolicitado;
    Solicitud* siguiente;

    Solicitud(string n, string d, string libro) : nombre(n), dni(d), libroSolicitado(libro), siguiente(nullptr) {}
};

class ColaSolicitudes {
private:
    Solicitud* frente;
    Solicitud* fin;

public:
    ColaSolicitudes() : frente(nullptr), fin(nullptr) {}

    void encolar(string nombre, string dni, string libro) {
        Solicitud* nuevaSolicitud = new Solicitud(nombre, dni, libro);
        if (fin != nullptr) {
            fin->siguiente = nuevaSolicitud;
        } else {
            frente = nuevaSolicitud;
        }
        fin = nuevaSolicitud;
    }

    bool esta_vacia() {
        return frente == nullptr;
    }

    Solicitud* desencolar() {
        if (esta_vacia()) return nullptr;
        Solicitud* temp = frente;
        frente = frente->siguiente;
        if (frente == nullptr) fin = nullptr;
        return temp;
    }

    void listar_solicitudes() {
        Solicitud* actual = frente;
        while (actual != nullptr) {
            cout << actual->nombre << " " << actual->dni << " solicitó " << actual->libroSolicitado << endl;
            actual = actual->siguiente;
        }
    }
};

class Historial {
private:
    vector<string> acciones;

public:
    void agregar_accion(string accion) {
        acciones.push_back(accion);
    }

    void mostrar_historial() {
        if (acciones.empty()) {
            cout << "No hay acciones en el historial." << endl;
            return;
        }
        for (const auto& accion : acciones) {
            cout << accion << endl;
        }
    }

    void borrar_historial() {
        acciones.clear();
        cout << "Historial borrado." << endl;
    }
};

class SistemaBiblioteca {
private:
    ListaLibros listaLibros;
    ColaSolicitudes colaSolicitudes;
    Historial historial;

public:
    SistemaBiblioteca() {
        listaLibros.cargar_datos();
    }

    void agregar_libro() {
        string titulo, autor, editorial, ISBN;
        int año, paginas;
        cout << "Ingrese el título: ";
        cin.ignore();
        getline(cin, titulo);
        cout << "Ingrese el autor: ";
        getline(cin, autor);
        cout << "Ingrese el año de edición: ";
        cin >> año;
        cout << "Ingrese la editorial: ";
        cin.ignore();
        getline(cin, editorial);
        cout << "Ingrese el ISBN: ";
        getline(cin, ISBN);
        cout << "Ingrese el número de páginas: ";
        cin >> paginas;
        listaLibros.agregar_libro(Libro(titulo, autor, año, editorial, ISBN, paginas));
        listaLibros.guardar_datos();
        historial.agregar_accion("Libro agregado: " + titulo);
    }

    void solicitar_libro() {
        string nombre, dni, titulo;
        cout << "Ingrese su nombre: ";
        cin.ignore();
        getline(cin, nombre);
        cout << "Ingrese su DNI: ";
        getline(cin, dni);
        cout << "Ingrese el título del libro: ";
        getline(cin, titulo);

        if (listaLibros.esta_vacia()) {
            cout << "No hay libros disponibles." << endl;
            return;
        }

        if (colaSolicitudes.esta_vacia()) {
            colaSolicitudes.encolar(nombre, dni, titulo);
            historial.agregar_accion(nombre + " ha solicitado el libro: " + titulo);
            cout << "Solicitud registrada." << endl;
        } else {
            cout << "El libro ya está solicitado. Se ha añadido a la cola de espera." << endl;
            colaSolicitudes.encolar(nombre, dni, titulo);
            historial.agregar_accion(nombre + " ha sido agregado a la cola para el libro: " + titulo);
        }
    }

    void devolver_libro() {
        string titulo;
        cout << "Ingrese el título del libro que desea devolver: ";
        cin.ignore();
        getline(cin, titulo);
        Solicitud* solicitud = colaSolicitudes.desencolar();

        if (solicitud == nullptr) {
            cout << "Nadie ha solicitado este libro." << endl;
            return;
        }
        cout << "Libro devuelto. Entregado a: " << solicitud->nombre << endl;
        historial.agregar_accion("Libro devuelto: " + titulo + " entregado a " + solicitud->nombre);
        delete solicitud;
    }

    void buscar_libro() {
        string criterio, valor;
        cout << "Buscar por (titulo, autor, ISBN): ";
        cin >> criterio;
        cout << "Ingrese el valor a buscar: ";
        cin.ignore();
        getline(cin, valor);
        listaLibros.buscar_libro(criterio, valor);
    }

    void listar_libros() {
        listaLibros.listar_libros();
    }

    void listar_solicitudes() {
        colaSolicitudes.listar_solicitudes();
    }

    void ordenar_libros() {
        listaLibros.ordenar_libros();
        cout << "Libros ordenados por título." << endl;
    }

    void mostrar_historial() {
        historial.mostrar_historial();
    }

    void borrar_historial() {
        historial.borrar_historial();
    }

    void menu() {
        int opcion;
        do {
            cout << "\n1. Agregar libro" << endl;
            cout << "2. Solicitar libro" << endl;
            cout << "3. Devolver libro" << endl;
            cout << "4. Buscar libro" << endl;
            cout << "5. Listar libros" << endl;
            cout << "6. Listar solicitudes" << endl;
            cout << "7. Ordenar libros" << endl;
            cout << "8. Mostrar historial" << endl;
            cout << "9. Borrar historial" << endl;
            cout << "10. Salir" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;

            switch (opcion) {
                case 1: agregar_libro(); break;
                case 2: solicitar_libro(); break;
                case 3: devolver_libro(); break;
                case 4: buscar_libro(); break;
                case 5: listar_libros(); break;
                case 6: listar_solicitudes(); break;
                case 7: ordenar_libros(); break;
                case 8: mostrar_historial(); break;
                case 9: borrar_historial(); break;
                case 10: cout << "Saliendo..." << endl; break;
                default: cout << "Opción no válida." << endl; break;
            }
        } while (opcion != 10);
    }
};

int main() {
    SistemaBiblioteca sistema;
    sistema.menu();
    return 0;
}
