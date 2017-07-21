#include "./window3.h"

void Window3::Init(const Ptr<Application> & app) {
    Window::Init(app);
    
    auto root_view = this->content_view();
    
    Ptr<View> red_group;
    Ptr<View> green_group;
    
    {
        auto view = OLIVIEW_INIT(View, app);
        red_group = view;
        view->set_background_color(Color(1.0f, 0.6f, 0.6f, 0.7f));
        root_view->AddChild(view);
        view->set_frame(Rect(Vector2(20, 20), Size(200, 300)));
        view->set_focusable(true);
    }
    
    {
        auto view = OLIVIEW_INIT(View, app);
        view->set_background_color(Color(0.6f, 0.3f, 0.3f, 0.5f));
        red_group->AddChild(view);
        view->set_frame(Rect(Vector2(20, 20), Size(160, 100)));
        view->set_focusable(true);
    }
    
    {
        auto view = OLIVIEW_INIT(View, app);
        view->set_background_color(Color(0.6f, 0.3f, 0.3f, 0.5f));
        red_group->AddChild(view);
        view->set_frame(Rect(Vector2(20, 140), Size(160, 100)));
        view->set_focusable(true);
    }
    
    {
        auto view = OLIVIEW_INIT(View, app);
        green_group = view;
        view->set_background_color(Color(0.6f, 1.0f, 0.6f, 0.7f));
        root_view->AddChild(view);
        view->set_frame(Rect(Vector2(160, 80), Size(200, 300)));
    }
    
    {
        auto view = OLIVIEW_INIT(View, app);
        view->set_background_color(Color(0.3f, 0.6f, 0.3f, 0.5f));
        green_group->AddChild(view);
        view->set_frame(Rect(Vector2(20, 20), Size(160, 100)));
        view->set_focusable(true);
    }
    
    {
        auto view = OLIVIEW_INIT(View, app);
        view->set_background_color(Color(0.3f, 0.6f, 0.3f, 0.5f));
        green_group->AddChild(view);
        view->set_frame(Rect(Vector2(20, 140), Size(160, 100)));
        view->set_focusable(true);
    }
    
    {
        auto view = OLIVIEW_INIT(View, app);
        view->set_background_color(Color(0.6f, 0.6f, 1.0f, 0.8f));
        root_view->AddChild(view);
        view->set_frame(Rect(Vector2(70, 240), Size(180, 120)));
        view->set_focusable(true);
    }
    
    
}

