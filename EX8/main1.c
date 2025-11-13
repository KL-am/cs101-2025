#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义常量
#define LOTTO_NUMBERS_PER_LINE 7
#define WINNING_NUMBERS_COUNT 3
#define MAX_LINE_LENGTH 256

// 结构体用于存储彩票数据
typedef struct {
    int id; // 编号，如 [1]
    int numbers[LOTTO_NUMBERS_PER_LINE];
    int valid; // 1: 有效号码行, 0: 无效号码行 (如 --)
} LottoEntry;

// 函数：检查一组彩票号码是否包含所有中奖号码
// lotto_nums: 7个彩票号码
// win_nums: 3个中奖号码
int check_winning(int lotto_nums[], int win_nums[]) {
    int matches = 0;

    // 遍历3个中奖号码
    for (int i = 0; i < WINNING_NUMBERS_COUNT; i++) {
        int found = 0;
        // 检查这个中奖号码是否在7个彩票号码中
        for (int j = 0; j < LOTTO_NUMBERS_PER_LINE; j++) {
            if (win_nums[i] == lotto_nums[j]) {
                found = 1;
                break;
            }
        }
        if (found) {
            matches++;
        }
    }

    // 只有当3个中奖号码全部匹配时才中奖
    return (matches == WINNING_NUMBERS_COUNT);
}

int main() {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char date_string[MAX_LINE_LENGTH] = ""; // 存储售出时间
    LottoEntry tickets[5]; // 假设最多5注彩票 (根据截图)
    int ticket_count = 0;
    int winning_numbers[WINNING_NUMBERS_COUNT]; // 用户输入的3个中奖号码

    // --- 1. 读取 lotto.txt 文件并解析 ---
    fp = fopen("lotto.txt", "r");
    if (fp == NULL) {
        printf("无法打开 lotto.txt 文件。\n");
        return 1;
    }

    // 逐行读取文件
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        // 去掉行末的换行符
        line[strcspn(line, "\n")] = 0;

        // 查找售出日期行 (简单假设第二行是日期，或通过匹配非彩票/分隔符行)
        // 这里的解析逻辑可能需要根据实际lotto.txt格式调整
        if (strstr(line, "======== ") == NULL && ticket_count == 0 && date_string[0] == '\0') {
            // 假设第二行是日期，跳过第一行标题
            // 简单实现：将非标题/非分隔符/非彩票行视为日期（此方法不严谨，但基于截图推测）
            if (strstr(line, "lotto649") == NULL) {
                 strcpy(date_string, line);
            }
        }
        
        // 尝试解析彩票号码行: 格式如 "[1]: 16 63 44 57 38 49 05"
        if (line[0] == '[' && ticket_count < 5) {
            int n_read;
            // 尝试读取 ID 和 7 个号码
            n_read = sscanf(line, "[%d]: %d %d %d %d %d %d %d",
                            &tickets[ticket_count].id,
                            &tickets[ticket_count].numbers[0],
                            &tickets[ticket_count].numbers[1],
                            &tickets[ticket_count].numbers[2],
                            &tickets[ticket_count].numbers[3],
                            &tickets[ticket_count].numbers[4],
                            &tickets[ticket_count].numbers[5],
                            &tickets[ticket_count].numbers[6]);

            if (n_read == LOTTO_NUMBERS_PER_LINE + 1) {
                // 成功读取 ID 和 7 个号码
                tickets[ticket_count].valid = 1;
                ticket_count++;
            } else {
                // 读取到无效的号码行 (如 -- -- --...)
                // 为了简化，我们只记录有效的号码行。如果需要保留 '--'，则需调整结构体和解析逻辑。
                // 基于截图，中奖彩票只会显示有效的行，所以此处可以跳过无效行或标记为无效。
                
                // 标记为无效但仍增加计数，以便与lotto.txt的行号对应
                int check_dash = sscanf(line, "[%d]: -- -- -- -- -- -- --", &tickets[ticket_count].id);
                if (check_dash == 1) {
                    tickets[ticket_count].valid = 0;
                    ticket_count++;
                }
            }
        }
    }
    fclose(fp);

    // --- 2. 用户输入中奖号码 ---
    printf("请输入中奖号码三个 : ");
    if (scanf("%d %d %d", &winning_numbers[0], &winning_numbers[1], &winning_numbers[2]) != WINNING_NUMBERS_COUNT) {
        printf("输入号码格式错误。\n");
        return 1;
    }

    printf("\n以下为中奖彩卷：\n");

    // --- 3. 兑奖和输出结果 ---
    for (int i = 0; i < ticket_count; i++) {
        // 检查是否是有效号码行 且 中奖
        if (tickets[i].valid && check_winning(tickets[i].numbers, winning_numbers)) {
            // 输出售出时间 (使用之前读取到的日期)
            printf("售出时间 : %s: [%d]", date_string, tickets[i].id);
            
            // 输出中奖号码
            for (int j = 0; j < LOTTO_NUMBERS_PER_LINE; j++) {
                printf(" %02d", tickets[i].numbers[j]);
            }
            printf("\n");
        }
    }

    return 0;
}
