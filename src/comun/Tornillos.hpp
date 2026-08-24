#pragma once
#include "plugin.hpp"

// agrega los 4 tornillos estandar a un
// ModuleWidget, en las esquinas
// el centro de cada tornillo queda a
// 1 HP del borde correspondiente
inline void agregarTornillos(
    ModuleWidget *widget)
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
            Vec(DESDE_DERECHA, 0)));

    widget->addChild(
        createWidget<ScrewBlack>(
            Vec(DESDE_IZQUIERDA,
                RACK_GRID_HEIGHT -
                    RACK_GRID_WIDTH)));

    widget->addChild(
        createWidget<ScrewBlack>(
            Vec(DESDE_DERECHA,
                RACK_GRID_HEIGHT -
                    RACK_GRID_WIDTH)));
}