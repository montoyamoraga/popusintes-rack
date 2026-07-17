#ifndef FILTRO_H
#define FILTRO_H

class Filtro
{
private:
    float b0, b1, b2, a1, a2;
    float xMem1, xMem2, yMem1, yMem2;

public:
    Filtro();
    ~Filtro();
    void definirCoeficientes(int tipo, float corte);
    float procesar(float entrada)
}

#endif