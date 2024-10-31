import os

class Libro:
    def __init__(self, titulo, autor, año, editorial, isbn, paginas):
        self.titulo = titulo
        self.autor = autor
        self.año = año
        self.editorial = editorial
        self.isbn = isbn
        self.paginas = paginas

class Nodo:
    def __init__(self, libro):
        self.libro = libro
        self.siguiente = None

class ListaLibros:
    def __init__(self):
        self.cabeza = None

    def agregar_libro(self, libro):
        nuevo_nodo = Nodo(libro)
        nuevo_nodo.siguiente = self.cabeza
        self.cabeza = nuevo_nodo

    def contar_libros(self):
        contador = 0
        actual = self.cabeza
        while actual:
            contador += 1
            actual = actual.siguiente
        return contador

    def listar_libros(self):
        actual = self.cabeza
        if actual is None:
            print("No hay libros en la biblioteca.")
        else:
            print("\nLibos en la biblioteca:")
            while actual:
                print(f"Título: {actual.libro.titulo}, Autor: {actual.libro.autor}, Año: {actual.libro.año}, Editorial: {actual.libro.editorial}, ISBN: {actual.libro.isbn}, Páginas: {actual.libro.paginas}")
                actual = actual.siguiente

    def guardar_lista_libros(self):
        with open('biblioteca.txt', 'w') as archivo:
            actual = self.cabeza
            while actual:
                archivo.write(f"{actual.libro.titulo},{actual.libro.autor},{actual.libro.año},{actual.libro.editorial},{actual.libro.isbn},{actual.libro.paginas}\n")
                actual = actual.siguiente
        print("Lista de libros guardada en biblioteca.txt")

    def cargar_lista_libros(self):
        if os.path.exists('biblioteca.txt'):
            with open('biblioteca.txt', 'r') as archivo:
                for linea in archivo:
                    datos = linea.strip().split(',')
                    if len(datos) == 6:
                        titulo, autor, año, editorial, isbn, paginas = datos
                        libro = Libro(titulo, autor, int(año), editorial, isbn, int(paginas))
                        self.agregar_libro(libro)

    def buscar_libro(self, criterio, valor):
        actual = self.cabeza
        while actual:
            libro = actual.libro
            if ((criterio == 'titulo' and libro.titulo.lower() == valor.lower()) or
                (criterio == 'autor' and libro.autor.lower() == valor.lower()) or
                (criterio == 'isbn' and libro.isbn == valor)):
                return libro
            actual = actual.siguiente
        return None

    def quicksort(self):
        if self.cabeza is None or self.cabeza.siguiente is None:
            return self

        menor = ListaLibros()
        igual = ListaLibros()
        mayor = ListaLibros()
        pivote = self.cabeza.libro.titulo
        actual = self.cabeza

        while actual:
            if actual.libro.titulo < pivote:
                menor.agregar_libro(actual.libro)
            elif actual.libro.titulo == pivote:
                igual.agregar_libro(actual.libro)
            else:
                mayor.agregar_libro(actual.libro)
            actual = actual.siguiente

        menor = menor.quicksort()
        mayor = mayor.quicksort()

        if menor.cabeza is not None:
            self.cabeza = menor.cabeza
            temp = menor.cabeza
            while temp.siguiente is not None:
                temp = temp.siguiente
            temp.siguiente = igual.cabeza
        else:
            self.cabeza = igual.cabeza

        if self.cabeza is None:
            self.cabeza = mayor.cabeza
        else:
            temp = self.cabeza
            while temp.siguiente is not None:
                temp = temp.siguiente
            temp.siguiente = mayor.cabeza

        return self

class Lector:
    def __init__(self, nombre, dni, libro_solicitado):
        self.nombre = nombre
        self.dni = dni
        self.libro_solicitado = libro_solicitado

class NodoCola:
    def __init__(self, lector):
        self.lector = lector
        self.siguiente = None

