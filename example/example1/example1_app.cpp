#include "./example1_app.h"

void Example1App::OnInit() {
    OpenWindow1();
    OpenWindow2();
}

void Example1App::OpenWindow1() {
    auto app = shared_from_this();
    
    auto window = Window::Create(app);
    
    auto view1 = New<View>(app);
    window->root_view()->AddChild(view1);
    view1->set_frame(Rect(Vector2(20, 40),
                          Size(120, 60)));
    view1->set_background_color(Color(1, 0, 0, 1));
    
    auto view2 = New<View>(app);
    window->root_view()->AddChild(view2);
    view2->set_frame(Rect(Vector2(20, 120),
                          Size(100, 100)));
    view2->set_background_color(Color(0, 0, 1, 1));
    view2->set_clipping_children(true);
    
    auto view3 = New<View>(app);
    view2->AddChild(view3);
    view3->set_frame(Rect(Vector2(20, 20),
                          Size(140, 40)));
    view3->set_background_color(Color(0, 1, 0, 1));
    
    auto view4 = New<Label>(app, "明るい未来\n暗い過去");
    window->root_view()->AddChild(view4);
    view4->set_frame(Rect(Vector2(20, 440),
                          Size(59, 46)));
    view4->set_background_color(Color(1, 0, 0, 1));
}

void Example1App::OpenWindow2() {
    auto app = shared_from_this();
    
    auto window = Window::Create(app);
    
    auto root_view = window->root_view();
    auto layouter = New<GridLayouter>();
    root_view->set_layouter(layouter);
    
    std::vector<GridLayouter::TrackSizeDef> column_size_defs;
    {
        GridLayouter::TrackSizeDef def;
        def.set_fixed_value(Some(100));
        column_size_defs.push_back(def);
    }
    {
        GridLayouter::TrackSizeDef def;
        def.set_wrap_content(true);
        column_size_defs.push_back(def);
    }
    {
        GridLayouter::TrackSizeDef def;
        def.set_weight(Some(1.0f));
        column_size_defs.push_back(def);
    }
    layouter->set_column_size_defs(column_size_defs);
    
    std::vector<GridLayouter::TrackSizeDef> row_size_defs;
    {
        GridLayouter::TrackSizeDef def;
        def.set_fixed_value(Some(100));
        row_size_defs.push_back(def);
    }
    {
        GridLayouter::TrackSizeDef def;
        def.set_weight(Some(2));
        row_size_defs.push_back(def);
    }
    {
        GridLayouter::TrackSizeDef def;
        def.set_weight(Some(1));
        row_size_defs.push_back(def);
    }
    {
        GridLayouter::TrackSizeDef def;
        def.set_fixed_value(Some(50));
        row_size_defs.push_back(def);
    }
    layouter->set_row_size_defs(row_size_defs);
    
    std::vector<GridLayouter::ItemAreaDef> area_defs;
    
    auto view1 = New<View>(app);
    view1->set_background_color(Color(1, 0, 0, 1));
    root_view->AddChild(view1);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view1);
        def.set_row_position(MakeRange(0, 1));
        def.set_column_position(MakeRange(0, 1));
        area_defs.push_back(def);
    }
    
    auto view2 = New<Label>(app, "hello grid layouter");
    view2->set_background_color(Color(0.8, 0.9, 0.8, 1.0));
    root_view->AddChild(view2);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view2);
        def.set_row_position(MakeRange(0, 1));
        def.set_column_position(MakeRange(1, 2));
        area_defs.push_back(def);
    }

    auto view3 = New<View>(app);
    view3->set_background_color(Color(1, 1, 0, 1));
    root_view->AddChild(view3);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view3);
        def.set_row_position(MakeRange(1, 2));
        def.set_column_position(MakeRange(0, 1));
        area_defs.push_back(def);
    }
    
    auto view4 = New<View>(app);
    view4->set_background_color(Color(0, 1, 0, 1));
    root_view->AddChild(view4);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view4);
        def.set_row_position(MakeRange(2, 3));
        def.set_column_position(MakeRange(0, 1));
        area_defs.push_back(def);
    }
    
    auto view5 = New<View>(app);
    view5->set_background_color(Color(0, 1, 1, 1));
    root_view->AddChild(view5);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view5);
        def.set_row_position(MakeRange(3, 4));
        def.set_column_position(MakeRange(0, 3));
        area_defs.push_back(def);
    }

    auto view6 = New<View>(app);
    view6->set_background_color(Color(0, 0, 1, 1));
    root_view->AddChild(view6);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view6);
        def.set_row_position(MakeRange(1, 2));
        def.set_column_position(MakeRange(2, 3));
        area_defs.push_back(def);
    }
    
    layouter->set_item_area_defs(area_defs);
    
}