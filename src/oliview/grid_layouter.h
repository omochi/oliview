#pragma once

#include "./layouter.h"
#include "./table_dimension.h"
#include "./view.h"

namespace oliview {
    class GridLayouter : public Layouter {
    public:
        class TrackDef {
        public:
            RHETORIC_ACCESSOR(Option<float>, fixed_value)
            RHETORIC_ACCESSOR(Option<float>, weight)
            RHETORIC_ACCESSOR_TRIVIAL(bool, wrap_content)
            RHETORIC_ACCESSOR(Option<float>, max_value)
        private:
            Option<float> fixed_value_;
            Option<float> weight_;
            bool wrap_content_;
            Option<float> max_value_;
        };
        
        class ItemAreaDef {
        public:
            Ptr<View> item() const;
            void set_item(const Ptr<View> & value);
            RHETORIC_ACCESSOR(Range<size_t>, row_position)
            RHETORIC_ACCESSOR(Range<size_t>, column_position)
            
            Range<size_t> GetPositionFor(TableDimension dimension) const;
        private:
            WeakPtr<View> item_;
            Range<size_t> row_position_;
            Range<size_t> column_position_;
        };
        
        RHETORIC_GETTER(std::vector<TrackDef>, row_defs);
        void set_row_defs(const std::vector<TrackDef> & value);
        void AddRowDef(const TrackDef & value);
        
        RHETORIC_GETTER(std::vector<TrackDef>, column_defs);
        void set_column_defs(const std::vector<TrackDef> & value);
        void AddColumnDef(const TrackDef & value);
        
        RHETORIC_GETTER(std::vector<ItemAreaDef>, item_area_defs);
        void set_item_area_defs(const std::vector<ItemAreaDef> & value);
        void AddItemAreaDef(const ItemAreaDef & value);
        
        virtual void Layout(NVGcontext * ctx) const override;
        virtual Size Measure(NVGcontext * ctx, const MeasureQuery & query) const override;
    private:
        struct LayoutResult {
            std::vector<float> row_sizes;
            std::vector<float> column_sizes;
            std::vector<float> top_list;
            std::vector<float> left_list;
        };
        
        std::vector<TrackDef> GetTrackDefsFor(TableDimension dimension) const;
        
        LayoutResult ResolveLayout(NVGcontext * ctx,
                                   const MeasureQuery & query) const;
        void CalcLayoutResult(LayoutResult * layout) const;
        
        std::vector<ItemAreaDef> GetSingleAreas(TableDimension dimension,
                                                size_t index) const;
        
        std::vector<TrackDef> row_defs_;
        std::vector<TrackDef> column_defs_;
        
        mutable std::vector<ItemAreaDef> item_area_defs_;
    };
}
