#include "./result.h"

namespace oliview {
    ResultFailureValue Failure(const Ref<Error> & error) {
        return ResultFailureValue { error };
    }
}
