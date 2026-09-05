#ifndef HUBFLOW_ENVIO_HPP
#define HUBFLOW_ENVIO_HPP
#include "NivelServicio.hpp"
#include <string>

/// Estados posibles del ciclo de vida de un envío.
enum class EstadoEnvio
{
    RECIBIDO,
    CLASIFICADO,
    EN_REPARTO,
    REPROGRAMADO,
    ENTREGADO
};

/**
 * @brief Representa un envío gestionado por el centro de distribución.
 *
 * Esta clase no administra su propio ciclo de vida de memoria respecto
 * de las listas: el ownership del objeto Envio recae en la estructura
 * que lo haya creado (ej: un registro central), nunca en los nodos de
 * ListaPendientes ni de un historial, que solo lo referencian.
 */
class Envio
{
  public:
    /**
     * @brief Construye un envío nuevo con estado inicial RECIBIDO.
     * @param codigo Código único de seguimiento.
     * @param destinatario Nombre del destinatario.
     * @param zona Zona de entrega.
     * @param peso Peso en kilogramos.
     * @param servicio Nivel de servicio contratado.
     */
    Envio(std::string codigo, std::string destinatario, std::string zona,
          double peso, NivelServicio servicio);

    const std::string& getCodigo() const { return codigo_; }
    const std::string& getDestinatario() const { return destinatario_; }
    const std::string& getZona() const { return zona_; }
    double getPeso() const { return peso_; }
    NivelServicio getServicio() const { return servicio_; }
    EstadoEnvio getEstado() const { return estado_; }
    int getIntentos() const { return intentos_; }

    void setEstado(EstadoEnvio nuevoEstado) { estado_ = nuevoEstado; }
    void incrementarIntentos() { ++intentos_; }

  private:
    std::string codigo_;
    std::string destinatario_;
    std::string zona_;
    double peso_;
    NivelServicio servicio_;
    EstadoEnvio estado_{EstadoEnvio::RECIBIDO};
    int intentos_{0};
};
#endif // HUBFLOW_ENVIO_HPP

