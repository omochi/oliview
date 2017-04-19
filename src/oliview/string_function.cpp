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

    bool StartWith(const std::string & string,
                   const std::string & test)
    {
        if (string.size() < test.size()) {
            return false;
        }
        return string.substr(0, test.size()) == test;
    }

    bool EndWith(const std::string & string,
                 const std::string & test)
    {
        if (string.size() < test.size()) {
            return false;
        }
        return string.substr(string.size() - test.size(), test.size()) == test;
    }

    Optional<int> Find(const std::string & string,
                       int start_index,
                       const std::vector<std::string> & targets,
                       std::string * found_target)
    {
        for (int i = start_index; i < string.size(); i++) {
            for (auto & target : targets) {
                if (i + target.size() > string.size()) {
                    continue;
                }
                auto test_str = string.substr(i, target.size());
                if (test_str == target) {
                    if (found_target) {
                        *found_target = target;
                    }
                    return Some(i);
                }
            }
        }
        return nullptr;
    }

    Optional<int> FindR(const std::string & string,
                        int start_index,
                        const std::vector<std::string> & targets,
                        std::string * found_target)
    {
        for (int i = start_index; i >= 0; i--) {
            for (auto & target : targets) {
                int test_index = i - (int)target.size();
                if (test_index < 0) {
                    continue;
                }
                auto test_str = string.substr(test_index, target.size());
                if (test_str == target) {
                    if (found_target) {
                        *found_target = target;
                    }
                    return Some(test_index);
                }
            }
        }
        return nullptr;
    }

    std::vector<std::string> Split(const std::string & string,
                                   const std::string & separator,
                                   const SplitOption & option)
    {
        return Split(string,
                     std::vector<std::string>{ separator },
                     option);
    }

    std::vector<std::string> Split(const std::string & string,
                                   const std::vector<std::string> & separators,
                                   const SplitOption & option)
    {
        if (option.limit) {
            OLIVIEW_ASSERT(*option.limit >= 0);
        }

        if (string.size() == 0) {
            return {};
        }
        if (option.limit) {
            if (*option.limit == 0) {
                return {};
            }
        }

        std::vector<std::string> elements;
        int last_pos = 0;

        while (true) {
            if (option.limit) {
                if (elements.size() / 2 + 1 >= *option.limit) {
                    break;
                }
            }

            std::string found_separator;
            auto found_pos = Find(string,
                                  last_pos,
                                  separators,
                                  &found_separator);
            if (!found_pos) {
                break;
            }

            int separator_pos = *found_pos;
            int separator_length = (int)found_separator.size();
            int element_pos = last_pos;
            int element_length = separator_pos - element_pos;

            elements.push_back(string.substr(element_pos, element_length));
            elements.push_back(string.substr(separator_pos, separator_length));

            last_pos = separator_pos + separator_length;
        }

        elements.push_back(string.substr(last_pos, string.size() - last_pos));

        std::vector<std::string> ret;
        for (int i = 0; i < elements.size(); i += 2) {
            std::string str = elements[i];
            if (option.include_separator && i + 1 < elements.size()) {
                str += elements[i + 1];
            }
            ret.push_back(str);
        }
        return ret;
    }

    std::vector<std::string> SplitR(const std::string & string,
                                    const std::string & separator,
                                    const SplitOption & option)
    {
        return SplitR(string,
                      std::vector<std::string>{ separator },
                      option);
    }

    std::vector<std::string> SplitR(const std::string & string,
                                    const std::vector<std::string> & separators,
                                    const SplitOption & option)
    {
        if (option.limit) {
            OLIVIEW_ASSERT(*option.limit >= 0);
        }

        if (string.size() == 0) {
            return {};
        }
        if (option.limit) {
            if (*option.limit == 0) {
                return {};
            }
        }

        std::vector<std::string> elements;
        int last_pos = (int)string.size();
        std::string last_found_separator;

        while (true) {
            if (option.limit) {
                if (elements.size() / 2 + 1 >= *option.limit) {
                    break;
                }
            }

            std::string found_separator;
            auto found_pos = FindR(string,
                                   last_pos,
                                   separators,
                                   &found_separator);
            if (!found_pos) {
                break;
            }

            int separator_pos = *found_pos;
            int separator_length = (int)found_separator.size();
            int element_pos = separator_pos + separator_length;
            int element_length = last_pos - element_pos;

            elements.push_back(string.substr(element_pos, element_length));
            elements.push_back(string.substr(separator_pos, separator_length));

            last_pos = separator_pos;
        }

        elements.push_back(string.substr(0, last_pos));
        
        std::reverse(elements.begin(), elements.end());

        std::vector<std::string> ret;
        for (int i = 0; i < elements.size(); i += 2) {
            std::string str = elements[i];
            if (option.include_separator && i + 1 < elements.size()) {
                str += elements[i + 1];
            }
            ret.push_back(str);
        }

        return ret;
    }
}
