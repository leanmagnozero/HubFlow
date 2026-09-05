#include "../include/HubFlow.hpp"

#include <iostream>

HubFlow::HubFlow()
    : registro_(nullptr),
      pendientes_()
{
}

HubFlow::~HubFlow()
{
    destruirRegistro(registro_);
}

void HubFlow::destruirRegistro(NodoEnvio* nodo)
{
    if (nodo == nullptr)
    {
        return;
    }

    destruirRegistro(nodo->siguiente);

    delete nodo->envio;
    delete nodo;
}

bool HubFlow::registrarEnvio(const std::string& codigo,
                             const std::string& destinatario,
                             const std::string& zona,
                             double peso,
                             NivelServicio servicio)
{
    // Verificamos que el código sea único.
    if (buscarEnvio(codigo) != nullptr)
    {
        return false;
    }

    // Creamos dinámicamente el envío.
    Envio* nuevoEnvio = new Envio(
        codigo,
        destinatario,
        zona,
        peso,
        servicio
    );

    // Registramos el primer movimiento.
    nuevoEnvio->getHistorial().agregar(
        Movimiento(
            1,
            EstadoEnvio::RECIBIDO,
            "Ingreso al centro de distribución"
        )
    );

    // Creamos el nodo del registro central.
    NodoEnvio* nuevoNodo = new NodoEnvio(nuevoEnvio);

    // Insertamos el nodo al principio del registro.
    nuevoNodo->siguiente = registro_;
    registro_ = nuevoNodo;

    // Insertamos el envío en pendientes.
    pendientes_.insertar(nuevoEnvio);

    return true;
}

Envio* HubFlow::buscarRecursivo(
    NodoEnvio* nodo,
    const std::string& codigo) const
{
    // Caso base:
    // llegamos al final de la lista.
    if (nodo == nullptr)
    {
        return nullptr;
    }

    // Encontramos el envío.
    if (nodo->envio->getCodigo() == codigo)
    {
        return nodo->envio;
    }

    // Caso recursivo:
    // buscamos en el siguiente nodo.
    return buscarRecursivo(
        nodo->siguiente,
        codigo
    );
}

Envio* HubFlow::buscarEnvio(
    const std::string& codigo) const
{
    return buscarRecursivo(
        registro_,
        codigo
    );
}

void HubFlow::mostrarPendientes() const
{
    const NodoPendiente* actual =
        pendientes_.getCabeza();

    if (actual == nullptr)
    {
        std::cout
            << "No hay envios pendientes.\n";

        return;
    }

    std::cout
        << "\n========== ENVIOS PENDIENTES ==========\n";

    while (actual != nullptr)
    {
        const Envio* envio = actual->envio;

        std::cout
            << "Codigo: "
            << envio->getCodigo()

            << " | Destinatario: "
            << envio->getDestinatario()

            << " | Zona: "
            << envio->getZona()

            << " | Peso: "
            << envio->getPeso()
            << " kg"

            << " | Servicio: "
            << static_cast<int>(
                envio->getServicio()
            )

            << " | Estado: "
            << static_cast<int>(
                envio->getEstado()
            )

            << " | Intentos: "
            << envio->getIntentos()

            << '\n';

        actual = actual->siguiente;
    }
}

bool HubFlow::cambiarEstado(
    const std::string& codigo,
    EstadoEnvio nuevoEstado,
    const std::string& observacion)
{
    Envio* envio = buscarEnvio(codigo);

    if (envio == nullptr)
    {
        return false;
    }

    // Actualizamos el estado.
    envio->setEstado(nuevoEstado);

    // El nuevo movimiento recibe el número siguiente.
    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

    // Guardamos el cambio en el historial.
    envio->getHistorial().agregar(
        Movimiento(
            numeroMovimiento,
            nuevoEstado,
            observacion
        )
    );

    return true;
}

Envio* HubFlow::despacharProximo()
{
    /*
     * La lista elimina el NodoPendiente,
     * pero NO elimina el Envio.
     */
    Envio* envio =
        pendientes_.despacharProximo();

    if (envio == nullptr)
    {
        return nullptr;
    }

    // Cambiamos el estado.
    envio->setEstado(
        EstadoEnvio::EN_REPARTO
    );

    // Creamos el movimiento.
    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

    envio->getHistorial().agregar(
        Movimiento(
            numeroMovimiento,
            EstadoEnvio::EN_REPARTO,
            "Envio despachado"
        )
    );

    return envio;
}

