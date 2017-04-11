#include <cstdio>
#include <cstdlib>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#include <oliview/oliview.h>

using namespace oliview;

int main(int argc, char * argv[]) {
    Application::Main([] {
        Ref<Window> window;
        window.Attach(new Window());
    },
                      [] {
                      } );
    return EXIT_SUCCESS;
}
