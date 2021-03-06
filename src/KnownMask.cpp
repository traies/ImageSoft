//
// Created by juanfra on 24/04/19.
//
#include <cstdio>
#include "KnownMask.h"

void setupRobertsHorizontal(int* maskSize, float* weights) {
    int _maskSize = 2;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            if(j!=i) {
                weights[i * _maskSize + j] = 0;
            } else if(i==0){
                weights[i * _maskSize + j] = 1;
            } else {
                weights[i * _maskSize + j] = -1;
            }
        }
    }
}


void setupRobertsVertical(int* maskSize, float* weights) {
    int _maskSize = 2;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            if(j==i) {
                weights[i * _maskSize + j] = 0;
            } else if(i==0){
                weights[i * _maskSize + j] = 1;
            } else {
                weights[i * _maskSize + j] = -1;
            }
        }
    }
}
void setupPrewittHorizontal(int* maskSize, float* weights) {
    int _maskSize = 3;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            weights[i * _maskSize + j] = i - 1;
        }
    }
}
void setupPrewittVertical(int* maskSize, float* weights) {
    int _maskSize = 3;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            weights[i * _maskSize + j] = j - 1;
        }
    }
}
void setupSobelHorizontal(int* maskSize, float* weights) {
    int _maskSize = 3;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            if(j==1) {
                weights[i * _maskSize + j] = (i - 1)*2;
            } else {
                weights[i * _maskSize + j] = i - 1;
            }
        }
    }
}

void setupSobelVertical(int* maskSize, float* weights) {
    int _maskSize = 3;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            if(i==1) {
                weights[i * _maskSize + j] = (j - 1)*2;
            } else {
                weights[i * _maskSize + j] = j - 1;
            }
        }
    }
}

void setupHouseMask(int* maskSize, float* weights) {
    int _maskSize = 3;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            if(i==0) {
                weights[i * _maskSize + j] = 1;
            } else if(i==2) {
                weights[i * _maskSize + j] = -1;
            } else {
                if(j==1) {
                    weights[i * _maskSize + j] = -2;
                } else {
                    weights[i * _maskSize + j] = 1;
                }
            }
        }
    }
}

void setupKirsh(int* maskSize, float* weights) {
    int _maskSize = 3;
    (*maskSize) = _maskSize;
    for (int i = 0; i < _maskSize; i++) {
        for (int j = 0; j < _maskSize; j++) {
            if(i==0) {
                weights[i * _maskSize + j] = 5;
            } else if(i==2) {
                weights[i * _maskSize + j] = -3;
            } else {
                if(j==1) {
                    weights[i * _maskSize + j] = 0;
                } else {
                    weights[i * _maskSize + j] = -3;
                }
            }
        }
    }
}


void clearMask(int maskSize, float* weights) {
    for (int i = 0; i < maskSize; i++) {
        for (int j = 0; j < maskSize; j++) {
            weights[i * maskSize + j] = 0;
        }
    }
}

void RotateMask(int maskSize, float* weights) {
    float *w = weights;
    if(maskSize%2 == 0) {
        for (int x = 0; x < maskSize / 2; x++) {
            for (int y = x; y < maskSize-x-1; y++) {
                // store current cell in temp variable
                float temp = w[x * maskSize + y];

                // move values from right to top
                w[x* maskSize + y] = w[y* maskSize + maskSize-1-x];

                // move values from bottom to right
                w[y* maskSize + maskSize-1-x] = w[(maskSize-1-x)* maskSize + maskSize-1-y];

                // move values from left to bottom
                w[(maskSize-1-x)* maskSize + maskSize-1-y] = w[(maskSize-1-y)* maskSize + x];

                // assign temp to left
                w[(maskSize-1-y)* maskSize + x] = temp;
            }
        }

    } else {
        int fromX;
        int fromY;
        int toX;
        int toY;
            for (int x = 0; x < maskSize / 2; x++) {

                for (int y = x; y < (maskSize-x-1)/2; y++) {
                    float temp = w[x* maskSize + y];

                    // move values from right to top
                    
                    toX = x;
                    toY = y;
                    fromX = y;
                    fromY = (maskSize-1-x)/2;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];

                    toX = fromX;
                    toY = fromY;
                    fromX = y;
                    fromY = maskSize-1-x;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];

                    // move values from bottom to right
                    toX = fromX;
                    toY = fromY;
                    fromX = (maskSize-1-y)/2;
                    fromY = maskSize-1-x;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];


                    toX = fromX;
                    toY = fromY;
                    fromX = maskSize-1-x;
                    fromY = maskSize-1-y;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];

                    // move values from left to bottom
                    toX = fromX;
                    toY = fromY;
                    fromX = maskSize-1-x;
                    fromY = (maskSize-1-y)/2;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];

                    toX = fromX;
                    toY = fromY;
                    fromX = maskSize-1-y;
                    fromY = x;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];

                    // assign temp to left
                    toX = fromX;
                    toY = fromY;
                    fromX = (maskSize-1-y)/2;
                    fromY = x;
                    w[toX * maskSize + toY] = w[fromX * maskSize + fromY];

                    toX = fromX;
                    toY = fromY;
                    w[toX * maskSize + toY] = temp;
                }
            }
        }
    }

