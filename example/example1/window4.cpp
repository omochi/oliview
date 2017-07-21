#include "./window4.h"

void Window4::Init(const Ptr<Application> & app) {
    Window::Init(app);
    
    {
        auto view = OLIVIEW_INIT(ScrollView, app);
        scroll_view_ = view;
        content_view()->AddChild(view);
        view->content_view()->set_background_color(Color(1.0f, 0.8f, 0.8f, 1.0f));
        
        {
            auto vw = OLIVIEW_INIT(View, app);
            scroll_view_->content_view()->AddChild(vw);
            vw->set_frame(Rect(Vector2(0, 0), Size(100, 100)));
            vw->set_background_color(Color(0.8f, 0.4f, 0.4f, 1.0f));
        }
        
        {
            auto vw = OLIVIEW_INIT(View, app);
            scroll_view_->content_view()->AddChild(vw);
            vw->set_frame(Rect(Vector2(250, 0), Size(100, 100)));
            vw->set_background_color(Color(0.6f, 0.6f, 0.4f, 1.0f));
        }
        
        {
            auto vw = OLIVIEW_INIT(View, app);
            scroll_view_->content_view()->AddChild(vw);
            vw->set_frame(Rect(Vector2(100, 150), Size(100, 100)));
            vw->set_background_color(Color(0.4f, 0.8f, 0.4f, 1.0f));
        }
    }
}

void Window4::LayoutContentView(NVGcontext * ctx, const Ptr<WindowContentView> & view) {
    Window::LayoutContentView(ctx, view);
    
    scroll_view_->set_frame(view->bounds().InsetBy(EdgeInset(20, 20, 20, 20)));
}
