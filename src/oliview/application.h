#pragma once

#include "./dependency.h"
#include "./font_manager.h"
#include "./window.h"

namespace oliview {
    class Application : public std::enable_shared_from_this<Application> {
    public:
        Application();
        virtual ~Application();

        int Main(int argc, char * argv[]);

        virtual void OnInit() {}
        virtual void OnFinish() {}
        
        RHETORIC_GETTER(NVGcontext *, nvg_context)
        RHETORIC_GETTER(Ptr<FontManager>, font_manager)
        
        void _AddWindow(const Ptr<Window> & window);
        void _RemoveWindow(const Ptr<Window> & window);
        Ptr<Window> _shared_context_window() const;
        
    private:
        void Init();
        void Finish();
        
        void InitNVGContext();
        Result<None> _InitNVGContext();
        void DestroyNVGContext();

        std::vector<Ptr<Window>> windows_;
        NVGcontext * nvg_context_;
        
        Ptr<FontManager> font_manager_;
    };
}

