#include "./text_test.h"

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
        Text::Position ep;
        t1->Insert(Text::Position(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana");
        RHETORIC_ASSERT(ep == Text::Position(0, 11));
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        Text::Position ep;
        t1->Insert(Text::Position(0, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "bananaapple");
        RHETORIC_ASSERT(ep == Text::Position(0, 6));
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        Text::Position ep;
        t1->Insert(Text::Position(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbananale");
        RHETORIC_ASSERT(ep == Text::Position(0, 9));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Position ep;
        t1->Insert(Text::Position(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbananale\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "");
        RHETORIC_ASSERT(ep == Text::Position(0, 9));
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana\n");
        Text::Position ep;
        t1->Insert(Text::Position(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "le");
        RHETORIC_ASSERT(ep == Text::Position(1, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Position ep;
        t1->Insert(Text::Position(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "");
        RHETORIC_ASSERT(ep == Text::Position(0, 11));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        Text::Position ep;
        t1->Insert(Text::Position(1, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana");
        RHETORIC_ASSERT(ep == Text::Position(1, 6));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Position ep;
        t1->Insert(Text::Position(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Position(1, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Position ep;
        t1->Insert(Text::Position(1, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Position(2, 0));
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        Text::Position ep;
        t1->Insert(Text::Position(0, 3), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "le\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "");
        RHETORIC_ASSERT(ep == Text::Position(1, 0));
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        Text::Position ep;
        t1->Insert(Text::Position(0, 5), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "applecherry\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "donuts\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "banana");
        RHETORIC_ASSERT(ep == Text::Position(1, 6));
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        Text::Position ep;
        t1->Insert(Text::Position(1, 0), t2, &ep);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "cherry\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "donutsbanana");
        RHETORIC_ASSERT(ep == Text::Position(2, 6));
    }
}
