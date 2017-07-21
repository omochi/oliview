#include "./view.h"

#include "./application.h"
#include "./window.h"

namespace oliview {
    View::DrawInfo::DrawInfo():
    window_transform(Matrix3x3::Identity())
    {}
    
    View::DrawCommand::DrawCommand():
    shadow(false)
    {}
    
    View::View():
    background_color_(1, 1, 1, 0),
    needs_layout_(true),
    clipping_children_(false),
    focusable_(false),
    visible_(true),
    focused_(false)
    {}
    
    View::~View() {}
    
    Ptr<View> View::GetChildAt(size_t index) const {
        return children_[index];
    }
    
    size_t View::child_num() const {
        return children_.size();
    }
    
    void View::AddChild(const Ptr<View> & child) {
        InsertChildAt(child_num(), child);
    }
    
    void View::InsertChildAt(size_t index, const Ptr<View> & child) {
        RHETORIC_ASSERT(child->parent() == nullptr);
        
        children_.insert(children_.begin() + ToSigned(index), child);
        child->_SetParent(shared_from_this());
        
        SetNeedsLayout();
    }

    void View::RemoveChild(const Ptr<View> & child) {
        while (true) {
            auto index = ArrayFindIndexEqR(children_, child);
            if (!index) {
                break;
            }
            RemoveChildAt(*index);
        }
    }

    void View::RemoveChildAt(size_t index) {
        auto child = children_[index];
        ArrayRemoveAt(&children_, index);
        child->_SetParent(nullptr);
        
        SetNeedsLayout();
    }
    
    void View::RemoveFromParent() {
        auto parent = this->parent();
        if (parent) {
            parent->RemoveChild(shared_from_this());
        }
    }

    size_t View::GetIndexOfChild(const Ptr<const View> & child) const {
        std::vector<Ptr<View>> children = this->children();
        std::vector<Ptr<const View>> const_children(children.begin(), children.end());
        return ArrayFindIndexEq(const_children, child).value();
    }
    
    Ptr<View> View::GetNextSibling() const {
        auto pr = parent();
        if (!pr) { return nullptr; }
        auto index = pr->GetIndexOfChild(shared_from_this());
        if (index + 1 == pr->child_num()) {
            return nullptr;
        }
        return pr->GetChildAt(index + 1);
    }
    
    Ptr<View> View::GetPrevSibling() const {
        auto pr = parent();
        if (!pr) { return nullptr; }
        auto index = pr->GetIndexOfChild(shared_from_this());
        if (index == 0) {
            return nullptr;
        }
        return pr->GetChildAt(index - 1);
    }

    void View::set_frame(const Rect & value) {
        if (frame_ == value) {
            return;
        }
        
        frame_ = value;
        set_bounds(Rect(bounds().origin(), frame_.size()));
        SetNeedsLayout();
    }
    
    void View::set_bounds(const Rect & value) {
        if (bounds_ == value) {
            return;
        }
        
        bounds_ = value;
        frame_.set_size(bounds_.size());
        SetNeedsLayout();
    }

    void View::set_background_color(const Color & value) {
        background_color_ = value;
    }
    
    void View::set_clipping_children(bool value) {
        clipping_children_ = value;
    }
    
    void View::set_visible(bool value) {
        visible_ = value;
        
        if (!visible_) {
            Unfocus();
        }
    }
    
    void View::set_focusable(bool value) {
        focusable_ = value;
        
        if (!focusable()) {
            Unfocus();
        }
    }
    
    void View::Focus() {
        if (!focusable()) { return; }
        
        auto w = window();
        if (w) {
            w->_Focus(shared_from_this());
        }
    }
    
    void View::Unfocus() {
        if (!focused()) { return; }
        
        auto w = window();
        if (w) {
            w->UnfocusView();
        }
    }
    
    Ptr<View> View::GetFirstFocusChild() const {
        if (child_num() == 0) {
            return nullptr;
        }
        return GetChildAt(0);
    }
    
    Ptr<View> View::GetNextFocusChild(const Ptr<const View> & child) const {
        RHETORIC_ASSERT(child->parent() == shared_from_this());
        return child->GetNextSibling();
    }
    
    Ptr<View> View::GetPrevFocusChild(const Ptr<const View> & child) const {
        RHETORIC_ASSERT(child->parent() == shared_from_this());
        return child->GetPrevSibling();
    }
    
    Ptr<View> View::GetLastFocusChild() const {
        if (child_num() == 0) {
            return nullptr;
        }
        return GetChildAt(child_num() - 1);
    }
    
