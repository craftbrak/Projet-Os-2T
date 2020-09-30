#include <stdio.h>
#include "voiture.h"
#include "sorting.h"

void sortSection (Voiture voitures[], int section, int buffer[], int length) {
    int max;
    int temp;

    generateOrderedIndex(buffer, length);
    
    for (int i=0;i<length;i++){
        max = i;
        for (int j=i+1;j<length;j++){
            if (voitures[buffer[j]].sections[section] < voitures[buffer[max]].sections[section]){
                max = j;
            }
        }
        if (max != i) {
            temp = buffer[i];
            buffer[i] = buffer[max];
            buffer[max] = temp;
        }
    }
}

void generateOrderedIndex(int OI[], int length) {
    for (int i=0;i<length;i++) {
        OI[i] = i;
    }
}
