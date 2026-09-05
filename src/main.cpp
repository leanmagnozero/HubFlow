#include "HubFlow.hpp"
#include "Historial.hpp"     // Define EstadoEnvio
#include "NivelServicio.hpp"  // Define NivelServicio
#include <iostream>
#include <limits>
#include <string>

// Carga del Dataset Inicial (Sección 8 de la consigna)
void cargarDatasetInicial(HubFlow& hub) {
    hub.registrarEnvio("PKG-1001", "Ana Torres", "CENTRO", 1.20, NivelServicio::ESTANDAR);
    hub.registrarEnvio("PKG-1002", "Bruno Diaz", "NORTE", 0.75, NivelServicio::EXPRESS);
    hub.registrarEnvio("PKG-1003", "Carla Ruiz", "SUR", 4.10, NivelServicio::PRIORITARIO);
    hub.registrarEnvio("PKG-1004", "Diego Lopez", "CENTRO", 2.30, NivelServicio::ESTANDAR);
    hub.registrarEnvio("PKG-1005", "Elena Castro", "NORTE", 1.90, NivelServicio::PRIORITARIO);
    hub.registrarEnvio("PKG-1006", "Franco Gomez", "SUR", 0.50, NivelServicio::EXPRESS);
    hub.registrarEnvio("PKG-1007", "Gabriela Soto", "CENTRO", 6.20, NivelServicio::ESTANDAR);
    hub.registrarEnvio("PKG-1008", "Hugo Perez", "NORTE", 3.40, NivelServicio::PRIORITARIO);
}

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    HubFlow hub;
    cargarDatasetInicial(hub);

    int opcion = 0;
    do {
        std::cout << "\n========== HUBFLOW ==========\n"
                  << "1. Mostrar envios pendientes\n"
                  << "2. Registrar nuevo envio\n"
                  << "3. Buscar envio por codigo\n"
                  << "4. Cambiar estado\n"
                  << "5. Despachar proximo envio\n"
                  << "6. Reprogramar envio\n"
                  << "7. Finalizar entrega\n"
                  << "8. Mostrar historial\n"
                  << "9. Obtener resumen recursivo por zona\n"
                  << "10. Finalizar programa\n"
                  << "Opcion: ";

        if (!(std::cin >> opcion)) {
            limpiarBuffer();
            continue;
        }

        if (opcion == 1) {
            hub.mostrarPendientes();
        }
        else if (opcion == 2) {
            std::string cod, dest, zona;
            double peso = 0.0;
            int servInt = 0;

            std::cout << "Codigo: "; std::cin >> cod;
            std::cout << "Destinatario: "; std::cin >> dest;
            std::cout << "Zona: "; std::cin >> zona;
            std::cout << "Peso (kg): "; std::cin >> peso;
            std::cout << "Servicio (0: EXPRESS, 1: PRIORITARIO, 2: ESTANDAR): ";
            std::cin >> servInt;

            NivelServicio serv = static_cast<NivelServicio>(servInt);
            if (hub.registrarEnvio(cod, dest, zona, peso, serv)) {
                std::cout << "Envio registrado con exito.\n";
            } else {
                std::cout << "Error: El codigo ya existe.\n";
            }
        }
        else if (opcion == 3) {
            std::string cod;
            std::cout << "Codigo a buscar: "; std::cin >> cod;
            Envio* e = hub.buscarEnvio(cod);
            if (e != nullptr) {
                std::cout << "Encontrado: " << e->getCodigo()
                          << " | Destinatario: " << e->getDestinatario()
                          << " | Zona: " << e->getZona()
                          << " | Peso: " << e->getPeso() << " kg\n";
            } else {
                std::cout << "Envio no encontrado.\n";
            }
        }
        else if (opcion == 4) {
            std::string cod, obs;
            int estInt = 0;

            std::cout << "Codigo: "; std::cin >> cod;
            std::cout << "Nuevo estado (0:RECIBIDO, 1:CLASIFICADO, 2:EN_REPARTO, 3:REPROGRAMADO, 4:ENTREGADO): ";
            std::cin >> estInt;
            std::cout << "Observacion: ";
            limpiarBuffer();
            std::getline(std::cin, obs);

            if (hub.cambiarEstado(cod, static_cast<EstadoEnvio>(estInt), obs)) {
                std::cout << "Estado actualizado.\n";
            } else {
                std::cout << "Error al actualizar estado.\n";
            }
        }
        else if (opcion == 5) {
            Envio* desp = hub.despacharProximo();
            if (desp != nullptr) {
                std::cout << "Despachado con exito: " << desp->getCodigo() << "\n";
            } else {
                std::cout << "No hay envios pendientes para despachar.\n";
            }
        }
        else if (opcion == 6) {
            std::string cod, obs;
            std::cout << "Codigo: "; std::cin >> cod;
            std::cout << "Motivo de reprogramacion: ";
            limpiarBuffer();
            std::getline(std::cin, obs);

            if (hub.reprogramarEnvio(cod, obs)) {
                std::cout << "Envio reprogramado y vuelto a ingresar a pendientes.\n";
            } else {
                std::cout << "Error: envio no existe o ya esta entregado.\n";
            }
        }
        else if (opcion == 7) {
            std::string cod, obs;
            std::cout << "Codigo: "; std::cin >> cod;
            std::cout << "Observacion de entrega: ";
            limpiarBuffer();
            std::getline(std::cin, obs);

            if (hub.finalizarEntrega(cod, obs)) {
                std::cout << "Entrega finalizada con exito.\n";
            } else {
                std::cout << "Error al finalizar la entrega.\n";
            }
        }
        else if (opcion == 8) {
            std::string cod;
            int modo = 1;
            std::cout << "Codigo: "; std::cin >> cod;
            std::cout << "Modo (1: Cronologico / 2: Inverso): "; std::cin >> modo;

            if (modo == 1) {
                if (!hub.mostrarHistorialAdelante(cod)) {
                    std::cout << "Envio no encontrado.\n";
                }
            } else {
                if (!hub.mostrarHistorialAtras(cod)) {
                    std::cout << "Envio no encontrado.\n";
                }
            }
        }
        else if (opcion == 9) {
            std::string zona;
            std::cout << "Zona a consultar: "; std::cin >> zona;
            hub.mostrarResumenPorZona(zona);
        }

    } while (opcion != 10);

    return 0;
}