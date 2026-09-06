#ifndef HUBFLOW_NIVEL_SERVICIO_HPP
#define HUBFLOW_NIVEL_SERVICIO_HPP

enum class NivelServicio
{
    EXPRESS = 0,
    PRIORITARIO = 1,
    ESTANDAR = 2
};

inline bool tieneMayorPrioridad(NivelServicio a, NivelServicio b)
{
    return static_cast<int>(a) < static_cast<int>(b);
}

#endif // HUBFLOW_NIVEL_SERVICIO_HPP