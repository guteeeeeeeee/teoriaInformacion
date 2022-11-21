#include <stdio.h>
#include <stdlib.h>
#include "teoria2.h"

int main()
{
    //parte1
    procesar();
    //parte2
    double emision1[5],canal1[5][3],emision2[4],canal2[4][4],emision3[6],canal3[6][4];
    emision1[0] = 0.2; emision1[1] = 0.1; emision1[2] = 0.3; emision1[3] = 0.3; emision1[4] = 0.1;
    emision2[0] = 0.25; emision2[1] = 0.33; emision2[2] = 0.27; emision2[3] = 0.15;
    emision3[0] = 0.15; emision3[1] = 0.1; emision3[2] = 0.2; emision3[3] = 0.25; emision3[4] = 0.14; emision3[5] = 0.16;

    canal1[0][0] = 0.3; canal1[0][1] = 0.3; canal1[0][2] = 0.4;
    canal1[1][0] = 0.4; canal1[1][1] = 0.4; canal1[1][2] = 0.2;
    canal1[2][0] = 0.3; canal1[2][1] = 0.3; canal1[2][2] = 0.4;
    canal1[3][0] = 0.3; canal1[3][1] = 0.4; canal1[3][2] = 0.3;
    canal1[4][0] = 0.3; canal1[4][1] = 0.4; canal1[4][2] = 0.3;

    canal2[0][0] = 0.2; canal2[0][1] = 0.3; canal2[0][2] = 0.2; canal2[0][3] = 0.3;
    canal2[1][0] = 0.3; canal2[1][1] = 0.3; canal2[1][2] = 0.2; canal2[1][3] = 0.2;
    canal2[2][0] = 0.3; canal2[2][1] = 0.2; canal2[2][2] = 0.2; canal2[2][3] = 0.3;
    canal2[3][0] = 0.3; canal2[3][1] = 0.3; canal2[3][2] = 0.3; canal2[3][3] = 0.1;

    canal3[0][0] = 0.2; canal3[0][1] = 0.3; canal3[0][2] = 0.2; canal3[0][3] = 0.3;
    canal3[1][0] = 0.3; canal3[1][1] = 0.3; canal3[1][2] = 0.3; canal3[1][3] = 0.1;
    canal3[2][0] = 0.2; canal3[2][1] = 0.2; canal3[2][2] = 0.3; canal3[2][3] = 0.2;
    canal3[3][0] = 0.3; canal3[3][1] = 0.3; canal3[3][2] = 0.2; canal3[3][3] = 0.2;
    canal3[4][0] = 0.2; canal3[4][1] = 0.3; canal3[4][2] = 0.3; canal3[4][3] = 0.2;
    canal3[5][0] = 0.2; canal3[5][1] = 0.3; canal3[5][2] = 0.3; canal3[5][3] = 0.2;

    //printf("CANAL 1: \n");
    calcularEquivocacion(1,5,3,emision1,canal1);
    //printf("\nCANAL 2: \n");
    calcularEquivocacion(2,4,4,emision2,canal2);
    //printf("\nCANAL 3: \n");
    calcularEquivocacion(3,6,4,emision3,canal3);
    return 0;
}
