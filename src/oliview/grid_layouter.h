#pragma once

#include "./layouter.h"
#include "./table_dimension.h"
#include "./view.h"

namespace oliview {
    class GridLayouter : public Layouter {
    public:
        class TrackSizeDef {
        public:
            RHETORIC_ACCESSOR(Optional<float>, fixed_value)
            RHETORIC_ACCESSOR(Optional<float>, weight)
            RHETORIC_ACCESSOR_TRIVIAL(bool, wrap_content)
            RHETORIC_ACCESSOR(Optional<float>, max_value)
        private:
            Optional<float> fixed_value_;
            Optional<float> weight_;
            bool wrap_content_;
            Optional<float> max_value_;
        };
        
        class ItemAreaDef {
        public:
            Ptr<View> item() const;
            void set_item(const Ptr<View> & value);
            RHETORIC_ACCESSOR(Range<int>, row_position)
            RHETORIC_ACCESSOR(Range<int>, column_position)
            
            Range<int> GetPositionFor(TableDimension dimension) const;
        private:
            WeakPtr<View> item_;
            Range<int> row_position_;
            Range<int> column_position_;
        };
        
        RHETORIC_GETTER(std::vector<TrackSizeDef>, row_size_defs);
        void set_row_size_defs(const std::vector<TrackSizeDef> & value);
        
        RHETORIC_GETTER(std::vector<TrackSizeDef>, column_size_defs);
        void set_column_size_defs(const std::vector<TrackSizeDef> & value);
        
        RHETORIC_GETTER(std::vector<ItemAreaDef>, item_area_defs);
        void set_item_area_defs(const std::vector<ItemAreaDef> & value);
        
        virtual void Layout(NVGcontext * ctx) const override;
        virtual Size Measure(NVGcontext * ctx, const MeasureQuery & query) const override;
    private:
        struct LayoutResult {
            std::vector<float> row_sizes;
            std::vector<float> column_sizes;
            
            //
            std::vector<float> top_list;
            std::vector<float> left_list;
        };
        
        std::vector<TrackSizeDef> GetSizeDefsFor(TableDimension dimension) const;
        
        LayoutResult ResolveLayout(NVGcontext * ctx,
                                   const MeasureQuery & query) const;
        void CalcLayoutResult(LayoutResult * layout) const;
        
        std::vector<ItemAreaDef> GetSingleAreas(TableDimension dimension,
                                                int index) const;
        
        std::vector<TrackSizeDef> row_size_defs_;
        std::vector<TrackSizeDef> column_size_defs_;
        
        mutable std::vector<ItemAreaDef> item_area_defs_;
    };
}
