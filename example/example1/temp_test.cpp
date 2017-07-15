#include "./temp_test.h"


void RunTests() {
    EitherTest();
    Utf8Test();
    TextAdvanceTest1();
    TextAdvanceCrlfTest1();
    TextBackTest1();
    TextBackCrlfTest1();
    TextInsertTest1();
    TextDeleteTest1();
}

void EitherTest() {
    Either2<int, std::string> a = EitherCase<Either2Tag::Case0>(3);
    RHETORIC_ASSERT(a.AsCase0() == 3);
    RHETORIC_ASSERT(a.AsCase<Either2Tag::Case0>() == 3);
}

void Utf8Test() {
    
}
