#include "./window1.h"

void Window1::Init(const Ptr<Application> & app) {
    Window::Init(app);
    
    test_stage_ = 0;
    
    auto content_view = OLIVIEW_INIT(Window1View, app);
    content_view->set_background_color(Color(1.0, 1.0, 0.9, 1.0));
    content_view->set_frame(Rect(Vector2(0, 0), Size(400, 400)));
    root_view()->AddChild(content_view);
    
    {
        auto layouter = New<GridLayouter>();
        root_view()->set_children_layouter(layouter);
        
        {
            GridLayouter::TrackDef def;
            def.set_weight(Some(1));
            layouter->AddRowDef(def);
        }
        
        {
            GridLayouter::TrackDef def;
            def.set_weight(Some(1));
            layouter->AddColumnDef(def);
        }
        
        {
            GridLayouter::ItemAreaDef def;
            def.set_item(content_view);
            def.set_row_position(MakeIndexRange(0, 1));
            def.set_column_position(MakeIndexRange(0, 1));
            layouter->AddItemAreaDef(def);
        }
    }
    
    auto view1 = OLIVIEW_INIT(View, app);
    content_view->AddChild(view1);
    view1->set_frame(Rect(Vector2(20, 40),
                          Size(120, 60)));
    view1->set_background_color(Color(1, 0, 0, 1));
    
    auto view2 = OLIVIEW_INIT(View, app);
    content_view->AddChild(view2);
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
//    content_view->AddChild(view4);
//    view4->set_frame(Rect(Vector2(20, 440),
//                          Size(59, 46)));
//    view4->set_background_color(Color(1, 0, 0, 1));

    auto view5 = OLIVIEW_INIT(TextBox, app);
    text_box_ = view5;
    view5->set_background_color(Color(0.8, 0.7, 0.5, 1.0));
    content_view->AddChild(view5);
    view5->set_frame(Rect(Vector2(200, 50),
                          Size(300, 200)));
    view5->set_text(New<Text>("初期テキスト\nお肉gg\n\n↑空行"));
    
    {
        auto view = OLIVIEW_INIT(ScrollBar, app, Axis::Y);
        view->set_content_size(1000);
        view->set_visible_size(100);
        view->ScrollTo(300);
        content_view->AddChild(view);
        view->set_frame(Rect(Vector2(500, 50), Size(15, 200)));
    }
}

void Window1::OnKeyDownEvent(const KeyEvent & event) {
    if (event.type() == KeyEventType::Down) {
        if (event.key() == GLFW_KEY_N) {
            test_stage_ += 1;
        } else if (event.key() == GLFW_KEY_T) {
            switch (test_stage_) {
                case 0: {
//                    text_box_->InsertTextAt(Text::Index(0, 9), New<Text>("砂漠"), nullptr);
                    break;
                }
                case 1: {
//                    text_box_->InsertTextAt(Text::Index(0, 9), New<Text>("１行\n２行\n３行"), nullptr);
                    break;
                }
                case 2: {
//                    text_box_->DeleteTextAt(Text::Index(2, 0), Text::Index(4, 0));
                    break;
                }
                case 3: {
                    
                    break;
                }
            }
            
            
        }
    }
}
