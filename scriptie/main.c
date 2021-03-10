#include <stdio.h>
#include <stdint-gcc.h>
#include <time.h>
#include <stdlib.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

/*Round constants*/
const unsigned char GIFT_RC[40] = {
        0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F,
        0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B,
        0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E,
        0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A
};

const unsigned char permutation[128] = {
        0, 33, 66, 99, 96, 1, 34, 67, 64, 97, 2, 35, 32, 65, 98, 3,
        4, 37, 70, 103, 100, 5, 38, 71, 68, 101, 6, 39, 36, 69, 102, 7,
        8, 41, 74, 107, 104, 9, 42, 75, 72, 105, 10, 43, 40, 73, 106,11,
        12, 45, 78, 111, 108, 13, 46, 79, 76, 109, 14, 47, 44, 77, 110, 15,
        16, 49, 82, 115, 112, 17, 50, 83, 80, 113, 18, 51, 48, 81, 114, 19,
        20, 53, 86, 119, 116, 21, 54, 87, 84, 117, 22, 55, 52, 85, 118, 23,
        24, 57, 90, 123, 120, 25, 58, 91, 88, 121, 26, 59, 56, 89, 122, 27,
        28, 61, 94, 127, 124, 29, 62, 95, 92, 125, 30, 63, 60, 93, 126, 31
};
void binaryRepresentation8bit(uint8_t word, uint8_t resultlist[8]) {
    for (int i = 0; i< 8; i++) {
        resultlist[7-i] = word >> i &1;
    }
}


void binaryRepresentation2(uint32_t word, uint8_t resultlist[32]) {
    for (int i = 0; i< 32; i++) {
        resultlist[31-i] = word >> i &1;
    }
}

uint16_t decimalRepresentation2(uint8_t list[32]) {
    uint32_t word = 0;
    uint32_t deler = 2147483648;
    for(int j = 0; j < 32; j ++) {
        if (list[j] == 1) {
            word = word + deler;
        }
        deler = deler/2;
    }
    return word;
}

void binaryRepresentation(uint8_t list[16], uint8_t resultList[128]) {
    uint8_t counter = 0;
    uint8_t counter2 = 7;
    for (int i = 0; i < 128; i++)
    {
        resultList[i] = (list[counter] >> counter2) & 1;
        counter2--;
        if(i%8 == 7) {
            counter++;
            counter2 = 7;
        }
    }
}
// Zet een 128 bit array om 16 8 bit woorden.
void decimalRepresentation(uint8_t result[16], uint8_t list[128]){
    for (int i = 0; i < 16 ; i++) {
        result[i] = 0;
        int deler = 128;
        for (int j = 0; j < 8; j++) {
            if (list[i*8+j] == 1) {
                result[i] = result[i] + deler;
            }
            deler = deler/2;
        }
    }
}

void writeFile(uint32_t number) {
   printf("%d",number);
   srand(time(NULL));
   FILE *fptr;
   fptr = fopen("data.txt", "w+");
    for (uint32_t i = 0; i < number ; i++) {
        if (i%16 == 0 && i != 0) {
            fputs("\n", fptr);
        }
        fputc('A' + (rand() % 26), fptr);
    }
    fclose(fptr);
}

