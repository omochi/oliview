#pragma once

#include "./dependency.h"

#include "./measure_query.h"
#include "./size.h"

namespace oliview {
    class View;
    
    class Layouter {
    public:
        Ptr<View> owner() const;
        
        virtual void Layout() const = 0;
        virtual Size Measure(const MeasureQuery & query) const = 0;
        
        void _set_owner(const Ptr<View> & owner);
    private:
        WeakPtr<View> owner_;
    };
}
