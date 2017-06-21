#pragma once

#include "./dependency.h"
#include "./font.h"
#include "./window.h"

namespace oliview {
    class Application : public std::enable_shared_from_this<Application> {
    public:
        Application();
        virtual ~Application();

        int Main(int argc, char * argv[]);

        virtual void OnInit();
        virtual void OnFinish();

        void AddWindowInternal(const Ptr<Window> & window);
        void RemoveWindowInternal(const Ptr<Window> & window);
    private:
        void Init();
        void Finish();

        void InitFont();

        std::vector<Ptr<Window>> windows_;
    };
}

