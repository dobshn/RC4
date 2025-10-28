#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// 파일명, 키를 입력받아 암호화한 파일 생성
int main(int argc, char* argv[]) {
    uint8_t S[256], T[256], *text, *result;
    char *key;
    int key_len, i, j;
    size_t text_len;
    uint8_t tmp;

    // 파일 열기
    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("파일 열기 실패");
        return 1;
    }

    // 파일 크기 구하기
    fseek(fp, 0, SEEK_END);
    text_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 파일 내용 읽기
    text = (uint8_t *)malloc(text_len);
    fread(text, 1, text_len, fp);
    fclose(fp);

    // 1. 초기화
    key = argv[2];
    key_len = strlen(key);
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
    result = (uint8_t *)malloc(text_len + 1);
    i = j = 0;
    for (int k = 0; k < text_len; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        tmp = S[i]; S[i] = S[j]; S[j] = tmp;
        tmp = (S[i] + S[j]) % 256;
        result[k] = text[k] ^ S[tmp];
    }

    // 4. 결과 출력
    fwrite(result, 1, text_len, stdout);
    
    free(text);
    free(result);
    return 0;
}