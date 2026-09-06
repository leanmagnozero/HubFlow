#ifndef HUBFLOW_LISTA_PENDIENTES_HPP
#define HUBFLOW_LISTA_PENDIENTES_HPP

#include "Envio.hpp"

struct NodoPendiente {
  Envio *envio;
  NodoPendiente *siguiente;

  explicit NodoPendiente(Envio *e) : envio(e), siguiente(nullptr) {}
};

class ListaPendientes {
public:
  ListaPendientes() = default;
  ListaPendientes(const ListaPendientes &) = delete;
  ListaPendientes &operator=(const ListaPendientes &) = delete;

  ~ListaPendientes();

  void insertar(Envio *envio);

  Envio *despacharProximo();

  Envio *buscarPorCodigo(const std::string &codigo) const;

  bool extraerPorCodigo(const std::string &codigo);

  bool estaVacia() const { return cabeza_ == nullptr; }

  const NodoPendiente *getCabeza() const { return cabeza_; }

private:
  NodoPendiente *cabeza_{nullptr};
};

#endif // HUBFLOW_LISTA_PENDIENTES_HPP