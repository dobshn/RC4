#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
 * Simple RC4 encrypt/decrypt utility.
 *
 * Usage:
 *   rc4 <input_file> <key> > output_file
 *
 * - input_file: 읽을 바이너리 파일 경로
 * - key: RC4 키(그냥 문자열 그대로 사용)
 * - 결과는 stdout(바이너리)로 출력되므로 리다이렉트해서 저장하면 됨.
 */

int main(int argc, char* argv[]) {
    uint8_t tmp;
    int i, j;

    /**
     * 1. 파일 내용 읽어서 메모리에 올리기
     */
    // 파일 열기
    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("파일 열기 실패");
        return 1;
    }
    // 파일 크기 구하기
    fseek(fp, 0, SEEK_END);
    size_t file_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    // 파일 내용 읽기
    uint8_t *file = (uint8_t *)malloc(file_len);
    fread(file, 1, file_len, fp);
    // 파일 닫기
    fclose(fp);

    /**
     * 2. S[], T[] 배열 초기화
     */
    uint8_t S[256], T[256];
    char *key = argv[2];
    size_t key_len = strlen(key);
    for (i = 0; i < 256; i++) {
        S[i] = i;
        T[i] = key[i % key_len];
    }

    /**
     * 3. S[] 배열 섞기
     */
    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;
        tmp = S[i]; S[i] = S[j]; S[j] = tmp;
    }

    /**
     * 4. 키 스트림 생성 및 메모리에 있는 파일에 적용
     */
    i = j = 0;
    for (int k = 0; k < file_len; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        tmp = S[i]; S[i] = S[j]; S[j] = tmp;
        file[k] = file[k] ^ S[(S[i] + S[j]) % 256];
    }

    /**
     * 5. 암/복호화 적용된 메모리에 있는 파일 표준 출력
     */
    fwrite(file, 1, file_len, stdout);
    
    free(file);
    return 0;
}