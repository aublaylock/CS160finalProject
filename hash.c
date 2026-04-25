#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

static unsigned long hash(const char *str) {
    unsigned long h = 5381;
    while (*str)
        h = (h << 5) + h + *str++;
    return h % TABLE_SIZE;
}

HashSet *load_dictionary(const char *filename) {
    HashSet *set = malloc(sizeof(HashSet));
    set->size = TABLE_SIZE;
    set->buckets = calloc(TABLE_SIZE, sizeof(char *));

    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen"); exit(1); }

    char word[64];
    while (fscanf(f, "%63s", word) == 1) {
        unsigned long idx = hash(word);
        while (set->buckets[idx])
            idx = (idx + 1) % TABLE_SIZE;
        set->buckets[idx] = strdup(word);
    }

    fclose(f);
    return set;
}

int is_valid_word(HashSet *set, const char *word) {
    unsigned long idx = hash(word);
    while (set->buckets[idx]) {
        if (strcmp(set->buckets[idx], word) == 0) return 1;
        idx = (idx + 1) % TABLE_SIZE;
    }
    return 0;
}

void free_set(HashSet *set) {
    for (int i = 0; i < TABLE_SIZE; i++)
        if (set->buckets[i]) free(set->buckets[i]);
    free(set->buckets);
    free(set);
}