    void View::SetNeedsLayout() {
        needs_layout_ = true;
    }
    
    void View::set_children_layouter(const Ptr<Layouter> & value) {
        if (children_layouter_) {
            children_layouter_->_set_view(nullptr);
        }
        
        children_layouter_ = value;
        
        if (children_layouter_) {
            children_layouter_->_set_view(shared_from_this());
        }
        
        SetNeedsLayout();
    }
    
    Size View::Measure(NVGcontext * ctx, const MeasureQuery & query) const {
        Size size = MeasureOwnContent(ctx, query);
        
        if (children_layouter_) {
            Size layouter_size = children_layouter_->Measure(ctx, query);
            
            size = size.GetMax(layouter_size);
        }
        
        return size;
    }
    
    Size View::MeasureOwnContent(NVGcontext *, const MeasureQuery & query) const {        
        float width = query.max_width().GetOr(0.0f);
        float height = query.max_height().GetOr(0.0f);
                
        return Size(width, height);
    }
    
    void View::Layout(NVGcontext * ctx) {
        LayoutOwnContent(ctx);
        
        if (children_layouter_) {
            children_layouter_->Layout(ctx);
        }
    }
    
    void View::LayoutOwnContent(NVGcontext * ctx) {
        RHETORIC_UNUSED(ctx);
    }
    
    void View::DrawOwnContent(NVGcontext * ctx) {
        RHETORIC_UNUSED(ctx);
    }

    Vector2 View::ConvertPointToWindow(const Vector2 & point) const {
        Matrix3x3 m = draw_info_.window_transform;
        return point.ApplyTransform(m);
    }
    
    Vector2 View::ConvertPointFromWindow(const Vector2 & point) const {
        Matrix3x3 m = draw_info_.window_transform.Invert();
        return point.ApplyTransform(m);
    }
    
    Vector2 View::ConvertPointToView(const Vector2 & point, const Ptr<View> & view) const {
        return view->ConvertPointFromWindow(ConvertPointToWindow(point));
    }
    
    Vector2 View::ConvertPointFromView(const Vector2 & point, const Ptr<View> & view) const {
        return ConvertPointFromWindow(view->ConvertPointToWindow(point));
    }
    
    bool View::IsPointInside(const Vector2 & point) const {
        return bounds().Contains(point);
    }
    
    Ptr<View> View::MouseHitTest(const Vector2 & pos) const {
        if (!visible()) {
            return nullptr;
        }
        
        bool is_inside = IsPointInside(pos);
        
        bool test_children = true;
        if (clipping_children()) {
            if (!is_inside) {
                test_children = false;
            }
        }
        
        if (test_children) {
            auto children = ArrayReversed(this->children());
            for (auto & child : children) {
                Vector2 pos_in_child = ConvertPointToView(pos, child);
                auto ret = child->MouseHitTest(pos_in_child);
                if (ret) {
                    return ret;
                }
            }
        }
        
        if (is_inside) {
            return const_cast<View *>(this)->shared_from_this();
        }
        
        return nullptr;
    }
    
    bool View::OnMouseDownEvent(const MouseEvent &) {
        return false;
    }
    
    void View::OnMouseMoveEvent(const MouseEvent &) {
    }
    
    void View::OnMouseUpEvent(const MouseEvent &) {
    }
    
    void View::OnMouseCancelEvent() {
    }
    
    bool View::OnScrollEvent(const ScrollEvent &) {
        return false;
    }
    
    bool View::OnKeyDownEvent(const KeyEvent &) {
        return false;
    }
    
    bool View::OnKeyUpEvent(const KeyEvent &) {
        return false;
    }

    bool View::OnKeyRepeatEvent(const KeyEvent &) {
        return false;
    }
    
    void View::OnCharEvent(const CharEvent &) {
    }
    
    void View::OnUpdateAnimation(float) {
    }
    
    void View::OnFocus() {
    }
    
    void View::OnUnfocus() {
    }
    
    bool View::_InvokeLayout(NVGcontext * ctx) {
        bool updated = false;
        
        if (needs_layout_) {
            needs_layout_ = false;
            
            Layout(ctx);
            
            updated = true;
        }
        
        for (auto & child : children()) {
            if (child->_InvokeLayout(ctx)) {
                updated = true;
            }
        }
        
        return updated;
    }

