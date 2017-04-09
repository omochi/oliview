#include <cstdio>
#include <cstdlib>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#include <oliview/oliview.h>

using namespace oliview;

void PostUpdate(Application * app, GLFWwindow * window) {
    app->task_queue()->Post([=] {
        glfwPollEvents();

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0, 0.5f);
        glEnd();

        glfwSwapBuffers(window);

        PostUpdate(app, window);
    });
}

int main(int argc, char * argv[]) {

    Application * app = Application::shared().get();
    app->set_init([] {
        Ref<Window> window;
        window.Attach(new Window());
    });
    app->set_final([] {

    });
    app->Run();

    return EXIT_SUCCESS;
}
