void giftNoRoundKeyAddition(const uint8_t P[16], const uint8_t K[16], uint8_t Cipher[16]){
    uint8_t Result[16];
    uint8_t binaryList[128], binaryPermList[128];
    uint32_t U, V;
    uint16_t W[8],T6,T7;
    for (int i = 0; i < 16; i++) {
        Result[i] = P[i];
    }

        uint8_t roundKey = GIFT_RC[round];
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

