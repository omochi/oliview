#pragma once

#include "./dependency.h"

#include "./measure_query.h"
#include "./size.h"

namespace oliview {
    class View;
    
    class Layouter {
    public:
        virtual ~Layouter();
        
        Ptr<View> owner() const;
        
        virtual void Layout(NVGcontext * ctx) const = 0;
        virtual Size Measure(NVGcontext * ctx, const MeasureQuery & query) const = 0;
        
        void _set_owner(const Ptr<View> & owner);
    private:
        WeakPtr<View> owner_;
    };
}
