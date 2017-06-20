#pragma once

#include "./dependency.h"
#include "./window.h"

namespace oliview {
    class Application : public std::enable_shared_from_this<Application> {
    public:
        Application();
        virtual ~Application();

        int Main(int argc, char * argv[]);

        virtual void OnInit();
        virtual void OnFinish();
        
        NVGcontext * nvg_context() const;

        void _AddWindow(const Ptr<Window> & window);
        void _RemoveWindow(const Ptr<Window> & window);
        Ptr<Window> _shared_context_window() const;
        NVGcontext * _nvg_context() const;
        void _set_nvg_context(NVGcontext * value);
        
    private:
        void Init();
        void Finish();

        std::vector<Ptr<Window>> windows_;
        NVGcontext * nvg_context_;
    };
}

