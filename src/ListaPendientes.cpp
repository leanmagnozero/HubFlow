#include "../include/ListaPendientes.hpp"
#include "../include/NivelServicio.hpp"

ListaPendientes::~ListaPendientes() {
  NodoPendiente *actual = cabeza_;
  while (actual != nullptr) {
    NodoPendiente *siguiente = actual->siguiente;
    delete actual; // solo el nodo: el Envio no es nuestro
    actual = siguiente;
  }
}

void ListaPendientes::insertar(Envio *envio) {
  NodoPendiente *nuevo = new NodoPendiente(envio);

  // Caso 1: lista vacía -> el nuevo nodo pasa a ser la cabeza.
  if (cabeza_ == nullptr) {
    cabeza_ = nuevo;
    return;
  }

  // Caso 2: el nuevo tiene MAYOR prioridad que la cabeza actual -> va al
  // inicio. Ojo con el uso de tieneMayorPrioridad y no "<=": así, a igual
  // prioridad, el nuevo NO desplaza al que ya estaba primero (eso es lo que
  // garantiza la estabilidad FIFO). [analizar algo mas optimizado]

  if (tieneMayorPrioridad(envio->getServicio(),
                          cabeza_->envio->getServicio())) {
    nuevo->siguiente = cabeza_;
    cabeza_ = nuevo;
    return;
  }

  // Caso 3 y 4: buscar el punto de inserción recorriendo la lista.
  // "anterior" avanza mientras el siguiente exista y NO tenga
  // prioridad estrictamente menor que el nuevo -- es decir,
  // avanzamos mientras el siguiente tenga prioridad mayor o igual.
  NodoPendiente *anterior = cabeza_;
  while (anterior->siguiente != nullptr &&
         !tieneMayorPrioridad(envio->getServicio(),
                              anterior->siguiente->envio->getServicio())) {
    anterior = anterior->siguiente;
  }

  // anterior->siguiente es nullptr (caso 4: inserción al final)
  // o apunta a un nodo de menor prioridad (caso 3: inserción al medio).
  nuevo->siguiente = anterior->siguiente;
  anterior->siguiente = nuevo;
}

Envio *ListaPendientes::despacharProximo() {
  if (cabeza_ == nullptr) {
    return nullptr;
  }
  NodoPendiente *viejo = cabeza_;
  Envio *envio = viejo->envio;
  cabeza_ = viejo->siguiente;
  delete viejo; // se libera el nodo; el Envio sigue vivo en el registro central
  return envio;
}

Envio *ListaPendientes::buscarPorCodigo(const std::string &codigo) const {
  NodoPendiente *actual = cabeza_;
  while (actual != nullptr) {
    if (actual->envio->getCodigo() == codigo) {
      return actual->envio;
    }
    actual = actual->siguiente;
  }
  return nullptr;
}

bool ListaPendientes::extraerPorCodigo(const std::string &codigo) {
  if (cabeza_ == nullptr) {
    return false;
  }

  // Caso 1: El nodo a extraer es la cabeza
  if (cabeza_->envio->getCodigo() == codigo) {
    NodoPendiente *aEliminar = cabeza_;
    cabeza_ = cabeza_->siguiente;
    delete aEliminar; // Liberamos solo el nodo contenedor, el Envio sigue vivo
    return true;
  }

  // Caso 2: El nodo a extraer está en el medio o al final
  NodoPendiente *anterior = cabeza_;
  while (anterior->siguiente != nullptr &&
         anterior->siguiente->envio->getCodigo() != codigo) {
    anterior = anterior->siguiente;
  }

  if (anterior->siguiente != nullptr) {
    NodoPendiente *aEliminar = anterior->siguiente;
    anterior->siguiente = aEliminar->siguiente; // Bypass / desenganche
    delete aEliminar; // Liberamos solo el nodo contenedor
    return true;
  }

  return false; // No se encontró en pendientes
}