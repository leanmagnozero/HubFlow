#include "HubFlow.hpp"

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
    while (nodo != nullptr)
    {
        NodoEnvio* siguiente = nodo->siguiente;

        delete nodo->envio;
        delete nodo;

        nodo = siguiente;
    }
}

bool HubFlow::registrarEnvio(
    const std::string& codigo,
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

    envio->setEstado(nuevoEstado);

    // Si el paquete ya no está en pendientes,
    // lo extraemos de la lista.
    if (nuevoEstado == EstadoEnvio::ENTREGADO ||
        nuevoEstado == EstadoEnvio::EN_REPARTO)
    {
        pendientes_.extraerPorCodigo(codigo);
    }

    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

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

    envio->setEstado(
        EstadoEnvio::EN_REPARTO
    );

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

    if (envio->getEstado() ==
        EstadoEnvio::ENTREGADO)
    {
        return false;
    }

    envio->incrementarIntentos();

    envio->setEstado(
        EstadoEnvio::REPROGRAMADO
    );

    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

    envio->getHistorial().agregar(
        Movimiento(
            numeroMovimiento,
            EstadoEnvio::REPROGRAMADO,
            observacion
        )
    );

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

    if (envio->getEstado() ==
        EstadoEnvio::ENTREGADO)
    {
        return false;
    }

    envio->setEstado(
        EstadoEnvio::ENTREGADO
    );

    pendientes_.extraerPorCodigo(codigo);

    int numeroMovimiento =
        envio->getHistorial().cantidad() + 1;

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

Envio* HubFlow::obtenerMasPesadoPorZonaRecursivo(
    const NodoPendiente* nodo,
    const std::string& zona,
    Envio* masPesado) const
{
    // CASO BASE
    if (nodo == nullptr)
    {
        return masPesado;
    }

    const Envio* envioActual = nodo->envio;

    // Si pertenece a la zona, comparamos su peso.
    if (envioActual->getZona() == zona)
    {
        if (masPesado == nullptr ||
            envioActual->getPeso() > masPesado->getPeso())
        {
            masPesado = nodo->envio;
        }
    }

    // LLAMADA RECURSIVA
    return obtenerMasPesadoPorZonaRecursivo(
        nodo->siguiente,
        zona,
        masPesado
    );
}

Envio* HubFlow::obtenerMasPesadoPorZona(
    const std::string& zona) const
{
    return obtenerMasPesadoPorZonaRecursivo(
        pendientes_.getCabeza(),
        zona,
        nullptr
    );
}