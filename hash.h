#ifndef HASHSET_H
#define HASHSET_H

#define TABLE_SIZE 600013

typedef struct {
    char **buckets;
    int size;
} HashSet;

HashSet *load_dictionary(const char *filename);
int      is_valid_word(HashSet *set, const char *word);
void     free_set(HashSet *set);

#endif