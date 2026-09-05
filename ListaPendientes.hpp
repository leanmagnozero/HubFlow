#ifndef HUBFLOW_LISTA_PENDIENTES_HPP
#define HUBFLOW_LISTA_PENDIENTES_HPP

#include "Envio.hpp"

/**
 * @brief Nodo de la lista simplemente enlazada de pendientes.
 *
 * El nodo NO es dueño del Envio: solo lo referencia mediante un
 * puntero no-owning. El ownership real del objeto Envio lo tiene
 * el registro central de HubFlow, así que sacar un nodo de esta
 * lista (despacharProximo) no destruye el envío.
 */
struct NodoPendiente {
    Envio* envio;
    NodoPendiente* siguiente;

    explicit NodoPendiente(Envio* e) : envio(e), siguiente(nullptr) {}
};

/**
 * @brief Lista simplemente enlazada que funciona como cola de
 * prioridad estable para los envíos pendientes.
 *
 * Ordenada por NivelServicio (EXPRESS > PRIORITARIO > ESTANDAR);
 * entre elementos de igual nivel se conserva el orden de llegada.
 */
class ListaPendientes {
public:
    ListaPendientes() = default;
    ListaPendientes(const ListaPendientes&) = delete;
    ListaPendientes& operator=(const ListaPendientes&) = delete;

    /// Libera todos los nodos de la lista (no los objetos Envio referenciados).
    ~ListaPendientes();

    /// Inserta un envío respetando prioridad y estabilidad FIFO.
    void insertar(Envio* envio);

    /// Extrae, sin destruir, el envío con mayor prioridad (el primero).
    Envio* despacharProximo();

    /// Busca un envío por código recorriendo la lista. Solo entre los pendientes.
    Envio* buscarPorCodigo(const std::string& codigo) const;

    bool estaVacia() const { return cabeza_ == nullptr; }

    /// Acceso de solo lectura, para recorridos externos (mostrar, recursiva).
    const NodoPendiente* getCabeza() const { return cabeza_; }

private:
    NodoPendiente* cabeza_{nullptr};
};

#endif // HUBFLOW_LISTA_PENDIENTES_HPP