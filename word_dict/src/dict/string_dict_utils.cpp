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

#include "string_dict_utils.h"

#include "dtree_utils.hpp"

#include <algorithm>
#include <stack>

const char string_dict_utils::tree_end_of_string_marker {'$'}; // any character (excluding those used in tree structure) will do just fine

bool string_dict_utils::add_string(dtree<char> &tree, const std::string &str)
{
    if(str.find(string_dict_utils::tree_end_of_string_marker) != std::string::npos) {
        return false; // string must not contain tree_end_of_string_marker
    }

    dtree<char>::node_t *node = &tree.root();
    for(const char c : str + string_dict_utils::tree_end_of_string_marker) {
        node = &node->set_child(c);
    }
    return true;
}

string_dict_utils::match_data string_dict_utils::match_string_exactly(const dtree<char> &tree,
                                                                      const std::string &str)
{
    // The algorithm below might be recursive but we prefer it iterative.
    //
    // Logic: we keep reading characters from tree until success (all characters
    //        in the given string have been read from tree, including the
    //        tree_end_of_string_marker) or failure (one character cannot be
    //        read).
    //
    // Complexity: roughly O(n * min(l, L)) where
    //                 n = number of children of the node with the widest
    //                     offspring in tree (this value is bounded by 256 for
    //                     char type and n is reduced to log(n) with the dtree
    //                     tree implementation)
    //                 l = length of the given string
    //                 L = length of the longest string in tree (it is
    //                     effectively the same as height of tree)
    //
    // Side notes: iterative version of algorithm is the fastest.

    typedef unsigned int uint;

    const std::string &s = str + string_dict_utils::tree_end_of_string_marker;
    uint s_nb_chars_read = 0;
    const uint s_len = s.length();

    auto *node = &tree.root();
    do {
        node = node->child_ptr(s.at(s_nb_chars_read));
        if(node) {
            s_nb_chars_read++;
        }
    }
    while(node && s_nb_chars_read < s_len);

    string_dict_utils::match_data match;
    match.set(
        "exact-match",
        str,
        s_nb_chars_read == s_len,
        [&]() { return "\"" + s + "\" matched successfully"; },
        [&]() { return "\"" + s + "\" failed to match at '"
                     + s.at(s_nb_chars_read) + "' after reading \""
                     + s.substr(0, s_nb_chars_read) + "\" successfully"; }
    );
    return match;
}

string_dict_utils::match_data string_dict_utils::match_string_allow_substitution(const dtree<char> &tree,
                                                                                 const std::string &str,
                                                                                 unsigned int subst_max)
{
    // The algorithm below might be recursive but we prefer it iterative.
    //
    // Logic: we compute the number of substitutions for each node in tree and
    //        yield success (when a string matching the given substitution
    //        criteria is read) or failure (in case no string in tree matches
    //        the given criteria).
    //
    // Complexity: O(number_of_nodes_in_tree) or roughly O(n ^ min(l, L)) where
    //                 n = number of children of the node with the widest
    //                     offspring in tree (this value is bounded by 256 for
    //                     char type and n is reduced to log(n) with the dtree
    //                     tree implementation)
    //                 l = length of the given string
    //                 L = length of the longest string in tree (it is
    //                     effectively the same as height of tree)
    //
    // Side notes: see (1) at the bottom of this file.

    typedef unsigned int uint;

    const std::string &s = str + string_dict_utils::tree_end_of_string_marker;
    const uint s_len = s.length();
    bool s_matched {false};
    std::string s_matched_string;
    uint s_matched_string_cost {0};

    // Set first tree node to visit.
    std::stack<
            std::tuple<const dtree<char>::node_t*, std::string, uint, uint>
            > unvisited_nodes;
    unvisited_nodes.push(
        std::make_tuple(&tree.root(), "", 0, 0)
    );

    // Start visiting.
    while(!s_matched && !unvisited_nodes.empty()) {
        // Pick one tree node to visit.
        const dtree<char>::node_t* prev_node;
        std::string prev_read_string;
        uint prev_nb_chars_read;
        uint prev_subst_cost;
        std::tie(prev_node,
                 prev_read_string,
                 prev_nb_chars_read,
                 prev_subst_cost) = unvisited_nodes.top();
        unvisited_nodes.pop();

        const char expected_char = s.at(prev_nb_chars_read);

        // Visit picked tree node.
        for(auto it = prev_node->begin(); it != prev_node->end(); it++) {
            const std::string &curr_read_string = prev_read_string + it->first;
            const uint curr_nb_chars_read = prev_nb_chars_read + 1;

            // Decide whether one must substitute or not.
            if(expected_char == it->first) {
                if(curr_nb_chars_read == s_len) {
                    if(prev_subst_cost <= subst_max) {
                        s_matched = true;
                        s_matched_string = curr_read_string;
                        s_matched_string_cost = prev_subst_cost;
                        break;
                    }
                }
                else {
                    if(curr_nb_chars_read < s_len) {
                        unvisited_nodes.push(
                            std::make_tuple(
                                &it->second,
                                curr_read_string,
                                curr_nb_chars_read,
                                prev_subst_cost // 0 substitution needed
                            )
                        );
                    }
                }
            }
            else {
                if(curr_nb_chars_read < s_len) {
                    unvisited_nodes.push(
                        std::make_tuple(
                            &it->second,
                            curr_read_string,
                            curr_nb_chars_read,
                            prev_subst_cost + 1 // 1 substitution needed
                        )
                    );
                }
            }
        }
    }

    string_dict_utils::match_data match;
    match.set(
        "subst-match(" + std::to_string(subst_max) + ")",
        str,
        s_matched,
        [&]() { return "\"" + s + "\" matched successfully with \""
                      + s_matched_string + "\" using "
                      + std::to_string(s_matched_string_cost) + " substs"; },
        [&]() { return "\"" + s + "\" failed to match"; }
    );
    return match;
}

