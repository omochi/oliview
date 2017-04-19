#pragma once

#include "./dependency.h"
#include "./assert.h"
#include "./optional.h"

namespace oliview {
    std::string Join(const std::vector<std::string> & array,
                     const std::string & glue);

    Optional<int> Find(const std::string & string,
                       int start_index,
                       const std::vector<std::string> & targets,
                       std::string * found_target);
    Optional<int> FindR(const std::string & string,
                        int start_index,
                        const std::vector<std::string> & targets,
                        std::string * found_target);

    struct SplitOption {
        Optional<int> limit;
        bool include_separator = false;
    };

    std::vector<std::string> Split(const std::string & string,
                                   const std::vector<std::string> & separators,
                                   const SplitOption & option = SplitOption());
    std::vector<std::string> SplitR(const std::string & string,
                                    const std::vector<std::string> & separators,
                                    const SplitOption & option = SplitOption());

}
