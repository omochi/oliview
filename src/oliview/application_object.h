#pragma once

#include "./dependency.h"

namespace oliview {
    class Application;
    
    class ApplicationObject :
    public std::enable_shared_from_this<ApplicationObject>
    {
    public:
        ApplicationObject();
        ApplicationObject(const ApplicationObject & other) = delete;
        ApplicationObject & operator=(const ApplicationObject & other) = delete;
        virtual void Init(const Ptr<Application> & application);
        
        virtual ~ApplicationObject();
        
        Ptr<Application> application() const;
    private:
        WeakPtr<Application> application_;
    };
}
