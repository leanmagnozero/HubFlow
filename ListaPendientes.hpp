#ifndef HUBFLOW_LISTA_PENDIENTES_HPP
#define HUBFLOW_LISTA_PENDIENTES_HPP
#include "Envio.hpp"

/**
 * @brief Nodo de la lista simplemente enlazada de pendientes.
 *
 * El nodo NO es dueño del Envio: solo lo referencia mediante un
 * puntero no-owning. El ownership del objeto envio lo administra
 * otra estructura (ej: un registro central de envíos por código),
 * de modo que despachar un envío (sacarlo de esta lista) no
 * destruye el objeto.
 */
struct NodoPendiente
{
    Envio* envio;
    NodoPendiente* siguiente;

    explicit NodoPendiente(Envio* e) : envio(e), siguiente(nullptr) {}
};

/**
 * @brief Lista simplemente enlazada que funciona como cola de
 * prioridad estable para los envíos pendientes.
 *
 * Los elementos se mantienen ordenados por NivelServicio
 * (EXPRESS > PRIORITARIO > ESTANDAR); entre elementos de igual
 * nivel se conserva el orden de llegada (estabilidad FIFO).
 */
class ListaPendientes
{
  public:
    ListaPendientes() = default;

    ListaPendientes(const ListaPendientes&) = delete;
    ListaPendientes& operator=(const ListaPendientes&) = delete;

    /// Libera todos los nodos de la lista (no los objetos Envio referenciados).
    ~ListaPendientes();

    /**
     * @brief Inserta un envío respetando prioridad y estabilidad.
     * @param envio Puntero al envío a insertar (no-owning para la lista).
     * Complejidad: O(1) mejor caso, O(n) peor caso.
     */
    void insertar(Envio* envio);

    /**
     * @brief Extrae, sin destruir, el envío con mayor prioridad.
     * @return Puntero al envío que estaba al frente, o nullptr si la lista está vacía.
     * Complejidad: O(1).
     */
    Envio* despacharProximo();

    /**
     * @brief Busca un envío por su código recorriendo la estructura.
     * @param codigo Código a buscar.
     * @return Puntero al envío encontrado, o nullptr si no existe.
     * Complejidad: O(n).
     */
    Envio* buscarPorCodigo(const std::string& codigo) const;

    /// @return true si la lista no tiene elementos.
    bool estaVacia() const { return cabeza_ == nullptr; }

    /// Acceso de solo lectura al primer nodo, para recorridos externos (RF02, resumen recursivo).
    const NodoPendiente* getCabeza() const { return cabeza_; }

  private:
    NodoPendiente* cabeza_{nullptr};
};
#endif // HUBFLOW_LISTA_PENDIENTES_HPP

