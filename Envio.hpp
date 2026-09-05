#ifndef HUBFLOW_ENVIO_HPP
#define HUBFLOW_ENVIO_HPP

#include "NivelServicio.hpp"
#include "Historial.hpp"
#include <string>

/// Estados posibles del ciclo de vida de un envío.
enum class EstadoEnvio {
    RECIBIDO,
    CLASIFICADO,
    EN_REPARTO,
    REPROGRAMADO,
    ENTREGADO
};

/**
 * @brief Representa un envío gestionado por el centro de distribución.
 *
 * Es dueño de su propio Historial: se crea junto con el envío y se
 * destruye automáticamente cuando el envío se destruye (RAII), sin
 * necesidad de new/delete manual para esa parte.
 */
class Envio {
public:
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

    // Acceso al historial: no-const para poder agregar movimientos,
    // const para solo consultarlo (mostrarAdelante/mostrarAtras).
    Historial& getHistorial() { return historial_; }
    const Historial& getHistorial() const { return historial_; }

private:
    std::string codigo_;
    std::string destinatario_;
    std::string zona_;
    double peso_;
    NivelServicio servicio_;
    EstadoEnvio estado_{EstadoEnvio::RECIBIDO};
    int intentos_{0};
    Historial historial_;   // miembro por valor: Envio es dueño, se destruye solo
};

#endif // HUBFLOW_ENVIO_HPP