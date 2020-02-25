/*
 MIT License

 Copyright (c) 2020 Fadyl Sokenou https://github.com/arlogy

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef STRING_DICT_UTILS_H
#define STRING_DICT_UTILS_H

#include "dtree.hpp"

#include <functional>
#include <vector>

/// Utility class for dictionary of strings implemented as tree of characters
/// (more precisely dtree<char>).
class string_dict_utils
{
public:
    typedef struct {
        std::string algorithm; // name (or short description) of the matching algorithm used
        std::string source;    // string to match in tree
        bool success {false};  // has string been matched?
        std::string message;   // message regarding success or failure

        // a convenient initialization function to avoid duplicates in code
        void set(const std::string &algorithm,
                 const std::string &source,
                 bool success,
                 const std::function<std::string ()> &message_success, // see (1) below
                 const std::function<std::string ()> &message_failure)
        {
            this->algorithm = algorithm;
            this->source = source;
            this->success = success;
            this->message = this->success ? message_success() : message_failure();

            // (1) We use a lambda instead of directly passing the message string
            //     because sometimes we might not want the code which build the
            //     success-message to run in case of failure (and vice versa, to
            //     prevent out-of-range exception for instance).
        }

        // convenient informative functions
        std::string short_str() const
        {
            return "running " + algorithm
                 + " on \"" + source + "\" "
                 + (success ? "succeeded" : "failed")
            ;
        }
        std::string full_str() const { return short_str() + ": " + message; }
    } match_data;

public:
    string_dict_utils() = delete;

    /// Adds string to tree. Note that string won't be added in case it contains
    /// the string_dict::tree_end_of_string_marker character.
    static bool add_string(dtree<char> &tree, const std::string &str);

    /// Least permissive string-matching-algorithm. Fastest. See comments on
    /// complexity in *.cpp file.
    static match_data match_string_exactly(const dtree<char> &tree,
                                           const std::string &str);
    /// Less permissive than the string-matching-algorithm using the Levenshtein
    /// distance. Faster than the said algorithm limited to substitutions only
    /// (i.e. no insertion or deletion). See comments on complexity in *.cpp
    /// file.
    static match_data match_string_allow_substitution(const dtree<char> &tree,
                                                      const std::string &str,
                                                      unsigned int subst_max = 0);
    /// Most permissive string-matching-algorithm. Slowest. See comments on
    /// complexity in *.cpp file. Note that this function allows substitution,
    /// insertion and deletion of characters.
    static match_data match_string_levenshtein_distance(const dtree<char> &tree,
                                                        const std::string &str,
                                                        unsigned int edit_max = 0);

    static void fetch_tree_strings(const dtree<char> &tree,
                                   std::vector<std::string> &strings);
    static void fetch_tree_strings(const dtree<char> &tree,
                                   const std::function<void (const std::string &)> &callback);
    static void fetch_tree_strings(const dtree<char>::node_t &node,
                                   std::string &acc,
                                   const std::function<void (const std::string &)> &callback);

    static void print_tree_structure(const dtree<char> &tree, std::ostream &stream);
    static void print_tree_strings(const dtree<char> &tree, std::ostream &stream);

public:
    static const char tree_end_of_string_marker;
};

#endif // STRING_DICT_UTILS_H
