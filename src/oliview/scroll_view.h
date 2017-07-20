#pragma once

#include "./axis.h"
#include "./init_macro.h"
#include "./scroll_bar.h"
#include "./scroll_event.h"
#include "./size.h"
#include "./view.h"

namespace oliview {
    class ScrollContentView;
    
    /*
     frame: 親ビューで見た窓サイズ
     content_size: スクロール空間のサイズ
     これらに基づき、 bounds を制御してスクロールを実現する。
     ScrollBarを縦横に搭載する。
     Barの表示中はframeが小さくなる。
     
     autoContentSizeEnabledをtrueにすると、
     Layoutで自動でcontentSizeを設定する。
     Layouterに対して max_width無し, max_height無しでMeasureして
     得られる理想サイズに対して、
     そのサイズ + frameのmaxで空間サイズを張る。
     */
    class ScrollView : public View {
    public:
        virtual void Init(const Ptr<Application> & app) override;
        
        Ptr<View> content_view() const;
        
        RHETORIC_GETTER(Size, content_size)
        void set_content_size(const Size & value);
        
        RHETORIC_GETTER(bool, auto_content_size_enabled)
        void set_auto_content_size_enabled(bool value);
        
        RHETORIC_GETTER(Vector2, scroll_position)
        bool ScrollTo(const Vector2 & position);
        
        virtual Size Measure(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void Layout(NVGcontext * ctx) override;
        
        virtual bool OnScrollEvent(const ScrollEvent & event) override;
    private:
        void ClampScrollPosition();
        Size ComputeContentSize(NVGcontext * ctx, const Size & visible_size) const;
        
        Size content_size_;
        Vector2 scroll_position_;
        
        bool auto_content_size_enabled_;
        
        Ptr<View> content_view_;
        Ptr<ScrollBar> y_bar_;
        Ptr<ScrollBar> x_bar_;
    };
}