string_dict_utils::match_data string_dict_utils::match_string_levenshtein_distance(const dtree<char> &tree,
                                                                                   const std::string &str,
                                                                                   unsigned int edit_max)
{
    // The algorithm below might be recursive but we prefer it iterative.
    //
    // Logic: we compute the Levenshtein distance from all strings in tree to
    //        the given string and yield success when we reach a string with an
    //        edit cost lower or equal to the given limit. In case no such
    //        string exists in tree, then certainly the given string cannot be
    //        matched.
    //
    // For instance the matrix to compute Lev("tea", "meat") is filled as
    // pictured below. And apart from the first picture which refers to the
    // initialization step, each cell is computed as follows:
    //     Cell[i][j] = min(
    //         Cell[i-1][j] + 1,
    //         Cell[i][j-1] + 1,
    //         Cell[i-1][j-1] + k (where k is either 0 or 1, depending whether
    //                             letters at i and j are identical)
    //     )
    // More information about the Levenshtein distance on the internet.
    //
    //     m e a t         m e a t         m e a t         m e a t
    //  |0|1|2|3|4|     |0|1|2|3|4|     |0|1|2|3|4|     |0|1|2|3|4|
    // t|1| | | | |    t|1|1|2|3|3|    t|1|1|2|3|3|    t|1|1|2|3|3|
    // e|2| | | | |    e|2| | | | |    e|2|2|1|2|3|    e|2|2|1|2|3|
    // a|3| | | | |    a|3| | | | |    a|3| | | | |    a|3|3|2|1|2|
    //
    // So Lev("tea", "meat") = bottom-right value in matrix = 2.
    //
    // Notice that as we fill the matrix vertically (initialization excluded)
    // only the last row changes and its values depend on those in the previous
    // row. That is: computing Lev("tea", "meat") depends on Lev("te", "meat")
    // which depends on Lev("t", "meat") which depends on Lev("", "meat"). This
    // is how we will compute the Levenshtein distance from any string in tree
    // to the given string, starting at first character in tree down to leaf
    // nodes.
    //
    // Complexity: O(length_of_given_string * nb_of_nodes_in_tree) or roughly
    //             O(length_of_given_string * n ^ min(l, L)) where
    //                 n = number of children of the node with the widest
    //                     offspring in tree (this value is bounded by 256 for
    //                     char type and n is reduced to log(n) with the dtree
    //                     tree implementation)
    //                 l = length of the given string
    //                 L = length of the longest string in tree (it is
    //                     effectively the same as height of tree)
    //
    // Side notes: see (1) at the bottom of this file.

    typedef unsigned int uint;
    typedef std::vector<uint> uint_vector;

    const std::string &s = str + string_dict_utils::tree_end_of_string_marker;
    bool s_matched {false};
    std::string s_matched_string;
    uint s_matched_string_cost {0};

    uint_vector s_lev_row; // first row in Levenshtein distance matrix
    const uint s_lev_row_size = s.length() + 1;
    for(uint i=0; i<s_lev_row_size; i++) {
        s_lev_row.push_back(i);
    }

    // Set first tree node to visit.
    std::stack<
            std::tuple<const dtree<char>::node_t*, std::string, uint_vector>
            > unvisited_nodes;
    unvisited_nodes.push(
        std::make_tuple(&tree.root(), "", s_lev_row)
    );

    // Start visiting.
    while(!s_matched && !unvisited_nodes.empty()) {
        // Pick one tree node to visit.
        const dtree<char>::node_t* prev_node;
        std::string prev_read_string;
        uint_vector prev_lev_row;
        std::tie(prev_node,
                 prev_read_string,
                 prev_lev_row) = unvisited_nodes.top();
        unvisited_nodes.pop();

        const uint prev_lev_row_size = prev_lev_row.size();

        // Visit picked tree node.
        for(auto it = prev_node->begin(); it != prev_node->end(); it++) {
            const std::string &curr_read_string = prev_read_string + it->first;

            // Compute current row in Levenshtein distance matrix.
            uint_vector curr_lev_row;
            curr_lev_row.push_back(prev_lev_row.at(0) + 1);
            for(uint i = 1; i < prev_lev_row_size; i++) {
                curr_lev_row.push_back(
                    std::min({
                        curr_lev_row.at(i-1) + 1, // insertion cost
                        prev_lev_row.at(i) + 1, // deletion cost
                        prev_lev_row.at(i-1) + (it->first == s.at(i-1) ? 0 : 1), // substitution cost
                    })
                );
            }

            // Check if we have reached a string matching the given edit distance criteria.
            const uint curr_lev_row_goal_cost = curr_lev_row.at(curr_lev_row.size()-1);
            if(curr_lev_row_goal_cost <= edit_max
            && it->first == string_dict_utils::tree_end_of_string_marker) {
                s_matched = true;
                s_matched_string = curr_read_string;
                s_matched_string_cost = curr_lev_row_goal_cost;
            }

            // Save tree node for later visit in case maximal edit cost hasn't
            // been reached yet (indeed next time we will be adding either 0 or
            // 1 to the costs in the computed Levenshtein distance matrix's row).
            const uint curr_lev_row_min_cost = *std::min_element(
                curr_lev_row.begin(),
                curr_lev_row.end()
            );
            if(curr_lev_row_min_cost <= edit_max) {
                unvisited_nodes.push(
                    std::make_tuple(
                        &it->second,
                        curr_read_string,
                        curr_lev_row
                    )
                );
            }

            // Check early break (no need to continue in case of match).
            if(s_matched) {
                break;
            }
        }
    }

    string_dict_utils::match_data match;
    match.set(
        "leven-match(" + std::to_string(edit_max) + ")",
        str,
        s_matched,
        [&]() { return "\"" + s + "\" matched successfully with \""
                     + s_matched_string + "\" using "
                     + std::to_string(s_matched_string_cost) + " edits"; },
        [&]() { return "\"" + s + "\" failed to match"; }
    );
    return match;
}

