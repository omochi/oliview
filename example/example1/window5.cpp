#include "./window5.h"

void Window5::Init(const Ptr<Application> & app) {
    Window::Init(app);
    
    {
        auto view = OLIVIEW_INIT(TextBox, app);
        text_box_ = view;
        content_view()->AddChild(view);
        content_view()->set_background_color(Color(0.8f, 1.0f, 0.8f, 1.0f));

        view->set_text_alignment(TextAlignment::Left);
        view->set_desired_height_in_line_num(Some(1));
        view->set_max_line_num(Some(1));
        view->set_word_wrap_enabled(false);
        view->set_text(New<Text>("C++14\n"
                                 "\n"
                                 "C++14 は、プログラミング言語 C++ のISO標準 ISO/IEC 14882:2014 の略称である。"
                                 "C++11 の後継であり、マイナーアップデートの位置付けとなっている。"
                                 "C++11 が2009年中の標準化を目指していたため C++0x と呼んでいたのを受けて、"
                                 "C++14 は制定中は C++1y という仮称で呼ばれていた。\n"
                                 "委員会草案 N3690[1] が2013年5月15日に発表され、"
                                 "作業草案 N3936 が2014年3月2日に発表され、"
                                 "投票が2014年8月15日までに行われ、"
                                 "その結果が2014年8月18日に発表され承認され[2]、2014年12月15日に出版された[3]。\n"
                                 "\n"
                                 "C++11 から下記の機能が追加になった。\n"
                                 "- ある種の C++ の文脈上の変換の微調整\n"
                                 "- 二進数リテラル\n"
                                 "- 戻り値の型推論\n"
                                 "- 汎用ラムダキャプチャ\n"
                                 "- ジェネリックラムダ式\n"
                                 "- 変数テンプレート\n"
                                 "- constexpr 関数の制限緩和\n"
                                 "- メンバー初期化と集成体\n"
                                 "- メモリ割り当ての明確化\n"
                                 "- メモリ解放時のサイズ指定\n"
                                 "- deprecated 属性\n"
                                 "- 数値区切り"
                                 ));
    }
    
}

void Window5::LayoutContentView(NVGcontext * ctx, const Ptr<WindowContentView> & view) {
    
    auto rect = view->bounds().InsetBy(EdgeInset(50, 50, 50, 50));
    
    MeasureQuery query;
    query.set_max_width(Some(rect.size().width()));
    query.set_max_height(Some(rect.size().height()));
    
    Size measured_size = text_box_->Measure(ctx, query);
    
    text_box_->set_frame(Rect(rect.origin(), measured_size));
}

void Window5::OnKeyDownEvent(const KeyEvent & event) {
//    if (event.key() == GLFW_KEY_C) {
//        if ((event.modifier() & GLFW_MOD_SHIFT) != 0) {
//            text_box_->set_max_line_num(None());
//        } else {
//            text_box_->set_max_line_num(Some(3));
//        }
//    }
}
