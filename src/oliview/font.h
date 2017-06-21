#pragma once

#include "./dependency.h"

namespace oliview {
    class Font : public std::enable_shared_from_this<Font> {
    public:
        Font(NVGcontext * nvg_context,
             int nvg_handle,
             std::string name,
             const Ptr<const Data> & data);
        virtual ~Font();

        RHETORIC_GETTER(NVGcontext *, nvg_context)
        RHETORIC_GETTER(int, nvg_handle)
        RHETORIC_GETTER(std::string, name)
        RHETORIC_GETTER(Ptr<const Data>, data)
    private:
        NVGcontext * nvg_context_;
        int nvg_handle_;
        std::string name_;
        Ptr<const Data> data_;
    };
}