void giftb128(uint8_t P[16], const uint16_t K[8], uint8_t CipherText[16]) {
    int round;
    uint32_t S[4],T;
    uint16_t W[8],T6,T7;

    S[0] = ((uint32_t)P[ 0]<<24) | ((uint32_t)P[ 1]<<16) | ((uint32_t)P[ 2]<<8) | (uint32_t)P[ 3];
    S[1] = ((uint32_t)P[ 4]<<24) | ((uint32_t)P[ 5]<<16) | ((uint32_t)P[ 6]<<8) | (uint32_t)P[ 7];
    S[2] = ((uint32_t)P[ 8]<<24) | ((uint32_t)P[ 9]<<16) | ((uint32_t)P[10]<<8) | (uint32_t)P[11];
    S[3] = ((uint32_t)P[12]<<24) | ((uint32_t)P[13]<<16) | ((uint32_t)P[14]<<8) | (uint32_t)P[15];

    W[0] = ((uint16_t)K[ 0]<<8) | (uint16_t)K[ 1];
    W[1] = ((uint16_t)K[ 2]<<8) | (uint16_t)K[ 3];
    W[2] = ((uint16_t)K[ 4]<<8) | (uint16_t)K[ 5];
    W[3] = ((uint16_t)K[ 6]<<8) | (uint16_t)K[ 7];
    W[4] = ((uint16_t)K[ 8]<<8) | (uint16_t)K[ 9];
    W[5] = ((uint16_t)K[10]<<8) | (uint16_t)K[11];
    W[6] = ((uint16_t)K[12]<<8) | (uint16_t)K[13];
    W[7] = ((uint16_t)K[14]<<8) | (uint16_t)K[15];
    for(round=0; round<1; round++) {
        S[1] ^= S[0] & S[2];
        S[0] ^= S[1] & S[3];
        S[2] ^= S[0] | S[1];
        S[3] ^= S[2];
        S[1] ^= S[3];
        S[3] ^= 0xffffffff;
        S[2] ^= S[0] & S[1];

        T = S[0];
        S[0] = S[3];
        S[3] = T;
//        uint8_t D[16] = {S[0]>>24, S[0]>>16, S[0]>>8, S[0], S[1]>>24, S[1]>>16, S[1]>>8, S[1], S[2]>>24, S[2]>>16, S[2]>>8, S[2], S[3]>>24, S[3]>>16, S[3]>>8, S[3] };
//        for(int i =0; i< 8; i++) {
//            printf("%hhu\n", D[i]);
//        }
        //        printf(D[0], 'c');
//        printf(S[1], 'i');
//        printf(S[2], 'i');
//        printf(S[3], 'i');
    }
    return;
}
const unsigned char sBox[16] = {
        0x01, 0x0a, 0x04, 0x0c, 0x06, 0x0f, 0x03, 0x09, 0x02, 0x0d, 0x0b, 0x07, 0x05, 0x00, 0x08, 0x0e
};

