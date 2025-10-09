#include <stdio.h>

int main() {
    FILE *fp;

    int a[] = {0, 1, 2};
    char b[] = {'A', 'B', 'C'};
    float c[] = {1.1, 1.2, 1.3};

    int ra[3];
    char rb[3];
    float rc[3];

    // 以二進位寫入
    fp = fopen("a.bin", "wb+");   // 開檔(可讀寫)
    fwrite(a, sizeof(a), 1, fp);  // 寫入整數陣列
    fwrite(b, sizeof(b), 1, fp);  // 寫入字元陣列
    fwrite(c, sizeof(c), 1, fp);  // 寫入浮點陣列

    // 將讀寫指標移回檔案開頭
    fseek(fp, 0, SEEK_SET);

    // 依序讀回三個陣列
    fread(ra, sizeof(ra), 1, fp);
    fread(rb, sizeof(rb), 1, fp);
    fread(rc, sizeof(rc), 1, fp);

    fclose(fp);

    // 輸出結果
    for (int i = 0; i < 3; i++)
        printf("%d ", ra[i]);
    printf("\n");

    for (int i = 0; i < 3; i++)
        printf("%c ", rb[i]);
    printf("\n");

    for (int i = 0; i < 3; i++)
        printf("%f ", rc[i]);
    printf("\n");

    return 0;
}
