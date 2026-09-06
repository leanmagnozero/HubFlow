#ifndef HUBFLOW_HISTORIAL_HPP
#define HUBFLOW_HISTORIAL_HPP

#include <iostream>
#include <string>

enum class EstadoEnvio {
    RECIBIDO,
    CLASIFICADO,
    EN_REPARTO,
    REPROGRAMADO,
    ENTREGADO
};

class Movimiento {
private:
    int numero;
    EstadoEnvio estado;
    std::string observacion;

public:
    Movimiento(int num, EstadoEnvio est, const std::string& obs)
        : numero(num), estado(est), observacion(obs) {}

    int getNumero() const { return numero; }
    EstadoEnvio getEstado() const { return estado; }
    const std::string& getObservacion() const { return observacion; }
};

class NodoDoble {
public:
    Movimiento dato;
    NodoDoble* next;
    NodoDoble* prev;

    NodoDoble(const Movimiento& m) : dato(m), next(nullptr), prev(nullptr) {}
};

class Historial {
private:
    NodoDoble* cabeza; // primer movimiento en el tiempo
    NodoDoble* cola;   // último movimiento registrado

public:
    Historial() : cabeza(nullptr), cola(nullptr) {}

    ~Historial() {
        NodoDoble* actual = cabeza;
        while (actual != nullptr) {
            NodoDoble* siguiente = actual->next;
            delete actual;
            actual = siguiente;
        }
    }

    void agregar(const Movimiento& m) {
        NodoDoble* nuevo = new NodoDoble(m);
        if (cabeza == nullptr) {
            cabeza = nuevo;
            cola = nuevo;
        } else {
            cola->next = nuevo;
            nuevo->prev = cola;
            cola = nuevo;
        }
    }

    // Cantidad de movimientos registrados hasta ahora — sirve para que
    // quien agregue un movimiento nuevo sepa qué número secuencial usar.
    int cantidad() const {
        int n = 0;
        NodoDoble* actual = cabeza;
        while (actual != nullptr) {
            ++n;
            actual = actual->next;
        }
        return n;
    }

    void mostrarAdelante() const {
        NodoDoble* actual = cabeza;
        while (actual != nullptr) {
            std::cout << actual->dato.getNumero() << " | " << actual->dato.getObservacion() << std::endl;
            actual = actual->next;
        }
    }

    void mostrarAtras() const {
        NodoDoble* actual = cola;
        while (actual != nullptr) {
            std::cout << actual->dato.getNumero() << " | " << actual->dato.getObservacion() << std::endl;
            actual = actual->prev;
        }
    }
};

#endif // HUBFLOW_HISTORIAL_HPP