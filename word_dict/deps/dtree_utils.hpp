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

#ifndef DTREE_UTILS_H
#define DTREE_UTILS_H

#include "dtree.hpp"

#include <iostream>

/// Utility class for the templated dtree class.
class dtree_utils
{
public:
    dtree_utils() = delete;

    /// Prints tree starting at root node.
    template<typename T>
    static void print_tree_bracketed(const dtree<T>& tree,
                                     std::ostream& stream = std::cout)
    {
        print_tree_bracketed<T>(tree.root(), stream);
    }

    /// Prints tree starting at the given node. Tree is printed as a node
    /// followed by the set of possible subtrees (one per child node).
    template<typename T>
    static void print_tree_bracketed(const typename dtree<T>::node_t &node,
                                     std::ostream& stream = std::cout)
    {
        const size_t node_index_max = node.number_of_children() - 1;
        size_t node_index = 0;
        for(auto it = node.begin(); it != node.end(); it++, node_index++) {
            print_sub_tree_bracketed(it->second, it->first, stream);
            if(node_index != node_index_max) {
                stream << std::endl;
            }
        }
    }

private:
    template<typename T>
    static void print_sub_tree_bracketed(const typename dtree<T>::node_t &node,
                                         const T &input_from_parent,
                                         std::ostream& stream = std::cout)
    {
        if(!node.has_children()) {
            stream << input_from_parent;
            return;
        }

        stream << input_from_parent << "(";
        const size_t node_index_max = node.number_of_children() - 1;
        size_t node_index = 0;
        for(auto it = node.begin(); it != node.end(); it++, node_index++) {
            print_sub_tree_bracketed(it->second, it->first, stream);
            if(node_index != node_index_max) {
                stream << ", ";
            }
        }
        stream << ")";
    }
};

#endif // DTREE_UTILS_H
