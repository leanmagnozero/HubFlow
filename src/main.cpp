#include "HubFlow.hpp"
#include "Historial.hpp"
#include "NivelServicio.hpp"

#include <iostream>
#include <limits>
#include <string>

// ============================================================
// DATASET INICIAL
// ============================================================

static void cargarDatasetInicial(HubFlow& hub) {
    hub.registrarEnvio(
        "PKG-1001", "Ana Torres", "CENTRO",
        1.20, NivelServicio::ESTANDAR
    );

    hub.registrarEnvio(
        "PKG-1002", "Bruno Diaz", "NORTE",
        0.75, NivelServicio::EXPRESS
    );

    hub.registrarEnvio(
        "PKG-1003", "Carla Ruiz", "SUR",
        4.10, NivelServicio::PRIORITARIO
    );

    hub.registrarEnvio(
        "PKG-1004", "Diego Lopez", "CENTRO",
        2.30, NivelServicio::ESTANDAR
    );

    hub.registrarEnvio(
        "PKG-1005", "Elena Castro", "NORTE",
        1.90, NivelServicio::PRIORITARIO
    );

    hub.registrarEnvio(
        "PKG-1006", "Franco Gomez", "SUR",
        0.50, NivelServicio::EXPRESS
    );

    hub.registrarEnvio(
        "PKG-1007", "Gabriela Soto", "CENTRO",
        6.20, NivelServicio::ESTANDAR
    );

    hub.registrarEnvio(
        "PKG-1008", "Hugo Perez", "NORTE",
        3.40, NivelServicio::PRIORITARIO
    );
}

// ============================================================
// UTILIDADES PARA LA INTERACCION CON EL USUARIO
// ============================================================

static void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

static int leerEntero(const std::string& mensaje) {
    int valor;

    while (true) {
        std::cout << mensaje;

        if (std::cin >> valor) {
            limpiarBuffer();
            return valor;
        }

        std::cout << "Entrada invalida. Debe ingresar un numero.\n";
        limpiarBuffer();
    }
}

static double leerPeso() {
    double peso;

    while (true) {
        std::cout << "Peso (kg): ";

        if (std::cin >> peso) {
            limpiarBuffer();

            if (peso > 0) {
                return peso;
            }

            std::cout << "El peso debe ser mayor que 0.\n";
        } else {
            std::cout << "Entrada invalida. Ingrese un numero.\n";
            limpiarBuffer();
        }
    }
}

static std::string leerTexto(const std::string& mensaje) {
    std::string texto;

    std::cout << mensaje;
    std::getline(std::cin, texto);

    return texto;
}

// ============================================================
// MENU
// ============================================================

static void mostrarMenu() {
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "              HUBFLOW\n";
    std::cout << "====================================\n";
    std::cout << "1. Mostrar envios pendientes\n";
    std::cout << "2. Registrar nuevo envio\n";
    std::cout << "3. Buscar envio por codigo\n";
    std::cout << "4. Cambiar estado\n";
    std::cout << "5. Despachar proximo envio\n";
    std::cout << "6. Reprogramar envio\n";
    std::cout << "7. Finalizar entrega\n";
    std::cout << "8. Mostrar historial\n";
    std::cout << "9. Obtener resumen por zona\n";
    std::cout << "10. Finalizar programa\n";
    std::cout << "====================================\n";
}

// ============================================================
// OPCION 2 - REGISTRAR ENVIO
// ============================================================

static void registrarNuevoEnvio(HubFlow& hub) {
    std::cout << "\n--- REGISTRAR NUEVO ENVIO ---\n";

    std::string codigo = leerTexto("Codigo: ");

    if (codigo.empty()) {
        std::cout << "El codigo no puede estar vacio.\n";
        return;
    }

    std::string destinatario = leerTexto("Destinatario: ");

    if (destinatario.empty()) {
        std::cout << "El destinatario no puede estar vacio.\n";
        return;
    }

    std::string zona = leerTexto("Zona: ");

    if (zona.empty()) {
        std::cout << "La zona no puede estar vacia.\n";
        return;
    }

    double peso = leerPeso();

    int servInt;

    while (true) {
        servInt = leerEntero(
            "Servicio (0: EXPRESS, 1: PRIORITARIO, 2: ESTANDAR): "
        );

        if (servInt >= 0 && servInt <= 2) {
            break;
        }

        std::cout << "Servicio invalido. Ingrese 0, 1 o 2.\n";
    }

    auto servicio = static_cast<NivelServicio>(servInt);

    if (hub.registrarEnvio(
            codigo,
            destinatario,
            zona,
            peso,
            servicio)) {

        std::cout << "Envio registrado con exito.\n";

    } else {

        std::cout << "Error: el codigo ya existe.\n";
    }
}

// ============================================================
// OPCION 3 - BUSCAR ENVIO
// ============================================================

static void buscarEnvio(const HubFlow& hub) {
    std::cout << "\n--- BUSCAR ENVIO ---\n";

    std::string codigo = leerTexto("Codigo a buscar: ");

    Envio* envio = hub.buscarEnvio(codigo);

    if (envio != nullptr) {

        std::cout << "\nEnvio encontrado:\n";

        std::cout << "Codigo: "
                  << envio->getCodigo() << "\n";

        std::cout << "Destinatario: "
                  << envio->getDestinatario() << "\n";

        std::cout << "Zona: "
                  << envio->getZona() << "\n";

        std::cout << "Peso: "
                  << envio->getPeso() << " kg\n";

    } else {

        std::cout << "Envio no encontrado.\n";
    }
}

