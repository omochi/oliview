#include "./text_test.h"

void TextAdvanceTest1() {
    auto text = New<Text>("迷ったら\n型付け");
    
    auto index = text->begin_index();
    
    RHETORIC_ASSERT(index == Text::Index(0, 0));
    RHETORIC_ASSERT(text->GetCharAt(index) == "迷");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 3));
    RHETORIC_ASSERT(text->GetCharAt(index) == "っ");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 6));
    RHETORIC_ASSERT(text->GetCharAt(index) == "た");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 9));
    RHETORIC_ASSERT(text->GetCharAt(index) == "ら");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 12));
    RHETORIC_ASSERT(text->GetCharAt(index) == "\n");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(1, 0));
    RHETORIC_ASSERT(text->GetCharAt(index) == "型");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(1, 3));
    RHETORIC_ASSERT(text->GetCharAt(index) == "付");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(1, 6));
    RHETORIC_ASSERT(text->GetCharAt(index) == "け");
    index = text->AdvanceIndex(index);
    
    RHETORIC_ASSERT(index == text->end_index());
}

void TextBackTest1() {
    auto text = New<Text>("迷ったら\n型付け");
    
    auto index = text->end_index();
    
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(1, 6));
    RHETORIC_ASSERT(text->GetCharAt(index) == "け");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(1, 3));
    RHETORIC_ASSERT(text->GetCharAt(index) == "付");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(1, 0));
    RHETORIC_ASSERT(text->GetCharAt(index) == "型");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 12));
    RHETORIC_ASSERT(text->GetCharAt(index) == "\n");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 9));
    RHETORIC_ASSERT(text->GetCharAt(index) == "ら");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 6));
    RHETORIC_ASSERT(text->GetCharAt(index) == "た");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 3));
    RHETORIC_ASSERT(text->GetCharAt(index) == "っ");
    index = text->BackIndex(index);
    
    RHETORIC_ASSERT(index == Text::Index(0, 0));
    RHETORIC_ASSERT(text->GetCharAt(index) == "迷");
    
    RHETORIC_ASSERT(index == text->begin_index());
}

void TextInsertTest1() {
    {
        auto t1 = New<Text>();
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "");
    }
    {
        auto t1 = New<Text>("");
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "");
    }
    {
        auto t1 = New<Text>("apple");
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple");
    }
    {
        auto t1 = New<Text>("banana");
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "banana");
    }
    {
        auto t1 = New<Text>("apple\n");
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana");
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana");
        RHETORIC_ASSERT(ep == Text::Index(0, 11));
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(0, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "bananaapple");
        RHETORIC_ASSERT(ep == Text::Index(0, 6));
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbananale");
        RHETORIC_ASSERT(ep == Text::Index(0, 9));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbananale\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "");
        RHETORIC_ASSERT(ep == Text::Index(0, 9));
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana\n");
        Text::Index ep;
        t1->Insert(Text::Index(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "le");
        RHETORIC_ASSERT(ep == Text::Index(1, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "");
        RHETORIC_ASSERT(ep == Text::Index(0, 11));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(0, 6), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana");
        RHETORIC_ASSERT(ep == Text::Index(1, 6));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Index ep;
        t1->Insert(Text::Index(1, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana");
        RHETORIC_ASSERT(ep == Text::Index(1, 6));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Index ep;
        t1->Insert(Text::Index(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Index(1, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Index ep;
        t1->Insert(Text::Index(0, 6), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Index(2, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Index ep;
        t1->Insert(Text::Index(1, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Index(2, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Index ep;
        t1->Insert(Text::Index(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "le\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Index(1, 0));
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        Text::Index ep;
        t1->Insert(Text::Index(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "applecherry\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "donuts\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "banana");
        RHETORIC_ASSERT(ep == Text::Index(1, 6));
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        Text::Index ep;
        t1->Insert(Text::Index(0, 6), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "cherry\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "donutsbanana");
        RHETORIC_ASSERT(ep == Text::Index(2, 6));
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        Text::Index ep;
        t1->Insert(Text::Index(1, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "cherry\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "donutsbanana");
        RHETORIC_ASSERT(ep == Text::Index(2, 6));
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("１行\n２行\n３行");
        Text::Index ep;
        t1->Insert(Text::Index(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 4);
        RHETORIC_ASSERT(*t1->lines()[0] == "app１行\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "２行\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "３行le\n");
        RHETORIC_ASSERT(*t1->lines()[3] == "banana");
        RHETORIC_ASSERT(ep == Text::Index(2, 6));
    }
}

void TextDeleteTest1() {
    {
        auto t1 = New<Text>("apple");
        t1->Delete(Text::Index(0, 0), Text::Index(0, 5));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "");
    }
    {
        auto t1 = New<Text>("apple");
        t1->Delete(Text::Index(0, 1), Text::Index(0, 4));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "ae");
    }
    {
        auto t1 = New<Text>("apple\n");
        t1->Delete(Text::Index(0, 5), Text::Index(0, 6));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "apple");
    }
    {
        auto t1 = New<Text>("apple\n");
        t1->Delete(Text::Index(0, 5), Text::Index(1, 0));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "apple");
    }
    {
        auto t1 = New<Text>("apple\n");
        t1->Delete(Text::Index(0, 3), Text::Index(0, 6));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "app");
    }
    {
        auto t1 = New<Text>("apple\n");
        t1->Delete(Text::Index(0, 3), Text::Index(1, 0));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "app");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        t1->Delete(Text::Index(0, 5), Text::Index(0, 6));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "applebanana");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        t1->Delete(Text::Index(0, 5), Text::Index(1, 0));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "applebanana");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        t1->Delete(Text::Index(0, 4), Text::Index(1, 2));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "applnana");
    }
    {
        auto t1 = New<Text>("apple\nbanana\ncherry");
        t1->Delete(Text::Index(0, 6), Text::Index(1, 7));
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "apple\n");
        RHETORIC_ASSERT(*t1->GetLineAt(1) == "cherry");
    }
    {
        auto t1 = New<Text>("apple\nbanana\ncherry");
        t1->Delete(Text::Index(0, 6), Text::Index(2, 0));
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "apple\n");
        RHETORIC_ASSERT(*t1->GetLineAt(1) == "cherry");
    }
    {
        auto t1 = New<Text>("apple\nbanana\ncherry");
        t1->Delete(Text::Index(1, 0), Text::Index(1, 7));
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "apple\n");
        RHETORIC_ASSERT(*t1->GetLineAt(1) == "cherry");
    }
    {
        auto t1 = New<Text>("apple\nbanana\ncherry");
        t1->Delete(Text::Index(1, 0), Text::Index(2, 0));
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "apple\n");
        RHETORIC_ASSERT(*t1->GetLineAt(1) == "cherry");
    }
    {
        auto t1 = New<Text>("apple\nbanana\ncherry");
        t1->Delete(Text::Index(0, 0), Text::Index(2, 6));
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->GetLineAt(0) == "");
    }
}
