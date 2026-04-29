#ifndef ALLOWEDS_H
#define ALLOWEDS_H

#include "types.h"
#include "hash.h"

int allHorizontalWordsAreValid(HashSet* dict, State state);
int allVerticalWordsAreValid(HashSet* dict, State state);
void calculateAlloweds(HashSet* dict, Allowed output[][15], State state, Rack rack);

#endif
