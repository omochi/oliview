#include "./example1_app.h"

void Example1App::OnInit() {
    auto app = shared_from_this();
    
    OLIVIEW_INIT(Window1, app);
    OLIVIEW_INIT(Window2, app);
    OLIVIEW_INIT(Window3, app);
}
