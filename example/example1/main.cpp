#include <oliview/oliview.h>

using namespace oliview;

class Example1 : public Application {
    virtual void OnInit() override {
        auto window = Create<Window>(this_ref());

        auto view1 = Create<View>();
        view1->set_frame(Rect(20, 40, 120, 60));
        view1->set_background_color(Color(1, 0, 0, 1));
        window->root_view()->AddChild(view1);
    }
};

int main(int argc, char * argv[]) {
    return ApplicationMain<Example1>(argc, argv);
}
