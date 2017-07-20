#include "./window2.h"

void Window2::Init(const Ptr<Application> & app) {
    Window::Init(app);
    
    auto root_view = this->root_view();
    auto layouter = New<GridLayouter>();
    root_view->set_children_layouter(layouter);
    
    {
        GridLayouter::TrackDef def;
        def.set_fixed_value(Some(100));
        layouter->AddColumnDef(def);
    }
    {
        GridLayouter::TrackDef def;
        def.set_wrap_content(true);
        layouter->AddColumnDef(def);
    }
    {
        GridLayouter::TrackDef def;
        def.set_weight(Some(1.0f));
        layouter->AddColumnDef(def);
    }
    
    {
        GridLayouter::TrackDef def;
        def.set_fixed_value(Some(100));
        layouter->AddRowDef(def);
    }
    {
        GridLayouter::TrackDef def;
        def.set_weight(Some(2));
        layouter->AddRowDef(def);
    }
    {
        GridLayouter::TrackDef def;
        def.set_weight(Some(1));
        layouter->AddRowDef(def);
    }
    {
        GridLayouter::TrackDef def;
        def.set_fixed_value(Some(50));
        layouter->AddRowDef(def);
    }
    
    auto view1 = OLIVIEW_INIT(View, app);
    view1->set_background_color(Color(1, 0, 0, 1));
    root_view->AddChild(view1);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view1);
        def.set_row_position(MakeIndexRange(0, 1));
        def.set_column_position(MakeIndexRange(0, 1));
        layouter->AddItemAreaDef(def);
    }
    
    auto view2 = OLIVIEW_INIT(Label, app, "hello grid layouter ! この列の幅は私が決める");
    view2->set_background_color(Color(0.8, 0.9, 0.8, 1.0));
    root_view->AddChild(view2);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view2);
        def.set_row_position(MakeIndexRange(0, 1));
        def.set_column_position(MakeIndexRange(1, 2));
        layouter->AddItemAreaDef(def);
    }
    
    auto view3 = OLIVIEW_INIT(View, app);
    view3->set_background_color(Color(1, 1, 0, 1));
    root_view->AddChild(view3);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view3);
        def.set_row_position(MakeIndexRange(1, 2));
        def.set_column_position(MakeIndexRange(0, 1));
        layouter->AddItemAreaDef(def);
    }
    
    auto view4 = OLIVIEW_INIT(View, app);
    view4->set_background_color(Color(0, 1, 0, 1));
    root_view->AddChild(view4);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view4);
        def.set_row_position(MakeIndexRange(2, 3));
        def.set_column_position(MakeIndexRange(0, 1));
        layouter->AddItemAreaDef(def);
    }
    
    auto view5 = OLIVIEW_INIT(View, app);
    view5->set_background_color(Color(0, 1, 1, 1));
    root_view->AddChild(view5);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view5);
        def.set_row_position(MakeIndexRange(3, 4));
        def.set_column_position(MakeIndexRange(0, 3));
        layouter->AddItemAreaDef(def);
    }
    
    auto view6 = OLIVIEW_INIT(View, app);
    view6->set_background_color(Color(0, 0, 1, 1));
    root_view->AddChild(view6);
    {
        GridLayouter::ItemAreaDef def;
        def.set_item(view6);
        def.set_row_position(MakeIndexRange(1, 2));
        def.set_column_position(MakeIndexRange(2, 3));
        layouter->AddItemAreaDef(def);
    }
    
    
}

