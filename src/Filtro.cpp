#include "Filtro.h"

// y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]

Filtro::Filtro()
{
}

Filtro::~Filtro()
{
}

void definirCoeficientes(int tipo, float corte)
{
}

float Filtro::procesar(float entrada)
{
    float acumulador;
    float salida;

    // primero sumar las memorias de hace 2 muestras
    sumas = b2 * xMem2 - a2 * yMem2;
    // luego sumar las memorias de hace 1 muestra
    sumas += b1 * xMem1 - a1 * yMem1;

    // grabar las memorias de hace 1 muestra,
    // en la de hace 2 muestras
    xMem2 = xMem1;
    yMem2 = yMem1;

    // grabar los valores actuales actuales,
    /// en las memorias de hace 1 muestra

    xMem1 = entrada;
    yMem1 = salida;
}