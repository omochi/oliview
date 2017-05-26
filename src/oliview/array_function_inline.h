namespace oliview {
    template <typename C>
    Optional<int>
    ArrayFind(const C & array,
              const std::function<bool(const typename C::value_type &)> & pred)
    {
        for (auto iter = array.cbegin(); iter != array.cend(); iter++) {
            if (pred(*iter)) {
                return Some((int)(iter - array.cbegin()));
            }
        }
        return None();
    }

    template <typename C>
    Optional<int>
    ArrayFindR(const C & array,
               const std::function<bool(const typename C::value_type &)> & pred)
    {
        for (auto iter = array.crbegin(); iter != array.crend(); iter++) {
            if (pred(*iter)) {
                int offset = (int)(iter - array.crbegin());
                return Some((int)array.size() - 1 - offset);
            }
        }
        return None();
    }

    template <typename C>
    void
    ArrayRemove(C & array,
                const std::function<bool(const typename C::value_type &)> & pred)
    {
        std::vector<typename C::value_type> ret;
        for (const auto & item : array) {
            if (!pred(item)) {
                ret.push_back(item);
            }
        }
        array = ret;
    }

    template <typename C>
    void
    ArrayRemoveAt(C & array, int index)
    {
        array.erase(array.begin() + index);
    }
}
