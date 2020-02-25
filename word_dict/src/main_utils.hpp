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

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "word_dict.h"

#include <iostream>

void add_words(word_dict &dict, const std::vector<std::string> &words)
{
    for(const std::string &word : words) {
        std::cout << "adding \"" << word << "\" "
                  << (dict.add_word(word) ? "succeeded" : "failed")
                  << std::endl;
    }
}

void match_words(const word_dict &dict, const std::vector<std::string> &words)
{
    const unsigned int min = 0;
    const unsigned int max = 4;
    const std::string &tag1 = "[-] ";
    const std::string &tag2 = "    ";

    for(const std::string &word : words) {
        std::cout << std::endl;

        std::cout << tag1 << dict.match_word_exactly(word).full_str() << std::endl;
        for(unsigned int i = min; i <= max; i++) {
            std::cout << (i == min ? tag1 : tag2) << dict.match_word_allow_substitution(word, i).full_str() << std::endl;
        }
        for(unsigned int i = min; i <= max; i++) {
            std::cout << (i == min ? tag1 : tag2) << dict.match_word_levenshtein_distance(word, i).full_str() << std::endl;
        }
    }
}

void add_sample_words(word_dict &dict)
{
    add_words(dict, {
        "",
        "a",
        "b",
        std::string(1, word_dict::end_of_word_marker()), // KO
        "aba",
        "abb",
        "aaaa",
        "aaaa", // OK but won't be duplicated in tree
    });
}

void match_sample_words(const word_dict &dict)
{
    match_words(dict, {
        "",
        "$",
        "abc",
        "aaaa",
        "zzzz",
    });
}

#endif // MAIN_UTILS_H
