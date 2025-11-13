#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define COUNTER_FILE "counter.bin"
#define MAX_LOTTO_NUM 7
#define NUM_SET 5  

void init_file();
int get_counter();
void set_counter(int counter);
void do_lotto_main(int counter);
void print_lottofile(int num_set, int counter, char lotto_file[]);
void print_lotto_row(FILE* tmpfp, int n);
int num_in_numset(int num, int numset[], int Len);

int main() {
    int counter;
   
    init_file();
    counter = get_counter();

    counter++;
   
    printf("counter = %d\n", counter); 
    do_lotto_main(counter);
    set_counter(counter);
   
    return 0;
}

void init_file() {
    int write_array[1] = {0}; 
    FILE* fp = fopen(COUNTER_FILE, "r");
    if (fp == NULL) { 
        FILE* tmpfp = fopen(COUNTER_FILE, "wb+"); 
        fwrite(write_array, sizeof(int), 1, tmpfp);
        fclose(tmpfp); 
    } else {
        fclose(fp); 
    }
}

int get_counter() {
    int read_array[1];
    FILE* tmpfp = fopen(COUNTER_FILE, "rb"); 
    fread(read_array, sizeof(int), 1, tmpfp); 
    fclose(tmpfp); 
    return read_array[0]; 
}

void set_counter(int counter) {
    int write_array[1];
    write_array[0] = counter;
    FILE* tmpfp = fopen(COUNTER_FILE, "wb"); 
    fwrite(write_array, sizeof(int), 1, tmpfp); 
    fclose(tmpfp); 
}

void do_lotto_main(int counter) {
    char lotto_file[32]; 
    int num_set = 0; 
   
    snprintf(lotto_file, 32, "lotto[%05d].txt", counter);
    printf("歡迎光臨長庚樂透彩選號系統\n");
    printf("請問您要購買幾組樂透彩 (1-5): ");
    scanf("%d", &num_set); // 簡報 92 行

    // 檢查使用者輸入，防止超過 5 組
    if (num_set > NUM_SET) {
        printf("輸入錯誤，最多只能購買 %d 組。\n", NUM_SET);
        num_set = NUM_SET;
    } else if (num_set < 1) {
        printf("輸入錯誤，至少要購買 1 組。\n");
        num_set = 1;
    }

    print_lottofile(num_set, counter, lotto_file); 
   
    printf("已為您購買 %d 組樂透組合，檔案已輸出至 %s\n", num_set, lotto_file);
}

void print_lottofile(int num_set, int counter, char lotto_file[]) {
    time_t curtime;
    time(&curtime); 
   
    srand(time(0));
   
    FILE* tmpfp = fopen(lotto_file, "w+"); 
   
    fprintf(tmpfp, "======== lotto649 =========\n"); 
    fprintf(tmpfp, "=======+ No.%05d +========\n", counter); 
   
    fprintf(tmpfp, "= %.24s =\n", ctime(&curtime));
   
    for (int i = 0; i < NUM_SET; i++) {
        if (i < num_set) { 
            fprintf(tmpfp, "[%d]: ", i + 1);
            print_lotto_row(tmpfp, i + 1);
        } else {
            fprintf(tmpfp, "[%d]: -- -- -- -- -- -- --\n", i+1);
        }
    }
   
    fprintf(tmpfp, "======== csie@CGU =========\n"); 
    fclose(tmpfp); 
}


void print_lotto_row(FILE* tmpfp, int n) {
    int numset[MAX_LOTTO_NUM]; 

    for (int i = 0; i < MAX_LOTTO_NUM - 1; ) { 
        int num = (rand() % 69) + 1; 
        if (num_in_numset(num, numset, i)) {
            continue; 
        } else {
            numset[i] = num; 
            i++; 
        }
    }
    for (int i = 0; i < 1; ) { 
        int num = (rand() % 10) + 1; 
        if (num_in_numset(num, numset, MAX_LOTTO_NUM - 1)) { 
            continue; 
        } else {
            numset[MAX_LOTTO_NUM - 1] = num; 
            i++; 
        }
    }
   
    for (int i = 0; i < MAX_LOTTO_NUM - 1; i++) { 
        for (int j = 0; j < i; j++) { 
            if (numset[j] > numset[i]) { 
                int temp = numset[j]; 
                numset[j] = numset[i]; 
                numset[i] = temp;
            }
        }
    }
   
    for (int i = 0; i < MAX_LOTTO_NUM; i++) { 
        fprintf(tmpfp, "%02d ", numset[i]); 
    }
    fprintf(tmpfp, "\n"); 
}


int num_in_numset(int num, int numset[], int Len) {
    int ret = 0; 
    for (int i = 0; i < Len; i++) { 
        if (num == numset[i]) { 
            ret = 1; 
            break; 
        }
    }
    return ret; 
}

