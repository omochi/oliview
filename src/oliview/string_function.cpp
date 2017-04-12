#include "./string_function.h"

namespace oliview {
    std::string Join(const std::vector<std::string> & array,
                     const std::string & glue)
    {
        std::string ret;
        for (int i = 0; i < array.size(); i++) {
            if (i > 0) {
                ret.append(glue);
            }
            ret.append(array[i]);
        }
        return ret;
    }
}
