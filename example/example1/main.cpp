#include <cstdio>
#include <cstdlib>

#include <oliview/oliview.h>

using namespace oliview;

void CreateWindow() {
    Ref<Window> w;
    w.Attach(new Window());


}

int main(int argc, char * argv[]) {
    Application::Main([] {
        CreateWindow();
    },
                      [] {
                      } );
    return EXIT_SUCCESS;
}
