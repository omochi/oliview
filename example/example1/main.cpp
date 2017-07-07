#include "./example1_app.h"

#include "./temp_test.h"

int main(int argc, char * argv[]) {
    RunTests();
    
    auto app = New<Example1App>();
    return app->Main(argc, argv);
}
