#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// 평문/암호문, 키를 입력받아 암호화/복호화 한 결과 출력
int main(int argc, char* argv[]) {
    uint8_t S[256], T[256];
    char *text, *key, *result;
    int key_len, text_len, i, j;
    uint8_t tmp;

    // 1. 초기화
    text = argv[1];
    key = argv[2];
    key_len = strlen(key);
    text_len = strlen(text);
    for (i = 0; i < 256; i++) {
        S[i] = i;
        T[i] = key[i % key_len];
    }

    // 2. 키 스케줄링
    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;
        tmp = S[i]; S[i] = S[j]; S[j] = tmp;
    }

    // 3. 암/복호화
    result = (char *)malloc(text_len + 1);
    strcpy(result, text);
    i = j = 0;
    for (int k = 0; k < text_len; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        tmp = S[i]; S[i] = S[j]; S[j] = tmp;
        tmp = (S[i] + S[j]) % 256;
        result[k] = result[k] ^ S[tmp];
    }

    // 4. 결과 출력
    for (i = 0; i < text_len + 1; i++)
        printf("%c", result[i]);
    printf("\n");

    free(result);
    return 0;
}