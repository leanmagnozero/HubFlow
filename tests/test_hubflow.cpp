#include "test_hubflow.hpp"

// CASO 1: Prioridades (EXPRESS > PRIORITARIO > ESTANDAR)
TEST(HubFlowTest, Caso1_Prioridades) {
    ListaPendientes lista;
    Envio e1("PKG-A", "Ana", "CENTRO", 1.0, NivelServicio::ESTANDAR);
    Envio e2("PKG-B", "Bruno", "NORTE", 2.0, NivelServicio::EXPRESS);
    Envio e3("PKG-C", "Carla", "SUR", 1.5, NivelServicio::PRIORITARIO);

    lista.insertar(&e1);
    lista.insertar(&e2);
    lista.insertar(&e3);

    // Debe quedar: PKG-B (EXPRESS), PKG-C (PRIORITARIO), PKG-A (ESTANDAR)
    const NodoPendiente* actual = lista.getCabeza();
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(actual->envio->getCodigo(), "PKG-B");

    actual = actual->siguiente;
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(actual->envio->getCodigo(), "PKG-C");

    actual = actual->siguiente;
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(actual->envio->getCodigo(), "PKG-A");
}

// CASO 2: Prioridad Estable (Conserva orden de llegada entre igual prioridad)
TEST(HubFlowTest, Caso2_PrioridadEstable) {
    ListaPendientes lista;
    Envio e1("PKG-A", "Ana", "CENTRO", 1.0, NivelServicio::EXPRESS);
    Envio e2("PKG-B", "Bruno", "NORTE", 2.0, NivelServicio::PRIORITARIO);
    Envio e3("PKG-C", "Carla", "SUR", 1.5, NivelServicio::ESTANDAR);
    Envio e4("PKG-D", "Diego", "NORTE", 3.0, NivelServicio::EXPRESS);

    lista.insertar(&e1);
    lista.insertar(&e2);
    lista.insertar(&e3);
    lista.insertar(&e4);

    // Debe conservar el orden FIFO para los EXPRESS: PKG-A -> PKG-D
    const NodoPendiente* actual = lista.getCabeza();
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(actual->envio->getCodigo(), "PKG-A");

    actual = actual->siguiente;
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(actual->envio->getCodigo(), "PKG-D");
}

// CASO 3: Despacho
TEST(HubFlowTest, Caso3_Despacho) {
    HubFlow hub;
    hub.registrarEnvio("PKG-1001", "Ana", "CENTRO", 1.2, NivelServicio::EXPRESS);

    Envio* desp = hub.despacharProximo();
    ASSERT_NE(desp, nullptr);
    EXPECT_EQ(desp->getCodigo(), "PKG-1001");
    EXPECT_EQ(desp->getEstado(), EstadoEnvio::EN_REPARTO);

    // El objeto Envio sigue existiendo en el registro central aunque no está en pendientes
    EXPECT_NE(hub.buscarEnvio("PKG-1001"), nullptr);
}

// CASO 4: Reprogramación
TEST(HubFlowTest, Caso4_Reprogramacion) {
    HubFlow hub;
    hub.registrarEnvio("PKG-1002", "Bruno", "NORTE", 0.75, NivelServicio::EXPRESS);

    hub.despacharProximo();
    bool reprogramado = hub.reprogramarEnvio("PKG-1002", "Destinatario ausente");

    EXPECT_TRUE(reprogramado);
    Envio* e = hub.buscarEnvio("PKG-1002");
    ASSERT_NE(e, nullptr);
    EXPECT_EQ(e->getIntentos(), 1);
    EXPECT_EQ(e->getEstado(), EstadoEnvio::REPROGRAMADO);
}

// CASO 5: Historial
TEST(HubFlowTest, Caso5_Historial) {
    Historial h;
    h.agregar(Movimiento(1, EstadoEnvio::RECIBIDO, "Ingreso"));
    h.agregar(Movimiento(2, EstadoEnvio::CLASIFICADO, "Clasificado"));

    EXPECT_EQ(h.cantidad(), 2);
}

// CASO 6: Recursividad
TEST(HubFlowTest, Caso6_Recursividad) {
    HubFlow hub;
    hub.registrarEnvio("PKG-01", "A", "NORTE", 1.0, NivelServicio::EXPRESS);
    hub.registrarEnvio("PKG-02", "B", "CENTRO", 2.0, NivelServicio::ESTANDAR);
    hub.registrarEnvio("PKG-03", "C", "NORTE", 3.5, NivelServicio::PRIORITARIO);

    // Probamos que la búsqueda recursiva y la lógica del resumen funcionen sobre la estructura
    Envio* buscado = hub.buscarEnvio("PKG-03");
    ASSERT_NE(buscado, nullptr);
    EXPECT_EQ(buscado->getZona(), "NORTE");
}

// CASO 7: Casos Límite
TEST(HubFlowTest, Caso7_CasosLimite) {
    HubFlow hub;

    // Búsqueda en lista vacía o inexistente
    EXPECT_EQ(hub.buscarEnvio("NO-EXISTE"), nullptr);

    // Código duplicado
    EXPECT_TRUE(hub.registrarEnvio("PKG-X", "Test", "SUR", 1.0, NivelServicio::ESTANDAR));
    EXPECT_FALSE(hub.registrarEnvio("PKG-X", "Test2", "SUR", 2.0, NivelServicio::EXPRESS));

    // Despachar en lista vacía
    hub.despacharProximo(); // vacía la lista
    EXPECT_EQ(hub.despacharProximo(), nullptr);
}

// CASO 8: Paquete más pesado por zona
TEST(HubFlowTest, Caso8_MasPesadoPorZona) {
    HubFlow hub;

    hub.registrarEnvio(
        "PKG-01", "Ana", "NORTE",
        1.0, NivelServicio::EXPRESS
    );

    hub.registrarEnvio(
        "PKG-02", "Bruno", "NORTE",
        3.5, NivelServicio::PRIORITARIO
    );

    hub.registrarEnvio(
        "PKG-03", "Carla", "NORTE",
        2.0, NivelServicio::ESTANDAR
    );

    hub.registrarEnvio(
        "PKG-04", "Diego", "SUR",
        10.0, NivelServicio::EXPRESS
    );

    // El paquete más pesado de NORTE debe ser PKG-02.
    Envio* masPesado =
        hub.obtenerMasPesadoPorZona("NORTE");

    ASSERT_NE(masPesado, nullptr);
    EXPECT_EQ(masPesado->getCodigo(), "PKG-02");
    EXPECT_DOUBLE_EQ(masPesado->getPeso(), 3.5);

    // El paquete de 10 kg pertenece a SUR,
    // por lo tanto no debe afectar el resultado.
    Envio* masPesadoSur =
        hub.obtenerMasPesadoPorZona("SUR");

    ASSERT_NE(masPesadoSur, nullptr);
    EXPECT_EQ(masPesadoSur->getCodigo(), "PKG-04");
}

// CASO 9: Zona sin envíos pendientes
TEST(HubFlowTest, Caso9_MasPesadoZonaInexistente) {
    HubFlow hub;

    hub.registrarEnvio(
        "PKG-01", "Ana", "NORTE",
        1.0, NivelServicio::EXPRESS
    );

    Envio* masPesado =
        hub.obtenerMasPesadoPorZona("OESTE");

    EXPECT_EQ(masPesado, nullptr);
}