const unsigned char revSBox[16] = {
        0x0d, 0x00, 0x08, 0x06, 0x03, 0x0c, 0x05, 0x0a, 0x0e, 0x07, 0x01, 0x0a, 0x03, 0x09, 0x0f, 0x05
};
void gift(const uint8_t P[16], const uint8_t K[16], uint8_t Cipher[16]){
    uint8_t Result[16];

    uint8_t x0[16] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
    uint8_t x1[16] = {0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1};
    uint8_t x2[16] = {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1};
    uint8_t x3[16] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
    uint8_t y0[16], y1[16], y2[16], y3[16];
    uint8_t binaryList[128], binaryPermList[128];
    uint32_t U, V;
    uint16_t W[8],T6,T7;
    W[0] = ((uint16_t)K[ 0]<<8) | (uint16_t)K[ 1];
    W[1] = ((uint16_t)K[ 2]<<8) | (uint16_t)K[ 3];
    W[2] = ((uint16_t)K[ 4]<<8) | (uint16_t)K[ 5];
    W[3] = ((uint16_t)K[ 6]<<8) | (uint16_t)K[ 7];
    W[4] = ((uint16_t)K[ 8]<<8) | (uint16_t)K[ 9];
    W[5] = ((uint16_t)K[10]<<8) | (uint16_t)K[11];
    W[6] = ((uint16_t)K[12]<<8) | (uint16_t)K[13];
    W[7] = ((uint16_t)K[14]<<8) | (uint16_t)K[15];

    for (int i = 0; i < 16; i++) {
//        printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(P[i]));
        Result[i] = P[i];
    }
//    printf("\n");

    for (int round = 0; round < 1; round ++) {
        uint8_t roundKey = GIFT_RC[round];
        uint8_t roundkeyBinary[8]={0};
        uint8_t uBinary[32];
        uint8_t vBinary[32];
        for (int i = 0; i < 16; i++) {
            // reken de Sbox uit
            Result[i] = (sBox[(Result[i] & 0xF0) >> 4] << 4) | (sBox[Result[i] & 0x0F]);
//            printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(W[i]));
        }
        binaryRepresentation(Result, binaryList);
//        for (int i = 0; i < 128; i++) {
//            printf("%hhu", binaryList[i]);
//        }
        printf("\n");
        // Permutation
        for (int i = 0; i < 128; i ++) {
            binaryPermList[i] = binaryList[permutation[i]];
        }
        // Add round key
        binaryPermList[127] = binaryPermList[127]^1;
        binaryRepresentation8bit(roundKey, roundkeyBinary);
        binaryPermList[23] = binaryPermList[23]^roundkeyBinary[2];
        binaryPermList[19] ^= roundkeyBinary[3];
        binaryPermList[15] ^= roundkeyBinary[5];
        binaryPermList[11] ^= roundkeyBinary[5];
        binaryPermList[7] ^= roundkeyBinary[6];
        binaryPermList[4] ^= roundkeyBinary[7];
        // Key schedule
        U = W[5] << 16 | W[4];
        V = W[1] << 16 | W[0];
        binaryRepresentation2(U, uBinary);
        binaryRepresentation2(V, vBinary);
        for(int i = 0; i<32; i++) {
            binaryPermList[4*i+2] = binaryPermList[4*i+2]^uBinary[i];
            binaryPermList[4*i+1] = binaryPermList[4*i+1]^vBinary[i];
        }
        for (int i = 0; i < 128; i ++) {
            //printf("%hhu", binaryPermList[i]);
        }
        /*===Key state update===*/
        T6 = (W[6]>>2) | (W[6]<<14);
        T7 = (W[7]>>12) | (W[7]<<4);
        W[7] = W[5];
        W[6] = W[4];
        W[5] = W[3];
        W[4] = W[2];
        W[3] = W[1];
        W[2] = W[0];
        W[1] = T7;
        W[0] = T6;

        decimalRepresentation(Result, binaryPermList);
        for (int i = 0; i <16; i++) {
            Cipher[i] = Result[i];
        }

        uint8_t counter = 0;
        for (int i = 0; i < 128; i ++) {
            if (i%4 == 0) {
                y0[counter] = binaryList[i];
            }
            if (i%4 == 1) {
                y1[counter] = binaryList[i];
            }
            if (i%4 == 2) {
                y2[counter] = binaryList[i];
            }
            if (i%4 == 3) {
                y3[counter] = binaryList[i];
                counter = counter + 1;
            }
        }
        // reken sbox matrix uit
        // initieer alles op 0;

        uint8_t matrix[16][16] = {0};
        for (int i = 0; i < 16; i++) {
            for (int j =0 ; j<16; j++) {
                if (i == 0 || j == 0) {
                    // deze waarden zijn altijd 8, ik noem alleen de noemer, de deler is altijd 16.
                    matrix[i][j] = 8;
                    continue;
                }
                for(int k = 0 ; k < 16; k++) {
                    uint8_t matrixResult = 0;
                    if (i%2 == 1) {
                        matrixResult = matrixResult ^ x3[k];
                    } if (j%2 == 1) {
                        matrixResult = matrixResult ^ y3[k];
                    } if (i>7) {
                        matrixResult = matrixResult ^ x0[k];
                    } if (j>7) {
                        matrixResult = matrixResult ^ y0[k];
                    } if ((i>3 && i < 8) || (i>11 && i<16)) {
                        matrixResult = matrixResult ^ x1[k];
                    } if ((j>3 && j < 8) || (j>11 && j<16)) {
                        matrixResult = matrixResult ^ y1[k];
                    } if ((i>1 && i <4) || (i>5 && i <8) || (i>9&&i<12) || (i>13&&i<16)) {
                        matrixResult = matrixResult ^ x2[k];
                    }
                    if ((j>1 && j<4) || (j>5 && j<8) || (j>9&&j<12) || (j>13&&j<16)) {
                        matrixResult = matrixResult ^ y2[k];
                    }
                    matrix [i][j] = matrix[i][j] + matrixResult;
                }
            }
        }
        for (int i = 0; i < 16; i ++) {
            for (int j = 0; j < 16; j++) {
                printf("%hhu", matrix[i][j]);
                printf(" ");
            }
            printf("\n");
       }
        uint8_t matrix2[16][4] = {0};
        for (int i = 0; i < 16; i++) {
            if (x3[i] == y2[i]) {
                matrix2[1][3] +=1;
            }
            if (x2[i] == y2[i]) {
                matrix2[2][3] +=1;
            }
            if ((x3[i] ^ x2[i])  == y2[i]) {
                matrix2[3][3] +=1;
            }
            if (x1[i] == y2[i]) {
                matrix2[4][3] +=1;
            }
            if ((x3[i]  ^ x1[i]) == y2[i]) {
                matrix2[5][3] +=1;
            }
            if ((x1[i] ^ x2[i])  == y2[i]) {
                matrix2[6][3] +=1;
            }
            if ((x1[i] ^ x2[i] ^ x3[i]) == y2[i]) {
                matrix2[7][3] +=1;
            }
            if (x0[i] == y2[i] ) {
                matrix2[8][3] +=1;
            }
            if ((x0[i] ^ x3[i]) == y2[i]) {
                matrix2[9][3] +=1;
            }
            if ((x0[i] ^ x2[i]) == y2[i]) {
                matrix2[10][3] +=1;
            }
            if ((x0[i] ^ x2[i] ^x3[i]) ==  y2[i]) {
                matrix2[11][3] +=1;
            }
            if ((x0[i] ^ x1[i]) == y2[i]) {
                matrix2[12][3] +=1;
            }
            if ((x0[i] ^x1[i] ^ x3[i]) ==  y2[i] ) {
                matrix2[13][3] +=1;
            }
            if ((x0[i] ^ x1[i] ^ x2[i]) ==  y2[i]) {
                matrix2[14][3] +=1;
            }
            if ((x0[i] ^ x1[i] ^ x3[i] ^ x2[i]) == y2[i]) {
                matrix2[15][3] +=1;
            }
        }
        for (int i = 0; i < 16; i ++) {
            for (int j = 0; j < 4; j++) {
            //    printf("%hhu", matrix2[i][j]);
            //    printf(" ");
            }
          //  printf("\n");
        }

//        printf("\n");
//        for(int i = 0; i<128; i++) {
//            binaryPermList[i] =binaryList[permutation[i]];
//            printf("%hhu", binaryPermList[i]);
//        }
    }

    printf ("\n");

}
void roundFunc(const uint8_t P[16], const uint8_t K[16], uint8_t Cipher[16]){
    uint8_t Result[16];
    uint8_t binaryList[128], binaryPermList[128];
    uint32_t U, V;
    uint16_t W[8],T6,T7;
    for (int i = 0; i < 16; i++) {
        Result[i] = P[i];
    }
            //fixme : zorg dat round mee werkt.
        uint8_t roundKey = GIFT_RC[0];
        uint8_t roundkeyBinary[8]={0};
        uint8_t uBinary[32];
        uint8_t vBinary[32];
        for (int i = 0; i < 16; i++) {
            // reken de Sbox uit
            Result[i] = (sBox[(Result[i] & 0xF0) >> 4] << 4) | (sBox[Result[i] & 0x0F]);
        }
        binaryRepresentation(Result, binaryList);
        printf("\n");
        // Permutation
        for (int i = 0; i < 128; i ++) {
            binaryPermList[i] = binaryList[permutation[i]];
        }
        // Add round key
        binaryPermList[127] = binaryPermList[127]^1;
        binaryRepresentation8bit(roundKey, roundkeyBinary);
        binaryPermList[23] = binaryPermList[23]^roundkeyBinary[2];
        binaryPermList[19] ^= roundkeyBinary[3];
        binaryPermList[15] ^= roundkeyBinary[5];
        binaryPermList[11] ^= roundkeyBinary[5];
        binaryPermList[7] ^= roundkeyBinary[6];
        binaryPermList[4] ^= roundkeyBinary[7];
        decimalRepresentation(Result, binaryPermList);
        for (int i = 0; i <16; i++) {
            Cipher[i] = Result[i];
        }
}


