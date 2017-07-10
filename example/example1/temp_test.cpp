#include "./temp_test.h"


void RunTests() {
    EitherTest();
    TextTest();
    Utf8Test();
    TextInsertTest1();
    TextDeleteTest1();
}

void EitherTest() {
    Either2<int, std::string> a = EitherCase<Either2Tag::Case0>(3);
    RHETORIC_ASSERT(a.AsCase0() == 3);
    RHETORIC_ASSERT(a.AsCase<Either2Tag::Case0>() == 3);
}

void TextTest() {
    auto text = New<Text>();
    
    text->set_string("迷ったら\n型付け");
    
    auto pos = text->begin_index();
    
    RHETORIC_ASSERT(pos == Text::Index(0, 0));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "迷");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(0, 3));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "っ");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(0, 6));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "た");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(0, 9));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "ら");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(0, 12));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "\n");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(1, 0));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "型");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(1, 3));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "付");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == Text::Index(1, 6));
    RHETORIC_ASSERT(text->GetCharAt(pos).AsString() == "け");
    pos = text->AdvanceIndex(pos);
    
    RHETORIC_ASSERT(pos == text->end_index());
}

void Utf8Test() {
    
    std::string str = "とても暑い\nお風呂のようだ\r夏はクソ\nぴ";
    Utf8LineReader(str.c_str(), (int)str.size());
}
