#ifndef HUBFLOW_HPP
#define HUBFLOW_HPP

#include "Envio.hpp"
#include "ListaPendientes.hpp"
#include "Historial.hpp"
#include "NivelServicio.hpp"

#include <string>

/**
 * @brief Coordina la gestión de los envíos del sistema HubFlow.
 *
 * Mantiene un registro central de todos los envíos y una lista
 * de los envíos que se encuentran pendientes.
 *
 * El registro central conserva los objetos Envio aunque estos
 * sean retirados de la lista de pendientes.
 */
class HubFlow
{
public:

    /**
     * @brief Construye un HubFlow vacío.
     */
    HubFlow();

    /**
     * @brief Destruye el sistema y libera la memoria dinámica.
     */
    ~HubFlow();

    /**
     * @brief Impide copiar un HubFlow.
     *
     * @param otro Objeto HubFlow que se intentaría copiar.
     */
    HubFlow(const HubFlow&) = delete;

    /**
     * @brief Impide asignar un HubFlow a otro.
     *
     * @param otro Objeto HubFlow que se intentaría asignar.
     *
     * @return Referencia al objeto actual.
     */
    HubFlow& operator=(const HubFlow&) = delete;

    /**
     * @brief Registra un nuevo envío.
     *
     * Verifica que el código sea único, crea dinámicamente
     * el envío, registra el movimiento inicial y lo incorpora
     * a la lista de pendientes.
     *
     * @param codigo Código único del envío.
     * @param destinatario Nombre del destinatario.
     * @param zona Zona de entrega.
     * @param peso Peso del envío.
     * @param servicio Nivel de servicio.
     *
     * @return true si el envío fue registrado correctamente.
     * @return false si el código ya existe.
     */
    bool registrarEnvio(const std::string& codigo,
                        const std::string& destinatario,
                        const std::string& zona,
                        double peso,
                        NivelServicio servicio);

    /**
     * @brief Muestra todos los envíos pendientes.
     */
    void mostrarPendientes() const;

    /**
     * @brief Busca un envío por su código.
     *
     * La búsqueda se realiza sobre el registro central.
     *
     * @param codigo Código del envío.
     *
     * @return Puntero al envío encontrado o nullptr si no existe.
     */
    Envio* buscarEnvio(const std::string& codigo) const;

    /**
     * @brief Cambia el estado de un envío.
     *
     * También registra el cambio en su historial.
     *
     * @param codigo Código del envío.
     * @param nuevoEstado Nuevo estado.
     * @param observacion Descripción del movimiento.
     *
     * @return true si el envío fue encontrado y actualizado.
     * @return false si el envío no existe.
     */
    bool cambiarEstado(const std::string& codigo,
                       EstadoEnvio nuevoEstado,
                       const std::string& observacion);

    /**
     * @brief Despacha el primer envío pendiente.
     *
     * Cambia su estado a EN_REPARTO y elimina solamente
     * el nodo de la lista de pendientes.
     *
     * El objeto Envio continúa existiendo en el registro central.
     *
     * @return Puntero al envío despachado o nullptr si no hay pendientes.
     */
    Envio* despacharProximo();

    /**
     * @brief Reprograma un envío cuya entrega no pudo realizarse.
     *
     * Incrementa los intentos, cambia el estado a REPROGRAMADO,
     * registra el movimiento y vuelve a insertar el envío
     * en la lista de pendientes respetando su prioridad.
     *
     * @param codigo Código del envío.
     * @param observacion Motivo de la reprogramación.
     *
     * @return true si se pudo reprogramar.
     * @return false si el envío no existe o ya fue entregado.
     */
    bool reprogramarEnvio(const std::string& codigo,
                          const std::string& observacion);

    /**
     * @brief Finaliza la entrega de un envío.
     *
     * Cambia el estado a ENTREGADO y registra el movimiento
     * correspondiente en el historial.
     *
     * @param codigo Código del envío.
     * @param observacion Información sobre la entrega.
     *
     * @return true si se pudo finalizar.
     * @return false si no existe o ya estaba entregado.
     */
    bool finalizarEntrega(const std::string& codigo,
                          const std::string& observacion);

    /**
     * @brief Muestra el historial en orden cronológico.
     *
     * @param codigo Código del envío.
     *
     * @return true si el envío existe.
     * @return false si no existe.
     */
    bool mostrarHistorialAdelante(const std::string& codigo) const;

    /**
     * @brief Muestra el historial en orden inverso.
     *
     * Utiliza los punteros prev de la lista doblemente enlazada.
     *
     * @param codigo Código del envío.
     *
     * @return true si el envío existe.
     * @return false si no existe.
     */
    bool mostrarHistorialAtras(const std::string& codigo) const;

    /**
     * @brief Muestra un resumen recursivo de una zona.
     *
     * Calcula cantidad de paquetes, peso total y cantidad
     * de envíos EXPRESS.
     *
     * @param zona Zona que se desea consultar.
     */
    void mostrarResumenPorZona(const std::string& zona) const;

private:

    /**
     * @brief Nodo del registro central de envíos.
     *
     * El nodo contiene un puntero al envío y un puntero
     * al siguiente nodo.
     */
    struct NodoEnvio
    {
        Envio* envio;
        NodoEnvio* siguiente;

        /**
         * @brief Construye un nodo del registro.
         *
         * @param e Puntero al envío.
         */
        explicit NodoEnvio(Envio* e)
            : envio(e), siguiente(nullptr)
        {
        }
    };

    /**
     * @brief Primer nodo del registro central.
     */
    NodoEnvio* registro_;

    /**
     * @brief Lista de envíos pendientes.
     */
    ListaPendientes pendientes_;

    /**
     * @brief Busca recursivamente un envío.
     *
     * @param nodo Nodo actual del registro.
     * @param codigo Código buscado.
     *
     * @return Puntero al envío encontrado o nullptr.
     */
    Envio* buscarRecursivo(NodoEnvio* nodo,
                           const std::string& codigo) const;

    /**
     * @brief Libera recursivamente el registro central.
     *
     * Destruye los nodos y los objetos Envio almacenados.
     *
     * @param nodo Nodo desde el cual comenzar.
     */
    void destruirRegistro(NodoEnvio* nodo);

    /**
     * @brief Realiza recursivamente el resumen de una zona.
     *
     * @param nodo Nodo actual de la lista de pendientes.
     * @param zona Zona buscada.
     * @param cantidad Cantidad acumulada de paquetes.
     * @param pesoTotal Peso acumulado.
     * @param cantidadExpress Cantidad acumulada de EXPRESS.
     */
    void resumenPorZonaRecursivo(
        const NodoPendiente* nodo,
        const std::string& zona,
        int& cantidad,
        double& pesoTotal,
        int& cantidadExpress
    ) const;
};

#endif // HUBFLOW_HPP