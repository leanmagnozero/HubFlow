#ifndef HUBFLOW_NIVEL_SERVICIO_HPP
#define HUBFLOW_NIVEL_SERVICIO_HPP

/**
 * @brief Niveles de servicio de un envío
 *
 * El valor numérico del tipo de dato “enum” define el orden de prioridad:
 * Un menor valor indica una prioridad mayor.
 */
enum class NivelServicio
{
    EXPRESS = 0,
    PRIORITARIO = 1,
    ESTANDAR = 2
};

/**
 * @brief hace una comparación entre el nivel de servicio(la prioridad) de dos envios
 * @param a Nivel de servicio a evaluar
 * @param b Nivel de servicio contra el cual se compara.
 * @return true si @p a tiene estrictamente mayor prioridad que @p b.
 */
inline bool tieneMayorPrioridad(NivelServicio a, NivelServicio b)
{
    return static_cast<int>(a) < static_cast<int>(b);
}

#endif // HUBFLOW_NIVEL_SERVICIO_HPP