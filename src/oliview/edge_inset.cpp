#include "./edge_inset.h"

namespace oliview {
    EdgeInset::EdgeInset():
    EdgeInset(0, 0, 0, 0)
    {}
    
    EdgeInset::EdgeInset(float top, float left, float bottom, float right):
    top_(top),
    left_(left),
    bottom_(bottom),
    right_(right)
    {}
    
}
