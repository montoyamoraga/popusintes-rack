#include "Popusintes.hpp"

// modulo
struct CompaModule : Module
{
    enum ParamIds
    {
        NUM_PARAMS,
    };

    enum InputIds
    {
        ENTRADA_A,
        ENTRADA_B,
        NUM_INPUTS,
    };

    enum OutputIds
    {
        SALIDA_COMPARADOR,
        NUM_OUTPUTS,
    };

    enum LightsIds
    {
        LUZ_SALIDA,
        NUM_LIGHTS,
    };

    CompaModule()
    {

        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    }

    void process(const ProcessArgs &args) override
    {
        // detectar si entrada A y entrada B tienen conexion
        if (inputs[ENTRADA_A].isConnected() && inputs[ENTRADA_B].isConnected())
        {
            // comparar si
            // voltaje en entrada A es mayor que en entrada B
            float salida = inputs[ENTRADA_A].getVoltage() > inputs[ENTRADA_B].getVoltage();

            // asignar ese voltaje a la salida
            // multiplicar por 10.f
            outputs[SALIDA_COMPARADOR].setVoltage(10.f * salida);

            // asignar ese voltaje a la luz
            lights[LUZ_SALIDA].setBrightness(salida);
        }
        else
        {
            outputs[SALIDA_COMPARADOR].setVoltage(0.f);
            lights[LUZ_SALIDA].setBrightness(0.f);
        }
    }
};

// widget
struct CompaModuleWidget : ModuleWidget
{
    CompaModuleWidget(CompaModule *module)
    {

        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CompaModule.svg")));

        // tornillos
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        // entradas
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.4 * 0.20, 128.4 * 0.50)), module, CompaModule::ENTRADA_A));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.4 * 0.80, 128.4 * 0.50)), module, CompaModule::ENTRADA_B));

        //  salidas
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.4 * 0.5, 128.4 * 0.80)), module, CompaModule::SALIDA_COMPARADOR));

        // luces
        addChild(createLightCentered<LargeLight<GreenLight>>(mm2px(Vec(25.4 * 0.5, 128.4 * 0.70)), module, CompaModule::LUZ_SALIDA));
    }
};

Model *modelCompa = createModel<CompaModule, CompaModuleWidget>("compa");