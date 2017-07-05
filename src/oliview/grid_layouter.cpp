#include "./grid_layouter.h"

namespace oliview {
    
    GridLayouter::Dimension GridLayouter::RotateDimension(GridLayouter::Dimension dimension) {
        switch (dimension) {
            case Dimension::Row:
                return Dimension::Column;
            case Dimension::Column:
                return Dimension::Row;
        }
        RHETORIC_FATAL("never");
    }
    
    Ptr<View> GridLayouter::ItemAreaDef::item() const {
        return item_.lock();
    }
    
    void GridLayouter::ItemAreaDef::set_item(const Ptr<View> & value) {
        item_ = value;
    }
    
    Range<int> GridLayouter::ItemAreaDef::GetPositionFor(GridLayouter::Dimension dimension) const {
        switch (dimension) {
            case Dimension::Row:
                return row_position();
            case Dimension::Column:
                return column_position();
        }
        RHETORIC_FATAL("never");
    }
    
    void GridLayouter::set_row_size_defs(const std::vector<TrackSizeDef> & value) {
        row_size_defs_ = value;
        SetNeedsLayout();
    }
    
    void GridLayouter::set_column_size_defs(const std::vector<TrackSizeDef> & value) {
        column_size_defs_ = value;
        SetNeedsLayout();
    }
    
    void GridLayouter::Layout(NVGcontext * ctx) const {
        auto view = this->view();
        if (!view) { return; }
        
        MeasureQuery query;
        query.set_max_width(Some(view->frame().size().width()));
        query.set_max_height(Some(view->frame().size().height()));
        auto layout = ResolveLayout(ctx, query);
        
        CalcLayoutResult(&layout);
        
        std::vector<ItemAreaDef> new_area_defs;
        for (auto & def : item_area_defs_) {
            auto item = def.item();
            if (item == nullptr) { continue; }
            new_area_defs.push_back(def);
            
            float top = layout.top_list[def.row_position().lower_bound()];
            float bottom = layout.top_list[def.row_position().upper_bound()];
            float left = layout.left_list[def.column_position().lower_bound()];
            float right = layout.left_list[def.column_position().upper_bound()];
            
            item->set_frame(Rect(Vector2(left, top),
                                 Size(right - left, bottom - top)));
        }
        item_area_defs_ = new_area_defs;
    }
    
    Size GridLayouter::Measure(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = ResolveLayout(ctx, query);
        
        Size ret;
        for (auto x : layout.row_sizes) {
            ret.set_height(ret.height() + x);
        }
        for (auto x : layout.column_sizes) {
            ret.set_width(ret.width() + x);
        }
        return ret;
    }
    
    std::vector<GridLayouter::TrackSizeDef> GridLayouter::GetSizeDefsFor(Dimension dimension) const {
        switch (dimension) {
            case Dimension::Row:
                return row_size_defs_;
            case Dimension::Column:
                return column_size_defs_;
        }
        RHETORIC_FATAL("never");
    }
    
    GridLayouter::LayoutResult GridLayouter::ResolveLayout(NVGcontext * ctx,
                                                           const MeasureQuery & query) const {
        GridLayouter::LayoutResult ret;
        
        std::vector<Dimension> dimensions = { Dimension::Row, Dimension::Column };
        
        for (auto dimension : dimensions) {
            std::vector<TrackSizeDef> size_defs = GetSizeDefsFor(dimension);
            std::vector<float> ret_sizes;
            
            float weight_sum = 0.0f;
            for (int i = 0; i < (int)size_defs.size(); i++) {
                auto def = size_defs[i];
                
                float row_height = 0.0f;
                
                if (def.fixed_value()) {
                    row_height = *def.fixed_value();
                } else if (def.weight()) {
                    weight_sum += *def.weight();
                } else if (def.wrap_content()) {
                    auto areas = GetSingleAreas(dimension, i);
                    for (auto & area : areas) {
                        auto item = area.item();
                        if (item == nullptr) { continue; }
                        
                        MeasureQuery item_query;
                        item_query.set_max_width(None());
                        item_query.set_max_height(None());
                        auto item_size = item->Measure(ctx, item_query);
                        row_height = std::max(row_height, item_size.height());
                    }
                }
                
                if (def.max_value()) {
                    row_height = std::min(row_height, *def.max_value());
                }
                
                ret_sizes.push_back(row_height);
            }
            
            float free_height = 0.0f;
            
            if (query.max_height()) {
                free_height = *query.max_height();
                for (auto h : ret_sizes) {
                    free_height -= h;
                }
                free_height = std::max(0.0f, free_height);
            }
            
            for (int i = 0; i < (int)size_defs.size(); i++) {
                auto def = size_defs[i];
                if (def.weight()) {
                    float weight_rate = *def.weight() / weight_sum;
                    ret_sizes[i] = free_height * weight_rate;
                }
            }
            
            switch (dimension) {
                case Dimension::Row:
                    ret.row_sizes = ret_sizes;
                    break;
                case Dimension::Column:
                    ret.column_sizes = ret_sizes;
                    break;
            }
        }
        
        return ret;
    }
    
    void GridLayouter::CalcLayoutResult(LayoutResult * layout) const {
        float top = 0.0f;
        layout->top_list.push_back(top);
        for (auto x : layout->row_sizes) {
            top += x;
            layout->top_list.push_back(top);
        }
        
        float left = 0.0f;
        layout->left_list.push_back(left);
        for (auto x : layout->column_sizes) {
            left += x;
            layout->left_list.push_back(left);
        }
    }
    
    std::vector<GridLayouter::ItemAreaDef>
    GridLayouter::GetSingleAreas(Dimension dimension,
                                 int index) const
    {
        std::vector<GridLayouter::ItemAreaDef> ret;
        for (auto & def : item_area_defs_) {
            auto position = def.GetPositionFor(dimension);
            if (position.lower_bound() == index &&
                position.upper_bound() == index + 1)
            {
                ret.push_back(def);
            }
        }
        return ret;
    }
}
