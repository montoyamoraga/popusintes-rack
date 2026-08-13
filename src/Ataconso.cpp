#include "Popusintes.hpp"

// interacciones entre dos chips 555
// el primer chip está en modo astable
// el segundo chip está en modo monostable

// modulo
struct AtaconsoModulo : Module
{
    enum ParamIds
    {
        PARAM_FRECUENCIA,
        PARAM_TIMBRE,
        NUM_PARAMS,
    };

    enum InputIds
    {
        ENTRADA_FRECUENCIA,
        ENTRADA_TIMBRE,
        NUM_INPUTS,
    };

    enum OutputIds
    {
        SALIDA_ATACONSO,
        NUM_OUTPUTS,
    };

    enum LightsIds
    {
        LUZ_SALIDA,
        NUM_LIGHTS,
    };

    // rango de la perilla de frecuencia
    static constexpr float FRECUENCIA_OCTAVAS_MIN = -4.0f;
    static constexpr float FRECUENCIA_OCTAVAS_MAX = 4.0f;

    // rango de la perilla de timbre, ancho del pulso del oscilador 2
    static constexpr float TIMBRE_MS_MIN = 0.1f;
    static constexpr float TIMBRE_MS_MAX = 20.0f;

    // fase acumulada del oscilador 1
    float fase = 0.f;

    // generador de pulso del oscilador 2
    dsp::PulseGenerator generadorPulso;

    AtaconsoModulo()
    {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

        configParam(PARAM_FRECUENCIA, FRECUENCIA_OCTAVAS_MIN, FRECUENCIA_OCTAVAS_MAX, 0.0f, "frecuencia", " oct");
        configParam(PARAM_TIMBRE, TIMBRE_MS_MIN, TIMBRE_MS_MAX, 5.0f, "timbre", " ms");

        configInput(ENTRADA_FRECUENCIA, "cv frecuencia");
        configInput(ENTRADA_TIMBRE, "cv timbre");

        configOutput(SALIDA_ATACONSO, "ataconso");
    }

    void process(const ProcessArgs &args) override
    {
        // oscilador 1 (astable): avanza la fase y dispara
        // cv de frecuencia abstracto, no v/oct: solo suma alcance a la perilla
        float altura = params[PARAM_FRECUENCIA].getValue();
        altura = altura + inputs[ENTRADA_FRECUENCIA].getVoltage() * ((FRECUENCIA_OCTAVAS_MAX - FRECUENCIA_OCTAVAS_MIN) / 10.f);

        if (altura > FRECUENCIA_OCTAVAS_MAX)
        {
            altura = FRECUENCIA_OCTAVAS_MAX;
        }
        else if (altura < FRECUENCIA_OCTAVAS_MIN)
        {
            altura = FRECUENCIA_OCTAVAS_MIN;
        }

        float frecuencia = dsp::FREQ_C4 * std::pow(2.0f, altura);

        fase = fase + frecuencia * args.sampleTime;

        bool disparo = fase >= 1.0f;

        if (disparo)
        {
            fase = fase - std::floor(fase);
        }

        // oscilador 2 monoestable: ancho de pulso en ms, con cv sumado
        float timbreMs = params[PARAM_TIMBRE].getValue();
        timbreMs = timbreMs + inputs[ENTRADA_TIMBRE].getVoltage() * (TIMBRE_MS_MAX / 10.f);

        if (timbreMs > TIMBRE_MS_MAX)
        {
            timbreMs = TIMBRE_MS_MAX;
        }
        else if (timbreMs < TIMBRE_MS_MIN)
        {
            timbreMs = TIMBRE_MS_MIN;
        }

        // el 555 monoestable real es no-reentrante: su pin de disparo es sensible a nivel,
        // asi que un disparo mientras el pulso sigue activo se ignora en vez de extenderlo.
        // sin este chequeo, cuando timbre es mayor al periodo del oscilador 1
        // el pulso se re-extiende en cada disparo y la salida se queda pegada en alto (dc, sin sonido)
        // en vez de completar su propio ciclo y volver a dispararse: el drone caracteristico de la apc
        if (disparo && generadorPulso.remaining <= 0.f)
        {
            generadorPulso.trigger(timbreMs * 1e-3f);
        }

        float salida = generadorPulso.process(args.sampleTime);

        // onda rectangular, bipolar, entre -5v y +5v
        outputs[SALIDA_ATACONSO].setVoltage(salida > 0.f ? 5.0f : -5.0f);
        lights[LUZ_SALIDA].setSmoothBrightness(salida, args.sampleTime);
    }
};

