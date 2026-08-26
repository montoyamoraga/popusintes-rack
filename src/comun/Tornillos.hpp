#pragma once
#include "plugin.hpp"

// agrega los tornillos estandar a un ModuleWidget, en las esquinas
// el centro de cada tornillo queda a 1 HP del borde correspondiente
// paneles angostos (4 hp) solo alcanzan para una columna de
// tornillos: con dos columnas los agujeros quedarian casi pegados
inline void agregarTornillos(
    ModuleWidget *widget,
    bool unaColumna = false)
{
    const float MITAD_TORNILLO = RACK_GRID_WIDTH / 2.f;
    const float DESDE_IZQUIERDA = RACK_GRID_WIDTH - MITAD_TORNILLO;
    const float DESDE_DERECHA = widget->box.size.x -
                                 RACK_GRID_WIDTH - MITAD_TORNILLO;

    widget->addChild(
        createWidget<ScrewBlack>(
            Vec(DESDE_IZQUIERDA, 0)));

    widget->addChild(
        createWidget<ScrewBlack>(
            Vec(DESDE_IZQUIERDA,
                RACK_GRID_HEIGHT -
                    RACK_GRID_WIDTH)));

    if (unaColumna)
        return;

    widget->addChild(
        createWidget<ScrewBlack>(
            Vec(DESDE_DERECHA, 0)));

    widget->addChild(
        createWidget<ScrewBlack>(
            Vec(DESDE_DERECHA,
                RACK_GRID_HEIGHT -
                    RACK_GRID_WIDTH)));
}