void string_dict_utils::fetch_tree_strings(const dtree<char> &tree,
                                           std::vector<std::string> &strings)
{
    strings.clear();
    string_dict_utils::fetch_tree_strings(tree, [&strings](const std::string &str) {
        strings.push_back(str);
    });
}

void string_dict_utils::fetch_tree_strings(const dtree<char> &tree,
                                           const std::function<void (const std::string &)> &callback)
{
    std::string acc;
    string_dict_utils::fetch_tree_strings(tree.root(), acc, callback);
}

void string_dict_utils::fetch_tree_strings(const dtree<char>::node_t &node,
                                           std::string &acc,
                                           const std::function<void (const std::string &)> &callback) {
    // The algorithm below is recursive but we don't care because this function
    // is provided for debugging purpose only (print tree content for instance).

    for(auto it = node.begin(); it != node.end(); it++) {
        acc += it->first;
        if(it->second.has_children()) {
            string_dict_utils::fetch_tree_strings(it->second, acc, callback);
        }
        else {
            callback(acc);
        }
        acc.pop_back();
    }
}

void string_dict_utils::print_tree_structure(const dtree<char> &tree,
                                             std::ostream &stream)
{
    dtree_utils::print_tree_bracketed(tree, stream);
    stream << std::endl;
}

void string_dict_utils::print_tree_strings(const dtree<char> &tree,
                                           std::ostream &stream)
{
    string_dict_utils::fetch_tree_strings(tree, [&stream](const std::string &str) {
        stream << str << std::endl;
    });
}

// (1) When we think of it again it is unsure which version of this algorithm is
//     the fastest. Indeed in the recursive version the call stack will never
//     contain more than x elements (when x refers to the length of the longest
//     string in tree). But the iterative version might end up storing all nodes
//     in tree into a stack. So both versions might be tested and compared in
//     the future (assuming a recursive version is also provided).