void executeGiftToFile() {
    // initialisation
    uint8_t F[16];
    uint16_t  K[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t C[16];
    FILE *fptr;
    FILE *cipherfile;
    fptr = fopen("data.txt", "r+");
    cipherfile = fopen("cipherdata.txt", "w+");
 // lees eerste character uit.
    char currentchar = fgetc(fptr);
    // start de loop :O
    while(currentchar != EOF) {
        int counter = 0;
        if (currentchar == '\n') {
            counter = 0;
            gift(F, K, C);
            for(int i =0; i < 16; i++) {
                fputc(C[i], cipherfile);
            }
            fputc('\n', cipherfile);
       } else {
            F[counter] = currentchar;
            counter ++;
       }
       currentchar = fgetc(fptr);

    }
}
// magische lijn van code
// s-box uitrekenmeuk
int approxTable[16][16];
// initalisatie.
    int16_t knownP[2000][8];
    int16_t knownC[2000][8];


int applyMask(int value, int mask)
{
    int interValue = value & mask;
    int total = 0;

    while(interValue > 0)
    {
        int temp = interValue % 2;
        interValue /= 2;

        if (temp == 1)
            total = total ^ 1;
    }
    return total;
}

int applyMask2(uint8_t[16] value, uint8_t mask) {
    int total = 0;
    for (int i = 0; < 16; i++){
        int interValue = value[i] & mask;
        int temp = interValue % 2;
        interValue /= 2;
        if (temp == 1) {
            total = total ^1;
        }
    }
    return total;

}

void findApprox()
{
    int c, d, e;

    for(c = 1; c < 16; c++)                                         //output mask
        for(d = 1; d < 16; d++)                                     //input mask
            for(e = 0; e < 16; e++)                                 //input
                if (applyMask(e, d) == applyMask(sBox[e], c))
                    approxTable[d][c]++;
}

void showApprox()
{
    printf("Strong Linear Approximations: \n");
    int c, d, e;
    for (c = 1; c < 16; c++) {

       printf("\n");
       for(d = 1; d < 16; d++)
            printf("%i ", approxTable[c][d]);
            if (approxTable[c][d] == 14 || approxTable[c][d] == 2);
//                printf("  %i : %i -> %i\n", approxTable[c][d], c, d);

    }
    printf("\n");
}
keyFunc(uint8_t K[16]) {
    uint32_t U, V;
    uint16_t W[8],T6,T7;
    uint8_t binaryList[128], binaryPermList[128];
    uint8_t uBinary[32];
    uint8_t vBinary[32];
    W[0] = ((uint16_t)K[ 0]<<8) | (uint16_t)K[ 1];
    W[1] = ((uint16_t)K[ 2]<<8) | (uint16_t)K[ 3];
    W[2] = ((uint16_t)K[ 4]<<8) | (uint16_t)K[ 5];
    W[3] = ((uint16_t)K[ 6]<<8) | (uint16_t)K[ 7];
    W[4] = ((uint16_t)K[ 8]<<8) | (uint16_t)K[ 9];
    W[5] = ((uint16_t)K[10]<<8) | (uint16_t)K[11];
    W[6] = ((uint16_t)K[12]<<8) | (uint16_t)K[13];
    W[7] = ((uint16_t)K[14]<<8) | (uint16_t)K[15];

    U = W[5] << 16 | W[4];
    V = W[1] << 16 | W[0];
        binaryRepresentation2(U, uBinary);
        binaryRepresentation2(V, vBinary);
        for(int i = 0; i<32; i++) {
            binaryPermList[4*i+2] = binaryPermList[4*i+2]^uBinary[i];
            binaryPermList[4*i+1] = binaryPermList[4*i+1]^vBinary[i];
        }

        T6 = (W[6]>>2) | (W[6]<<14);
        T7 = (W[7]>>12) | (W[7]<<4);
        W[7] = W[5];
        W[6] = W[4];
        W[5] = W[3];
        W[4] = W[2];
        W[3] = W[1];
        W[2] = W[0];
        W[1] = T7;
        W[0] = T6;

        K[0] = W[0];
        K[1] = W[0] >> 8;
        K[2] = W[1];
        K[3] = W[1] >> 8;
        K[4] = W[2];
        K[5] = W[2] >> 8;
        K[6] = W[3];
        K[7] = W[3] >> 8;
        K[8] = W[4];
        K[9] = W[4] >> 8;
        K[10] = W[5];
        K[11] = W[5] >> 8;
        K[12] = W[6];
        K[13] = W[6] >> 8;
        K[14] = W[7];
        K[15] = W[7] >> 8;
}


void fillKnowns(numKnown) {
    uint8_t key[16];
    for (int i = 0; i < 8; i ++) {
        key[i] = rand() % 255;
    }

    int total = 0;
    for (int c = 0; c < numKnown; c++) {
            // initialistatie.
            for (int i = 0; i< 8; i++) {
                knownP[c][i] = rand() % 65535;
            }
            roundFunc(knownP[c], keyFunc(key), knownC[c]);
            roundFunc(knownP[c], keyFunc(key), knownC[c]);
    }
}

void attackGift() {
    srand(time(NULL));

    findApprox();
    showApprox();
    int inputApprox = 11;
    int outputApprox = 11;
    int numKnown = 2000; // amount of bits
    fillKnowns(numKnown);
    int keyScore[255];
    int sofar1 = 0;
    printf("Linear Attack:  Using Linear Approximation = %i -> %i\n", inputApprox, outputApprox);
        int c, d;
    for(c = 0; c < 255; c++)
    {
        keyScore[c] = 0;
        for(d = 0; d < numKnown; d++)
        {
            sofar1++;
            int midRound = roundFunc(knownP[d], c);         //Find Xi by guessing at K1

            if ((applyMask(midRound, inputApprox) == applyMask(knownC[d], outputApprox)))
                keyScore[c]++;
            else
                keyScore[c]--;

        }

    }
}
int main() {
    uint32_t number = 1000;
    uint16_t  K[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t F[16] = {0x01, 0x23, 0x45, 0x067, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x067, 0x89, 0xab, 0xcd, 0xef};
    uint8_t C[16];
   // writeFile(number);
   //  executeGiftToFile();
    //  giftb128(F, K, C);
    attackGift();
 //   gift(F, K, C);

    return 0;
}