// ============================================================
// OPCION 4 - CAMBIAR ESTADO
// ============================================================

static void cambiarEstado(HubFlow& hub) {
    std::cout << "\n--- CAMBIAR ESTADO ---\n";

    std::string codigo = leerTexto("Codigo: ");

    int estado;

    while (true) {
        estado = leerEntero(
            "Nuevo estado "
            "(0: RECIBIDO, "
            "1: CLASIFICADO, "
            "2: EN_REPARTO, "
            "3: REPROGRAMADO, "
            "4: ENTREGADO): "
        );

        if (estado >= 0 && estado <= 4) {
            break;
        }

        std::cout << "Estado invalido. "
                     "Ingrese un valor entre 0 y 4.\n";
    }

    std::string observacion = leerTexto("Observacion: ");

    auto nuevoEstado = static_cast<EstadoEnvio>(estado);

    if (hub.cambiarEstado(
            codigo,
            nuevoEstado,
            observacion)) {

        std::cout << "Estado actualizado correctamente.\n";

    } else {

        std::cout << "Error al actualizar el estado.\n";
        std::cout << "Verifique que el envio exista.\n";
    }
}

// ============================================================
// OPCION 5 - DESPACHAR
// ============================================================

static void despacharEnvio(HubFlow& hub) {
    std::cout << "\n--- DESPACHAR PROXIMO ENVIO ---\n";

    Envio* envio = hub.despacharProximo();

    if (envio != nullptr) {

        std::cout << "Envio despachado con exito.\n";
        std::cout << "Codigo: "
                  << envio->getCodigo() << "\n";

    } else {

        std::cout
            << "No hay envios pendientes para despachar.\n";
    }
}

// ============================================================
// OPCION 6 - REPROGRAMAR
// ============================================================

static void reprogramarEnvio(HubFlow& hub) {
    std::cout << "\n--- REPROGRAMAR ENVIO ---\n";

    std::string codigo = leerTexto("Codigo: ");

    std::string motivo =
        leerTexto("Motivo de reprogramacion: ");

    if (hub.reprogramarEnvio(codigo, motivo)) {

        std::cout
            << "Envio reprogramado correctamente.\n";

    } else {

        std::cout
            << "Error: el envio no existe "
            << "o ya fue entregado.\n";
    }
}

// ============================================================
// OPCION 7 - FINALIZAR ENTREGA
// ============================================================

static void finalizarEntrega(HubFlow& hub) {
    std::cout << "\n--- FINALIZAR ENTREGA ---\n";

    std::string codigo = leerTexto("Codigo: ");

    std::string observacion =
        leerTexto("Observacion de entrega: ");

    if (hub.finalizarEntrega(codigo, observacion)) {

        std::cout
            << "Entrega finalizada con exito.\n";

    } else {

        std::cout
            << "Error al finalizar la entrega.\n";

        std::cout
            << "Verifique que el envio exista "
            << "y pueda ser finalizado.\n";
    }
}

// ============================================================
// OPCION 8 - HISTORIAL
// ============================================================

static void mostrarHistorial(const HubFlow& hub) {
    std::cout << "\n--- HISTORIAL DEL ENVIO ---\n";

    std::string codigo = leerTexto("Codigo: ");

    int modo;

    while (true) {

        modo = leerEntero(
            "Modo (1: Cronologico / 2: Inverso): "
        );

        if (modo == 1 || modo == 2) {
            break;
        }

        std::cout
            << "Modo invalido. Ingrese 1 o 2.\n";
    }

    bool encontrado;

    if (modo == 1) {

        encontrado =
            hub.mostrarHistorialAdelante(codigo);

    } else {

        encontrado =
            hub.mostrarHistorialAtras(codigo);
    }

    if (!encontrado) {
        std::cout << "Envio no encontrado.\n";
    }
}

// ============================================================
// OPCION 9 - RESUMEN POR ZONA
// ============================================================

static void mostrarResumenPorZona(const HubFlow& hub) {
    std::cout << "\n--- RESUMEN POR ZONA ---\n";

    std::string zona = leerTexto("Zona a consultar: ");

    if (zona.empty()) {
        std::cout << "La zona no puede estar vacia.\n";
        return;
    }

    hub.mostrarResumenPorZona(zona);
}

// ============================================================
// MAIN
// ============================================================

int main() {
    HubFlow hub;

    // Carga de los envios iniciales.
    cargarDatasetInicial(hub);

    int opcion;

    do {
        mostrarMenu();

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

            case 1:
                std::cout
                    << "\n--- ENVIOS PENDIENTES ---\n";

                hub.mostrarPendientes();
                break;

            case 2:
                registrarNuevoEnvio(hub);
                break;

            case 3:
                buscarEnvio(hub);
                break;

            case 4:
                cambiarEstado(hub);
                break;

            case 5:
                despacharEnvio(hub);
                break;

            case 6:
                reprogramarEnvio(hub);
                break;

            case 7:
                finalizarEntrega(hub);
                break;

            case 8:
                mostrarHistorial(hub);
                break;

            case 9:
                mostrarResumenPorZona(hub);
                break;

            case 10:
                std::cout
                    << "\nPrograma finalizado.\n";
                break;

            default:
                std::cout
                    << "Opcion invalida. "
                    << "Ingrese un numero entre 1 y 10.\n";
                break;
        }

    } while (opcion != 10);

    return 0;
}