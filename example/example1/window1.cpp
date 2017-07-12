#include "./window1.h"

void Window1::Init(const Ptr<Application> & app) {
    Window::Init(app);
        
    auto view1 = OLIVIEW_INIT(View, app);
    root_view()->AddChild(view1);
    view1->set_frame(Rect(Vector2(20, 40),
                          Size(120, 60)));
    view1->set_background_color(Color(1, 0, 0, 1));
    
    auto view2 = OLIVIEW_INIT(View, app);
    root_view()->AddChild(view2);
    view2->set_frame(Rect(Vector2(20, 120),
                          Size(100, 100)));
    view2->set_background_color(Color(0, 0, 1, 1));
    view2->set_clipping_children(true);
    
    auto view3 = OLIVIEW_INIT(View, app);
    view2->AddChild(view3);
    view3->set_frame(Rect(Vector2(20, 20),
                          Size(140, 40)));
    view3->set_background_color(Color(0, 1, 0, 1));
    
//    auto view4 = OLIVIEW_INIT(Label, app, "明るい未来\n暗い過去");
//    root_view()->AddChild(view4);
//    view4->set_frame(Rect(Vector2(20, 440),
//                          Size(59, 46)));
//    view4->set_background_color(Color(1, 0, 0, 1));

    auto view5 = OLIVIEW_INIT(TextBox, app);
    view5->set_background_color(Color(0.8, 0.7, 0.5, 1.0));
    root_view()->AddChild(view5);
    view5->set_frame(Rect(Vector2(200, 50),
                          Size(300, 200)));
    view5->set_text("初期テキスト\nお肉\n\n↑空行");
}
