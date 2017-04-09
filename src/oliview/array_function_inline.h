
namespace oliview {
    template <typename C>
    std::vector<typename C::value_type>
    ArrayRemove(const C & array,
                const std::function<bool(const typename C::value_type &)> & pred)
    {
        std::vector<typename C::value_type> ret;
        for (const auto & item : array) {
            if (!pred(item)) {
                ret.push_back(item);
            }
        }
        return ret;
    }
}
