namespace oliview {
    template <typename T>
    T Clamp(const T & value, const T & min, const T & max) {
        return std::max(min, std::min(value, max));
    }
}
