#include <oliview/oliview.h>

using namespace oliview;

class Example1 : public Application {
    virtual void OnInit() override {
        auto window = Create<Window>(this_ref());

        auto view1 = Create<View>();
        window->root_view()->AddChild(view1);
        view1->set_frame(Rect(20, 40, 120, 60));
        view1->set_background_color(Color(1, 0, 0, 1));

        auto view2 = Create<View>();
        window->root_view()->AddChild(view2);
        view2->set_frame(Rect(20, 120, 300, 300));
        view2->set_background_color(Color(0, 0, 1, 1));

        auto view3 = Create<View>();
        view2->AddChild(view3);
        view3->set_frame(Rect(20, 20, 40, 40));
        view3->set_background_color(Color(0, 1, 0, 1));
    }
};

int main(int argc, char * argv[]) {
    return ApplicationMain<Example1>(argc, argv);
}