bool HubFlow::reprogramarEnvio(
    const std::string& codigo,
    const std::string& observacion)
{
    Envio* envio = buscarEnvio(codigo);

    if (envio == nullptr)
    {
        return false;
    }

    // Un envío entregado no puede volver a pendientes.
    if (envio->getEstado() ==
        EstadoEnvio::ENTREGADO)
    {
        return false;
    }

    // Aumentamos la cantidad de intentos.
    envio->incrementarIntentos();

    // Cambiamos el estado.
    envio->setEstado(
        EstadoEnvio::REPROGRAMADO
    );

    // Obtenemos el número del nuevo movimiento.
    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

    // Registramos la reprogramación.
    envio->getHistorial().agregar(
        Movimiento(
            numeroMovimiento,
            EstadoEnvio::REPROGRAMADO,
            observacion
        )
    );

    // Volvemos a insertar el envío en pendientes.
    // La lista decide dónde colocarlo según prioridad.
    pendientes_.insertar(envio);

    return true;
}

bool HubFlow::finalizarEntrega(
    const std::string& codigo,
    const std::string& observacion)
{
    Envio* envio = buscarEnvio(codigo);

    if (envio == nullptr)
    {
        return false;
    }

    // Evitamos entregar dos veces el mismo envío.
    if (envio->getEstado() ==
        EstadoEnvio::ENTREGADO)
    {
        return false;
    }

    // Cambiamos el estado.
    envio->setEstado(
        EstadoEnvio::ENTREGADO
    );

    // Calculamos el número del movimiento.
    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

    // Registramos la entrega.
    envio->getHistorial().agregar(
        Movimiento(
            numeroMovimiento,
            EstadoEnvio::ENTREGADO,
            observacion
        )
    );

    return true;
}

bool HubFlow::mostrarHistorialAdelante(
    const std::string& codigo) const
{
    Envio* envio = buscarEnvio(codigo);

    if (envio == nullptr)
    {
        return false;
    }

    envio->getHistorial().mostrarAdelante();

    return true;
}

bool HubFlow::mostrarHistorialAtras(
    const std::string& codigo) const
{
    Envio* envio = buscarEnvio(codigo);

    if (envio == nullptr)
    {
        return false;
    }

    envio->getHistorial().mostrarAtras();

    return true;
}

void HubFlow::resumenPorZonaRecursivo(
    const NodoPendiente* nodo,
    const std::string& zona,
    int& cantidad,
    double& pesoTotal,
    int& cantidadExpress) const
{
    // CASO BASE
    if (nodo == nullptr)
    {
        return;
    }

    const Envio* envio = nodo->envio;

    // Procesamos el nodo actual.
    if (envio->getZona() == zona)
    {
        ++cantidad;

        pesoTotal += envio->getPeso();

        if (envio->getServicio() ==
            NivelServicio::EXPRESS)
        {
            ++cantidadExpress;
        }
    }

    // LLAMADA RECURSIVA
    resumenPorZonaRecursivo(
        nodo->siguiente,
        zona,
        cantidad,
        pesoTotal,
        cantidadExpress
    );
}

void HubFlow::mostrarResumenPorZona(
    const std::string& zona) const
{
    int cantidad = 0;
    double pesoTotal = 0.0;
    int cantidadExpress = 0;

    resumenPorZonaRecursivo(
        pendientes_.getCabeza(),
        zona,
        cantidad,
        pesoTotal,
        cantidadExpress
    );

    std::cout
        << "\n========== RESUMEN POR ZONA ==========\n";

    std::cout
        << "Zona: "
        << zona
        << '\n';

    std::cout
        << "Cantidad de paquetes: "
        << cantidad
        << '\n';

    std::cout
        << "Peso total pendiente: "
        << pesoTotal
        << " kg\n";

    std::cout
        << "Cantidad EXPRESS: "
        << cantidadExpress
        << '\n';
}