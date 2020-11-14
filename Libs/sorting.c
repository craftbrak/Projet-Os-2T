#include "voiture.h"
#include "sorting.h"

void sortSection(Voiture voitures[], int sortedArr[], int section, int buffer[], int length, int amount) {
    int sortingArr[length];
    int max, temp;

    copyArr(sortedArr, sortingArr, length);

    for (int i = 0; i < amount; i++) {
        max = i;
        for (int j = i + 1; j < length; j++) {
            if (voitures[sortingArr[j]].sections[section] > 0 &&
                    (voitures[sortingArr[max]].sections[section] < 0 ||
                     voitures[sortingArr[j]].sections[section] < voitures[sortingArr[max]].sections[section])) {
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

void copyArr(int origin[], int OA[], int length) {
    for (int i = 0; i < length; i++) {
        OA[i] = origin[i];
    }

}

void generateOrderedArr(int OA[], int length) {
    for (int i = 0; i < length; i++) {
        OA[i] = i;
    }
}

void sortLapTime(Voiture voitures[], int sortingArr[], int length) {
    int max, temp;

    for (int i = 0; i < length; i++) {
        max = i;
        for (int j = i + 1; j < length; j++) {
            if (voitures[sortingArr[j]].bestLap > 0 &&
                    (voitures[sortingArr[max]].bestLap < 0 ||
                     voitures[sortingArr[j]].bestLap < voitures[sortingArr[max]].bestLap)) {
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

int isOut(Voiture voiture, int qteSections) {
    return voiture.done && voiture.qteSections < qteSections;
}

void sortSpeed(Voiture voitures[], int sortingArr[], int length) {
    int max, temp;

    for (int i = 0; i < length; i++) {
        max = i;
        for (int j = i + 1; j < length; j++) {
            if (!voitures[sortingArr[j]].out &&
                    (voitures[sortingArr[max]].out ||
                     voitures[sortingArr[j]].speed > voitures[sortingArr[max]].speed)) {
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

double getMinTime(Voiture voitures[], int sortedArr[], int length) {
    double min = -1.0;
    for (int i = 0; i < length; i++) {
        if ((voitures + sortedArr[i])->done == 0 && (min < 0 || (voitures + sortedArr[i])->TotalTime < min)) {
            min = (voitures + sortedArr[i])->TotalTime;
        }
    }
    return min;
}