    void View::_PrepareToDraw(const DrawInfo & info) {
        if (!visible()) {
            return;
        }
        
        draw_info_ = info;
        
        Matrix3x3 wtr = Matrix3x3::Identity();
        
        wtr *= Matrix3x3::Translation(-bounds().origin());
        wtr *= Matrix3x3::Translation(frame().origin());
        wtr *= draw_info_.window_transform;
        
        draw_info_.window_transform = wtr;
        
        Rect clip_in_window = bounds().ApplyTransform(draw_info_.window_transform);
        draw_info_.content_clip_frame = clip_in_window.GetIntersection(draw_info_.parent_clip_frame);

        DrawInfo child_info = draw_info_;
        
        if (clipping_children_) {
            child_info.parent_clip_frame = child_info.content_clip_frame;
        }
        
        for (auto & child : children_) {
            child->_PrepareToDraw(child_info);
        }
    }
    
    void View::_InvokeDraw(NVGcontext * ctx, bool shadow) {
        nvgSave(ctx);
        if (shadow) {
            NVGSetScissor(ctx, draw_info_.parent_clip_frame);
            nvgResetTransform(ctx);
            NVGTransform(ctx, draw_info_.window_transform);
            DrawShadow(ctx);
        } else {
            NVGSetScissor(ctx, draw_info_.content_clip_frame);
            nvgResetTransform(ctx);
            NVGTransform(ctx, draw_info_.window_transform);
            DrawBackground(ctx);
            DrawOwnContent(ctx);
        }
        nvgRestore(ctx);
    }

    void View::_CollectDrawCommand(std::vector<DrawCommand> * commands) {
        for (auto & child : children_) {
            if (!child->visible()) {
                continue;
            }
            
            DrawCommand command;
            command.view = child;
            command.shadow = true;
            commands->push_back(command);
            
            command.shadow = false;
            commands->push_back(command);
        }

        for (auto & child : children_) {
            if (!child->visible()) {
                continue;
            }
            
            child->_CollectDrawCommand(commands);
        }
    }
    
    void View::_SetParent(const Ptr<View> & parent) {        
        if (parent) {
            RHETORIC_ASSERT(application() == parent->application());
        }
        
        parent_ = parent;
        
        auto old_window = this->window();
        Ptr<Window> new_window;
        if (parent) {
            new_window = parent->window();
        }
        
        if (old_window) {
            InvokeWindowOnRemoveView(old_window);
        }
        _SetWindow(new_window);
        if (new_window) {
            InvokeWindowOnAddView(new_window);
        }
    }

    void View::_SetWindow(const Ptr<Window> & window) {
        if (window) {
            RHETORIC_ASSERT(application() == window->application());
        }
    
        window_ = window;

        for (auto & child : children_) {
            child->_SetWindow(window);
        }
    }
    
    MouseEvent View::_ConvertMouseEventFromWindow(const MouseEvent & event) const {
        MouseEvent ret = event;
        ret.set_pos(ConvertPointFromWindow(event.pos()));
        return ret;
    }
    
    ScrollEvent View::_ConvertScrollEventFromWindow(const ScrollEvent & event) const {
        ScrollEvent ret = event;
        ret.set_pos(ConvertPointFromWindow(event.pos()));
        return ret;        
    }
    
    void View::_SetFocused(bool value) {
        focused_ = value;
    }
    
    void View::_UpdateAnimation(float delta_time) {
        OnUpdateAnimation(delta_time);
        
        for (auto & child : children_) {
            child->_UpdateAnimation(delta_time);
        }
    }

    void View::DrawBackground(NVGcontext * ctx) {
        nvgBeginPath(ctx);
        NVGAddRectPath(ctx, bounds());
        NVGSetFillColor(ctx, background_color_);
        nvgFill(ctx);
    }
    
    void View::DrawShadow(NVGcontext * ctx) {
        if (focused()) {
            EdgeInset shadow_inset(-2, -2, -2, -2);
            Rect shadow_rect = bounds().InsetBy(shadow_inset);
            
            nvgBeginPath(ctx);
            NVGAddRoundRectPath(ctx, shadow_rect, 2.0f);
            NVGSetFillColor(ctx, Color(0.0f, 0.4f, 0.8f, 0.3f));
            
            nvgFill(ctx);
        }
    }
    
    void View::InvokeWindowOnAddView(const Ptr<Window> & window) {
        window->_OnAddView(shared_from_this());
        
        for (auto & child : children()) {
            child->InvokeWindowOnAddView(window);
        }
    }
    
    void View::InvokeWindowOnRemoveView(const Ptr<Window> & window) {
        for (auto & child : children()) {
            child->InvokeWindowOnRemoveView(window);
        }
        
        window->_OnRemoveView(shared_from_this());
    }
    
    
}
