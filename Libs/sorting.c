#include "voiture.h"
#include "sorting.h"

void sortSection (Voiture* sortedArr[], int section, Voiture* buffer[], int length, int amount) {
    Voiture* sortingArr[length];
    int max;
    Voiture* temp;

    copyArr(sortedArr, sortingArr, length);

    for (int i=0;i<amount;i++) {
        max = i;
        for (int j=i+1;j<length;j++){
            if (sortingArr[j]->sections[section] < sortingArr[max]->sections[section]){
                max = j;
            }
        }
        if (max != i) {
            temp = sortingArr[i];
            sortingArr[i] = sortingArr[max];
            sortingArr[max] = temp;
        }
        buffer[i] = sortingArr[i];
    }
}

void copyArr(Voiture* origin[], Voiture* OA[], int length) {
    for (int i=0;i<length;i++) {
        OA[i] = origin[i];
    }

}

void generateOrderedArr(Voiture voitures[], Voiture* OA[], int length) {
    for (int i=0;i<length;i++) {
        OA[i] = &voitures[i];
    }
}

void sortLapTime (Voiture* sortingArr[], int length) {
    int max;
    Voiture* temp;

    for (int i=0;i<length;i++) {
        max = i;
        for (int j=i+1;j<length;j++){
            if (sortingArr[j]->bestLap < sortingArr[max]->bestLap){
                max = j;
            }
        }
        if (max != i) {
            temp = sortingArr[i];
            sortingArr[i] = sortingArr[max];
            sortingArr[max] = temp;
        }
    }
}
