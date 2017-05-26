namespace oliview {
    template <typename T>
    int ApplicationMain(int argc, char * argv[]) {
        Ptr<T> app = New<T>();
        app->Run();
        return EXIT_SUCCESS;
    }
}
