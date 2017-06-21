#include "./font.h"

namespace oliview {

    Font::Font(NVGcontext * nvg_context,
               int nvg_handle,
               std::string name,
               const Ptr<const Data> & data):
    nvg_context_(nvg_context),
    nvg_handle_(nvg_handle),
    name_(name),
    data_(data)
    {        
    }

}

