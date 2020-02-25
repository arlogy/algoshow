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

#include "word_dict.h"

bool word_dict::add_word(const std::string &word)
{
    return string_dict_utils::add_string(m_words, word);
}

string_dict_utils::match_data word_dict::match_word_exactly(const std::string &word) const
{
    return string_dict_utils::match_string_exactly(m_words, word);
}

string_dict_utils::match_data word_dict::match_word_allow_substitution(const std::string &word,
                                                                       unsigned int subst_max) const
{
    return string_dict_utils::match_string_allow_substitution(
        m_words,
        word,
        subst_max
    );
}

string_dict_utils::match_data word_dict::match_word_levenshtein_distance(const std::string &word,
                                                                         unsigned int edit_max) const
{
    return string_dict_utils::match_string_levenshtein_distance(
        m_words,
        word,
        edit_max
    );
}

void word_dict::fetch_words(std::vector<std::string> &words) const
{
    string_dict_utils::fetch_tree_strings(m_words, words);
}

void word_dict::print_words_tree(std::ostream &stream) const
{
    string_dict_utils::print_tree_structure(m_words, stream);
}

void word_dict::print_words_values(std::ostream &stream) const
{
    string_dict_utils::print_tree_strings(m_words, stream);
}

char word_dict::end_of_word_marker()
{
    return string_dict_utils::tree_end_of_string_marker;
}
