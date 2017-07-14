#include "./application_object.h"

#include "./application.h"

namespace oliview {
    ApplicationObject::ApplicationObject() {}
    
    void ApplicationObject::Init(const Ptr<Application> & application)
    {
        application_ = application;
    }

    ApplicationObject::~ApplicationObject()
    {}

    Ptr<Application> ApplicationObject::application() const {
        return application_.lock();
    }
    
}
