#ifndef HUBFLOW_HPP
#define HUBFLOW_HPP

#include "Envio.hpp"
#include "ListaPendientes.hpp"
#include "Historial.hpp"
#include "NivelServicio.hpp"

#include <string>

class HubFlow
{
public:

    HubFlow();

    ~HubFlow();

    HubFlow(const HubFlow&) = delete;

    HubFlow& operator=(const HubFlow&) = delete;

    bool registrarEnvio(const std::string& codigo,
                        const std::string& destinatario,
                        const std::string& zona,
                        double peso,
                        NivelServicio servicio);

    void mostrarPendientes() const;

    Envio* buscarEnvio(const std::string& codigo) const;

    bool cambiarEstado(const std::string& codigo,
                       EstadoEnvio nuevoEstado,
                       const std::string& observacion);

    Envio* despacharProximo();

    bool reprogramarEnvio(const std::string& codigo,
                          const std::string& observacion);

    bool finalizarEntrega(const std::string& codigo,
                          const std::string& observacion);

    bool mostrarHistorialAdelante(const std::string& codigo) const;

    bool mostrarHistorialAtras(const std::string& codigo) const;

    void mostrarResumenPorZona(const std::string& zona) const;

    /**
     * @brief Obtiene el envío pendiente de mayor peso de una zona.
     *
     * La búsqueda se realiza recorriendo recursivamente la lista
     * de envíos pendientes.
     *
     * @param zona Zona que se desea consultar.
     *
     * @return Puntero al envío de mayor peso de la zona o nullptr
     *         si no hay envíos pendientes en esa zona.
     */
    Envio* obtenerMasPesadoPorZona(const std::string& zona) const;

private:

    struct NodoEnvio
    {
        Envio* envio;
        NodoEnvio* siguiente;

        explicit NodoEnvio(Envio* e)
            : envio(e), siguiente(nullptr)
        {
        }
    };

    NodoEnvio* registro_;

    ListaPendientes pendientes_;

    Envio* buscarRecursivo(NodoEnvio* nodo,
                           const std::string& codigo) const;

    void destruirRegistro(NodoEnvio* nodo);

    void resumenPorZonaRecursivo(
        const NodoPendiente* nodo,
        const std::string& zona,
        int& cantidad,
        double& pesoTotal,
        int& cantidadExpress
    ) const;

    /**
     * @brief Busca recursivamente el envío más pesado de una zona.
     *
     * @param nodo Nodo actual de la lista de pendientes.
     * @param zona Zona que se está buscando.
     * @param masPesado Puntero al envío más pesado encontrado hasta el momento.
     *
     * @return Puntero al envío más pesado encontrado.
     */
    Envio* obtenerMasPesadoPorZonaRecursivo(
        const NodoPendiente* nodo,
        const std::string& zona,
        Envio* masPesado
    ) const;
};

#endif // HUBFLOW_HPP