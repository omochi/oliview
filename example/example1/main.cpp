#include <oliview/oliview.h>

using namespace oliview;

class Example1 : public Application {
    virtual void OnInit() override {
        auto window = Window::Create(shared_from_this());

        auto view1 = New<View>();
        window->root_view()->AddChild(view1);
        view1->set_frame(Rect(20, 40, 120, 60));
        view1->set_background_color(Color(1, 0, 0, 1));

        auto view2 = New<View>();
        window->root_view()->AddChild(view2);
        view2->set_frame(Rect(20, 120, 300, 300));
        view2->set_background_color(Color(0, 0, 1, 1));

        auto view3 = New<View>();
        view2->AddChild(view3);
        view3->set_frame(Rect(20, 20, 40, 40));
        view3->set_background_color(Color(0, 1, 0, 1));

        auto view4 = New<Label>("明xるい未来");
        window->root_view()->AddChild(view4);
        view4->set_frame(Rect(20, 440, 30, 30));
        view4->set_background_color(Color(1, 0, 0, 1));
        
        auto usi = GetUnicodeScalarIndices(view4->text());
//
//        font_manager()->Find("ヒラギノ角ゴシック W3");
//        font_manager()->Find("Arial").value();
    }
};

int main(int argc, char * argv[]) {
    auto app = New<Example1>();
    return app->Main(argc, argv);
}
