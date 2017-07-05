#include "./layouter.h"

#include "./view.h"

namespace oliview {
    Layouter::~Layouter() {}
    
    Ptr<View> Layouter::owner() const {
        return owner_.lock();
    }
    
    void Layouter::_set_owner(const Ptr<View> & owner) {
        owner_ = owner;
    }
}