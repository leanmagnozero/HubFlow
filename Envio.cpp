#include "Envio.hpp"

Envio::Envio(std::string codigo, std::string destinatario, std::string zona,
             double peso, NivelServicio servicio)
    : codigo_(std::move(codigo)),
      destinatario_(std::move(destinatario)),
      zona_(std::move(zona)),
      peso_(peso),
      servicio_(servicio)
{
    // Movimiento inicial obligatorio (sección 8 / RF01):
    // todo envío nace RECIBIDO y eso queda asentado en su historial.
    historial_.agregar(Movimiento(1, EstadoEnvio::RECIBIDO,
                                  "Ingreso al centro de distribución"));
}