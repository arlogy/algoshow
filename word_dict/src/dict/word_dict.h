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

#ifndef WORD_DICT_H
#define WORD_DICT_H

#include "string_dict_utils.h"

/// Dictionary of words (strings).
class word_dict
{
public:
    explicit word_dict() {}

    bool add_word(const std::string &word);

    string_dict_utils::match_data match_word_exactly(const std::string &word) const;
    string_dict_utils::match_data match_word_allow_substitution(const std::string &word,
                                                                unsigned int subst_max = 0) const;
    string_dict_utils::match_data match_word_levenshtein_distance(const std::string &word,
                                                                  unsigned int edit_max = 0) const;

    void fetch_words(std::vector<std::string> &words) const;
    void print_words_tree(std::ostream &stream) const;
    void print_words_values(std::ostream &stream) const;

    static char end_of_word_marker();

private:
    dtree<char> m_words;
};

#endif // WORD_DICT_H
