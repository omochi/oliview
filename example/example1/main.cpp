#include <oliview/oliview.h>

#include <stb_truetype.h>

using namespace oliview;

void f(const FilePath & path_) {
    auto p = path_;
    p.Expand().value();

    Print(Format("[%s] => [%s]",
                 path_.ToString().c_str(),
                 p.ToString().c_str()));
}

//const char *GetFontName(const stbtt_fontinfo *font, int *length, int platformID, int encodingID, int languageID, int nameID)
//{
//    int32_t i,count,stringOffset;
//    uint8_t *fc = font->data;
//    uint32_t offset = font->fontstart;
//    uint32_t nm = stbtt__find_table(fc, offset, "name");
//    if (!nm) return NULL;
//
//    count = ttUSHORT(fc+nm+2);
//    stringOffset = nm + ttUSHORT(fc+nm+4);
//    for (i=0; i < count; ++i) {
//        uint32_t loc = nm + 6 + 12 * i;
//        if (platformID == ttUSHORT(fc+loc+0) && encodingID == ttUSHORT(fc+loc+2)
//            && languageID == ttUSHORT(fc+loc+4) && nameID == ttUSHORT(fc+loc+6)) {
//            *length = ttUSHORT(fc+loc+8);
//            return (const char *) (fc+stringOffset+ttUSHORT(fc+loc+10));
//        }
//    }
//    return NULL;
//}


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


        Font::Find(window->nvg_context(), "ヒラギノ角ゴシック W3").value();
        Font::Find(window->nvg_context(), "Arial").value();
    }
};

int main(int argc, char * argv[]) {
    return ApplicationMain<Example1>(argc, argv);
}
