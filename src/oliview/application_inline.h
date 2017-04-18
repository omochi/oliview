namespace oliview {
    template <typename T>
    int ApplicationMain(int argc, char * argv[]) {
        Ref<T> app = Create<T>();
        app->Run();
        return EXIT_SUCCESS;
    }
}
