#include "./text_test.h"

void TextInsertTest1() {
    {
        auto t1 = New<Text>();
        RHETORIC_ASSERT(t1->line_num() == 0);
    }
    {
        auto t1 = New<Text>("");
        RHETORIC_ASSERT(t1->line_num() == 0);
    }
    {
        auto t1 = New<Text>("apple");
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple");
    }
    {
        auto t2 = New<Text>("banana");
        RHETORIC_ASSERT(t2->line_num() == 1);
        RHETORIC_ASSERT(*t2->lines()[0] == "banana");
    }
    {
        auto t1 = New<Text>("apple\n");
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
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
        t1->Insert(Text::Position(0, 5), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana");
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        t1->Insert(Text::Position(0, 0), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "bananaapple");
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana");
        t1->Insert(Text::Position(0, 3), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbananale");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        t1->Insert(Text::Position(0, 3), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbananale\n");
    }
    {
        auto t1 = New<Text>("apple");
        auto t2 = New<Text>("banana\n");
        t1->Insert(Text::Position(0, 3), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "le");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        t1->Insert(Text::Position(0, 5), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 1);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana\n");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        t1->Insert(Text::Position(0, 6), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana");
        t1->Insert(Text::Position(1, 0), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        t1->Insert(Text::Position(0, 5), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "applebanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "\n");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        t1->Insert(Text::Position(0, 6), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana\n");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        t1->Insert(Text::Position(1, 0), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "banana\n");
    }
    {
        auto t1 = New<Text>("apple\n");
        auto t2 = New<Text>("banana\n");
        t1->Insert(Text::Position(0, 3), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 2);
        RHETORIC_ASSERT(*t1->lines()[0] == "appbanana\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "le\n");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        t1->Insert(Text::Position(0, 5), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "applecherry\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "donuts\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "banana");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        t1->Insert(Text::Position(0, 6), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "cherry\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "donutsbanana");
    }
    {
        auto t1 = New<Text>("apple\nbanana");
        auto t2 = New<Text>("cherry\ndonuts");
        t1->Insert(Text::Position(1, 0), t2, nullptr);
        RHETORIC_ASSERT(t1->line_num() == 3);
        RHETORIC_ASSERT(*t1->lines()[0] == "apple\n");
        RHETORIC_ASSERT(*t1->lines()[1] == "cherry\n");
        RHETORIC_ASSERT(*t1->lines()[2] == "donutsbanana");
    }
}
