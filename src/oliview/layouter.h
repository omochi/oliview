#pragma once

#include "./dependency.h"

#include "./measure_query.h"
#include "./size.h"

namespace oliview {
    class View;
    
    class Layouter {
    public:
        virtual ~Layouter();
        
        Ptr<View> view() const;
        
        virtual void Layout(NVGcontext * ctx) const = 0;
        virtual Size Measure(NVGcontext * ctx, const MeasureQuery & query) const = 0;
        
        void SetNeedsLayout() const;
        
        void _set_view(const Ptr<View> & value);
    private:
        WeakPtr<View> view_;
    };
}
