#include "Panel.hpp"

Plugin *pluginInstance;

void init(Plugin *p)
{
    pluginInstance = p;

    p->addModel(modelPanel);
}
