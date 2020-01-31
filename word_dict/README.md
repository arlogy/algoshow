Given a dictionary of words built as a **tree of characters**, we want to check
whether dictionary contains a given input word. The following rules must be
taken into account.

- **Zero constraint:** tells whether a given word has been added to the
dictionary (no constraint applies here).
- **Substitution constraint:** same as above but substitutions are allowed. For
instance if `word` is in dictionary, `wood` can also be considered as part of it.
This costs 1 substitution: we have to transform the third letter (`o`) into `r`
to get a word in the dictionary.
- **Substitution/Insertion/Deletion constraint:** same as above but support
insertion and deletion as well. For instance if `word` is in dictionary,
    - `words` can also be considered as part of it. This costs 1 deletion
because we have to remove the last character (`s`).
    - `war` can also be considered as part of it. We just need to replace the
second character (`a`) by `o` and add the missing one (`d`). This costs 2
operations (1 substitution + 1 insertion + 0 deletion).

Detailed comments on implementation can be found in source code.