class ColaLectores:
    def __init__(self):
        self.frente = None
        self.final = None

    def encolar_lector(self, lector):
        nuevo_nodo = NodoCola(lector)
        if self.final is None:
            self.frente = self.final = nuevo_nodo
        else:
            self.final.siguiente = nuevo_nodo
            self.final = nuevo_nodo

    def desencolar_lector(self):
        if self.frente is None:
            return None
        lector = self.frente.lector
        self.frente = self.frente.siguiente
        if self.frente is None:
            self.final = None
        return lector

    def listar_solicitudes(self):
        actual = self.frente
        if actual is None:
            print("No hay solicitudes de préstamo.")
        else:
            print("\nSolicitudes de préstamo:")
            while actual:
                print(f"Nombre: {actual.lector.nombre}, DNI: {actual.lector.dni}, Libro solicitado: {actual.lector.libro_solicitado.titulo}")
                actual = actual.siguiente

    def guardar_solicitudes(self):
        with open('solicitudes.txt', 'w') as archivo:
            actual = self.frente
            while actual:
                archivo.write(f"{actual.lector.nombre},{actual.lector.dni},{actual.lector.libro_solicitado.titulo}\n")
                actual = actual.siguiente
        print("Solicitudes guardadas en solicitudes.txt")

    def cargar_solicitudes(self, lista_libros):
        if os.path.exists('solicitudes.txt'):
            with open('solicitudes.txt', 'r') as archivo:
                for linea in archivo:
                    datos = linea.strip().split(',')
                    if len(datos) == 3:
                        nombre, dni, titulo_libro = datos
                        libro = lista_libros.buscar_libro('titulo', titulo_libro)
                        if libro:
                            lector = Lector(nombre, dni, libro)
                            self.encolar_lector(lector)

class Historial:
    def __init__(self):
        self.operaciones = []

    def agregar_operacion(self, operacion):
        self.operaciones.append(operacion)

    def deshacer_operacion(self):
        if self.operaciones:
            return self.operaciones.pop()
        return None

def agregar_libro(libros, historial):
    titulo = input("Título: ")
    autor = input("Autor: ")
    año = int(input("Año de edición: "))
    editorial = input("Editorial: ")
    isbn = input("ISBN: ")
    paginas = int(input("Número de páginas: "))
    libro = Libro(titulo, autor, año, editorial, isbn, paginas)
    libros.agregar_libro(libro)
    historial.agregar_operacion(f"Se agregó el libro: {titulo}")

def solicitar_libro(libros, solicitudes, historial):
    titulo = input("Título del libro a solicitar: ")
    libro = libros.buscar_libro('titulo', titulo)
    if libro:
        nombre = input("Nombre del lector: ")
        dni = input("DNI del lector: ")
        lector = Lector(nombre, dni, libro)
        solicitudes.encolar_lector(lector)
        historial.agregar_operacion(f"{nombre} solicitó el libro: {titulo}")
        print(f"Solicitud registrada para el libro: {titulo}")
    else:
        print("Libro no encontrado.")

def devolver_libro(libros, solicitudes, historial):
    if solicitudes.frente is None:
        print("No hay solicitudes para devolver.")
        return

    lector = solicitudes.desencolar_lector()
    libro = lector.libro_solicitado
    print(f"El lector {lector.nombre} ha devuelto el libro: {libro.titulo}.")
    historial.agregar_operacion(f"{lector.nombre} devolvió el libro: {libro.titulo}")

def buscar_libro_menu(libros):
    criterio = input("Buscar por (titulo, autor, isbn): ").lower()
    valor = input("Ingrese el valor a buscar: ")
    libro_encontrado = libros.buscar_libro(criterio, valor)
    if libro_encontrado:
        print(f"Libro encontrado: {libro_encontrado.titulo}, Autor: {libro_encontrado.autor}, Año: {libro_encontrado.año}")
    else:
        print("Libro no encontrado.")

def menu_principal():
    libros = ListaLibros()
    solicitudes = ColaLectores()
    historial = Historial()

    libros.cargar_lista_libros()
    solicitudes.cargar_solicitudes(libros)

    while True:
        print("\n--- Menú de Biblioteca ---")
        print("1. Agregar libro")
        print("2. Solicitar libro")
        print("3. Devolver libro")
        print("4. Buscar libro")
        print("5. Listar libros")
        print("6. Listar solicitudes")
        print("7. Ordenar libros por título")
        print("8. Guardar datos")
        print("9. Cargar datos")
        print("10. Salir")
        
        opcion = input("Selecciona una opción: ")

        if opcion == '1':
            agregar_libro(libros, historial)
        elif opcion == '2':
            solicitar_libro(libros, solicitudes, historial)
        elif opcion == '3':
            devolver_libro(libros, solicitudes, historial)
        elif opcion == '4':
            buscar_libro_menu(libros)
        elif opcion == '5':
            libros.listar_libros()
        elif opcion == '6':
            solicitudes.listar_solicitudes()
        elif opcion == '7':
            libros.quicksort()
            print("Libros ordenados por título.")
        elif opcion == '8':
            libros.guardar_lista_libros()
            solicitudes.guardar_solicitudes()
        elif opcion == '9':
            libros.cargar_lista_libros()
            solicitudes.cargar_solicitudes(libros)
        elif opcion == '10':
            print("Saliendo del programa.")
            break
        else:
            print("Opción no válida.")

menu_principal()








