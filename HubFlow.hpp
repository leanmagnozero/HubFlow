#ifndef HUBFLOW_HPP
#define HUBFLOW_HPP

#include "Envio.hpp"
#include "ListaPendientes.hpp"
#include "Historial.hpp"
#include "NivelServicio.hpp"

#include <string>

/**
 * @brief Coordina la gestión de los envíos del centro de distribución.
 *
 * HubFlow mantiene el registro central de los envíos y coordina las
 * operaciones de registro, búsqueda, despacho, reprogramación,
 * actualización de estados y finalización de entregas.
 */
class HubFlow
{
public:

    /**
     * @brief Construye un HubFlow vacío.
     */
    HubFlow() = default;

    /**
     * @brief Registra un nuevo envío en el sistema.
     *
     * El envío debe quedar almacenado en el registro central y también
     * incorporarse a la lista de pendientes respetando su prioridad.
     *
     * @param envio Envío que se desea registrar.
     */
    void registrarEnvio(Envio* envio);

    /**
     * @brief Muestra todos los envíos que se encuentran pendientes.
     */
    void mostrarPendientes() const;

    /**
     * @brief Busca un envío por su código de seguimiento.
     *
     * La búsqueda se realiza sobre el registro central, por lo que
     * también permite encontrar envíos que ya fueron despachados.
     *
     * @param codigo Código de seguimiento del envío.
     * @return Puntero al envío encontrado, o nullptr si no existe.
     */
    Envio* buscarEnvio(const std::string& codigo);

    /**
     * @brief Cambia el estado de un envío y registra el movimiento.
     *
     * @param codigo Código del envío cuyo estado se desea modificar.
     * @param nuevoEstado Nuevo estado del envío.
     * @param observacion Descripción del movimiento realizado.
     * @return true si el envío fue encontrado y actualizado,
     *         false si no existe.
     */
    bool cambiarEstado(const std::string& codigo,
                       EstadoEnvio nuevoEstado,
                       const std::string& observacion);

    /**
     * @brief Despacha el próximo envío según su prioridad.
     *
     * El envío se elimina de la lista de pendientes pero continúa
     * existiendo en el registro central.
     *
     * @return Puntero al envío despachado, o nullptr si no hay
     *         envíos pendientes.
     */
    Envio* despacharProximo();

    /**
     * @brief Reprograma un envío y registra el movimiento correspondiente.
     *
     * @param codigo Código del envío que se desea reprogramar.
     * @param observacion Motivo o detalle de la reprogramación.
     * @return true si el envío fue encontrado y reprogramado,
     *         false si no existe.
     */
    bool reprogramarEnvio(const std::string& codigo,
                          const std::string& observacion);

    /**
     * @brief Finaliza la entrega de un envío.
     *
     * Cambia su estado a ENTREGADO y registra el movimiento
     * correspondiente en su historial.
     *
     * @param codigo Código del envío que se desea finalizar.
     * @param observacion Detalle asociado a la entrega.
     * @return true si el envío fue encontrado y finalizado,
     *         false si no existe.
     */
    bool finalizarEntrega(const std::string& codigo,
                          const std::string& observacion);

    /**
     * @brief Muestra un resumen de los envíos correspondientes a una zona.
     *
     * El recorrido de los envíos se realiza mediante una función
     * recursiva, sin utilizar ciclos en dicho recorrido.
     *
     * @param zona Zona sobre la cual se desea obtener el resumen.
     */
    void mostrarResumenPorZona(const std::string& zona) const;

private:
    /**
     * @brief Lista de envíos que todavía se encuentran pendientes de despacho.
     */
    ListaPendientes pendientes_;

    /*
     * Registro central de envíos.
     *
     * Acá tenemos que decidir qué estructura vamos a utilizar para
     * mantener vivos los Envio aunque salgan de ListaPendientes.
     */

    /**
     * @brief Realiza recursivamente el resumen de los envíos de una zona.
     *
     * @param nodo Nodo actual desde el cual continuar el recorrido.
     * @param zona Zona que se desea consultar.
     * @param cantidad Cantidad acumulada de envíos encontrados.
     */
    void resumenPorZonaRecursivo(const std::string& zona, int& cantidad) const;
};

#endif // HUBFLOW_HPP