namespace layout
{
    // columna unica, centrada: ataconso no tiene canales a/b
    constexpr float PORCENTAJE_COLUMNA = 0.5f;

    // coordenadas frecuencia
    constexpr float PORCENTAJE_FRECUENCIA_X = PORCENTAJE_COLUMNA;
    constexpr float PORCENTAJE_FRECUENCIA_Y = 0.20f;
    constexpr float PORCENTAJE_ENTRADA_FRECUENCIA_X = PORCENTAJE_COLUMNA;
    constexpr float PORCENTAJE_ENTRADA_FRECUENCIA_Y = PORCENTAJE_FRECUENCIA_Y + espaciado::DELTA_Y_BOTON_ENTRADA;

    // coordenadas timbre
    constexpr float PORCENTAJE_TIMBRE_X = PORCENTAJE_COLUMNA;
    constexpr float PORCENTAJE_TIMBRE_Y = 0.45f;
    constexpr float PORCENTAJE_ENTRADA_TIMBRE_X = PORCENTAJE_COLUMNA;
    constexpr float PORCENTAJE_ENTRADA_TIMBRE_Y = PORCENTAJE_TIMBRE_Y + espaciado::DELTA_Y_BOTON_ENTRADA;

    // coordenadas salida y luz
    constexpr float PORCENTAJE_SALIDA_X = PORCENTAJE_COLUMNA;
    constexpr float PORCENTAJE_SALIDA_Y = 0.90f;
    constexpr float PORCENTAJE_LUZ_X = PORCENTAJE_COLUMNA;
    constexpr float PORCENTAJE_LUZ_Y = PORCENTAJE_SALIDA_Y - espaciado::DELTA_Y_SALIDA_LUZ;
}

// widget
struct AtaconsoModuloWidget : ModuleWidget
{
    AtaconsoModuloWidget(AtaconsoModulo *modulo)
    {
        setModule(modulo);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AtaconsoModulo.svg")));

        // tornillos
        agregarTornillos(this);

        Posicionador posicionador(dimensiones::ATACONSO_ANCHO, dimensiones::ATACONSO_ALTURA);

        // perillas y entradas
        addParam(createParamCentered<RoundBlackKnob>(
            posicionador.posicion(layout::PORCENTAJE_FRECUENCIA_X, layout::PORCENTAJE_FRECUENCIA_Y),
            modulo, AtaconsoModulo::PARAM_FRECUENCIA));

        addInput(createInputCentered<PJ301MPort>(
            posicionador.posicion(layout::PORCENTAJE_ENTRADA_FRECUENCIA_X, layout::PORCENTAJE_ENTRADA_FRECUENCIA_Y),
            modulo, AtaconsoModulo::ENTRADA_FRECUENCIA));

        addParam(createParamCentered<RoundBlackKnob>(
            posicionador.posicion(layout::PORCENTAJE_TIMBRE_X, layout::PORCENTAJE_TIMBRE_Y),
            modulo, AtaconsoModulo::PARAM_TIMBRE));

        addInput(createInputCentered<PJ301MPort>(
            posicionador.posicion(layout::PORCENTAJE_ENTRADA_TIMBRE_X, layout::PORCENTAJE_ENTRADA_TIMBRE_Y),
            modulo, AtaconsoModulo::ENTRADA_TIMBRE));

        // salida y luz
        addOutput(createOutputCentered<PJ301MPort>(
            posicionador.posicion(layout::PORCENTAJE_SALIDA_X, layout::PORCENTAJE_SALIDA_Y),
            modulo, AtaconsoModulo::SALIDA_ATACONSO));

        addChild(createLightCentered<MediumLight<GreenLight>>(
            posicionador.posicion(layout::PORCENTAJE_LUZ_X, layout::PORCENTAJE_LUZ_Y),
            modulo, AtaconsoModulo::LUZ_SALIDA));
    }
};

Model *modeloAtaconso = createModel<AtaconsoModulo, AtaconsoModuloWidget>("ataconso");
