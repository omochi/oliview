#include "./example1_app.h"

void EitherTest() {
    Either2<int, std::string> a = EitherCase<Either2Tag::Case0>(3);
    RHETORIC_ASSERT(a.AsCase0() == 3);
    RHETORIC_ASSERT(a.AsCase<Either2Tag::Case0>() == 3);
}

int main(int argc, char * argv[]) {
    EitherTest();
    
    auto app = New<Example1App>();
    return app->Main(argc, argv);
}
