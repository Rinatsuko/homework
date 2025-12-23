#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>

// 歌曲节点结构体
typedef struct Song // 歌曲节点
{
    int id;
    char title[100];
    char artist[50];
    char filepath[300];
    struct Song *next;
} Song;

// 播放列表管理器 包括播放列表的头音乐 尾音乐 头部音乐 以及歌曲总数四个信息
typedef struct PlaylistManager
{
    Song *head;
    Song *tail;
    Song *current;
    int song_count;
} PlaylistManager;

// 函数声明
void init_playlist_manager(PlaylistManager *manager);                          // 初始化链表
int load_songs_from_file(PlaylistManager *manager, const char *filename);      // 从文件中读取到链表
void add_song(PlaylistManager *manager, const char *title, const char *artist, // 人工增加音乐
              const char *filepath);
void display_playlist(PlaylistManager *manager);                                                                         // 显示播放列表
int delete_songs_by_title(PlaylistManager *manager, const char *title);                                                  // 删除指定名字的音乐
int play_song_by_title(PlaylistManager *manager, const char *title);                                                     // 根据名字播放音乐
int export_playlist(PlaylistManager *manager, const char *filename);                                                     // 导出播放列表
int play_song_random(PlaylistManager *manager);                                                                          // 随机播放音乐
int insert_song_at(PlaylistManager *manager, int position, const char *title, const char *artist, const char *filepath); // 向指定位置添加音乐
void destroy_playlist(PlaylistManager *manager);                                                                         // 清空列表

void play_audio(const char *filename)
{
    char command[256];
    FILE *mp3File = fopen(filename, "rb");
    if (!mp3File)
    {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    fclose(mp3File);

    // 停止当前可能正在播放的音乐
    mciSendString("close current_song", NULL, 0, NULL);

    // 使用 MCI 命令打开音乐文件，别名为 current_song
    // type mpegvideo 支持 mp3 等多种格式
    snprintf(command, sizeof(command), "open \"%s\" type mpegvideo alias current_song", filename);

    int ret = mciSendString(command, NULL, 0, NULL);
    if (ret != 0)
    {
        printf("播放失败或中断，请检查文件格式是否支持。\n");
    }

    // 播放音乐
    mciSendString("play current_song", NULL, 0, NULL);
    // printf("正在后台播放...\n");
}

int load_songs_from_file(PlaylistManager *manager, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return -1;

    char title[100], artist[50], filepath[300];
    // 使用 %[^,] 读取逗号前的内容，用 ,%[^,] 跳过逗号继续读取
    // 最后一项读取到换行符为止 %s 或 %[^\n]
    while (fscanf(file, " %[^,],%[^,],%s", title, artist, filepath) == 3)
    {
        add_song(manager, title, artist, filepath);
    }
    fclose(file);
    return 0;
}
// 初始化播放管理器
void init_playlist_manager(PlaylistManager *manager)
{
    manager->head = NULL;
    manager->tail = NULL;
    manager->current = NULL;
    manager->song_count = 0;
}

// 1. 在链表末尾添加歌曲
void add_song(PlaylistManager *manager, const char *title, const char *artist, const char *filepath)
{
    Song *newsong = (Song *)malloc(sizeof(Song));
    if (newsong == NULL)
        return; // 检查内存分配

    // ID 生成模块：使用当前歌曲总数 + 1 作为新 ID
    newsong->id = manager->song_count + 1;
    // 字符串数组赋值要用strcpy
    strcpy(newsong->title, title);
    strcpy(newsong->artist, artist);
    strcpy(newsong->filepath, filepath);

    newsong->next = NULL; // 新节点是最后一个，next 必须为空
    if (manager->head == NULL)
    {
        manager->head = newsong;
        manager->tail = newsong;
    }
    else
    {
        manager->tail->next = newsong; // 旧尾巴指向新节点
        manager->tail = newsong;       // 更新尾指针指向新节点
    }

    manager->song_count++; // 增加歌曲总数
}

// 2. 显示播放列表
void display_playlist(PlaylistManager *manager)
{
    Song *temp = manager->head;
    if (temp == NULL)
    {
        printf("播放列表为空\n");
        return;
    }

    // 遍历整个链表，直到 temp 为 NULL
    while (temp != NULL)
    {
        printf("ID: %d, 标题: %s, 作者: %s\n", temp->id, temp->title, temp->artist);
        temp = temp->next;
    }
}

// 3. 删除歌曲
int delete_songs_by_title(PlaylistManager *manager, const char *title)
{
    Song *temp = manager->head;
    if (temp == NULL)
    {
        printf("播放列表为空，无法删除歌曲\n");
        return -1;
    }
    // 删除队头歌曲
    if (strcmp(title, temp->title) == 0)
    {
        if (manager->head == manager->tail)
        {
            manager->head = NULL;
            manager->tail = NULL;
            manager->song_count--;
            free(temp);
            return 0;
        }
        Song *temp1 = temp->next;
        free(temp);
        manager->head = temp1;
        manager->song_count--;
        return 0;
    }
    // 删除队中或队尾歌曲
    Song *prev = NULL; // 初始化 prev
    while (temp != NULL && strcmp(title, temp->title) != 0)
    { // temp != NULL 检查，防止越界，确保temp指向的是有效节点
        prev = temp;
        temp = temp->next;
    }

    // 如果遍历完也没找到
    if (temp == NULL)
    {
        printf("未找到歌曲: %s\n", title);
        return -1;
    }

    // 处理队尾情况
    if (temp == manager->tail)
    {
        manager->tail = prev;
        prev->next = NULL;
        free(temp);
        manager->song_count--;
        return 0;
    }
    prev->next = temp->next;
    free(temp);
    manager->song_count--;
    return 0;
}

// 4. 播放歌曲
int play_song_by_title(PlaylistManager *manager, const char *title)
{
    Song *temp = manager->head;
    while (temp != NULL)
    {
        if (strcmp(temp->title, title) == 0)
        {
            printf("正在播放: %s by %s\n", temp->title, temp->artist);
            play_audio(temp->filepath);
            return 0;
        }
        temp = temp->next;
    }
    printf("未找到歌曲: %s\n", title);
    return -1;
}

// 5. 将播放列表保存到文件
int export_playlist(PlaylistManager *manager, const char *filename)
{
    Song *temp = manager->head;
    if (temp == NULL)
    {
        printf("播放列表为空，无法导出\n");
        return -1;
    }
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行写入\n", filename);
        return -1;
    }
    for (int i = 0; i < manager->song_count && temp != NULL; i++)
    {
        fprintf(file, "%d. %s by %s\n", i + 1, temp->title, temp->artist);
        temp = temp->next;
    }
    fclose(file);
    printf("播放列表已导出到 %s\n", filename);
    return 0;
}

// 6. 随机播放歌曲（非必做）
int play_song_random(PlaylistManager *manager)
{
    if (manager->song_count == 0)
        return -1;

    srand(time(NULL)); // 初始化随机种子
    int index = rand() % manager->song_count;

    Song *curr = manager->head;
    for (int i = 0; i < index; i++)
        curr = curr->next;

    printf("随机推荐: %s\n", curr->title);
    play_audio(curr->filepath);
    return 0;
}

// 7. 在指定位置插入歌曲（非必做）
int insert_song_at(PlaylistManager *manager, int position, const char *title,
                   const char *artist, const char *filepath)
{
    if (position < 1 || position > manager->song_count + 1)
    {
        printf("插入位置无效: %d\n", position);
        return -1;
    }

    Song *newsong = (Song *)malloc(sizeof(Song));
    if (newsong == NULL)
    {
        printf("内存分配失败\n");
        return -1;
    }

    // ID 生成
    newsong->id = manager->song_count + 1;
    strcpy(newsong->title, title);
    strcpy(newsong->artist, artist);
    strcpy(newsong->filepath, filepath);

    // 插入到头部
    if (position == 1)
    {
        newsong->next = manager->head;
        manager->head = newsong;
        if (manager->tail == NULL) // 如果原本链表为空
        {
            manager->tail = newsong;
        }
    }
    else
    {
        // 找到插入位置的前一个节点
        Song *prev = manager->head;
        for (int i = 1; i < position - 1; i++)
        {
            prev = prev->next;
        }

        newsong->next = prev->next;
        prev->next = newsong;

        // 如果插入的是最后一个位置，更新 tail
        if (newsong->next == NULL)
        {
            manager->tail = newsong;
        }
    }

    manager->song_count++;
    printf("成功在位置 %d 插入歌曲: %s\n", position, title);
    return 0;
}

// 8. 销毁整个链表（非必做）
void destroy_playlist(PlaylistManager *manager)
{
    Song *current = manager->head;
    while (current != NULL)
    {
        Song *next = current->next;
        free(current);
        current = next;
    }
    init_playlist_manager(manager);
    printf("播放列表已清空\n");
}

void display_menu()
{
    printf("\n");
    printf("链表音乐播放器管理器\n");
    printf("==========================================\n");
    printf("1. 人工添加歌曲\n");
    printf("2. 显示播放列表\n");
    printf("3. 删除歌曲 (按标题)\n");
    printf("4. 播放歌曲 (按标题)\n");
    printf("5. 导出歌单\n");
    printf("6. 随机播放歌曲(非必做)\n");
    printf("7. 在指定位置添加歌曲(非必做)\n");
    printf("8. 清空播放列表(非必做)\n");
    printf("0. 退出程序\n");
    printf("==========================================\n");
    printf("请选择操作 (0-8): ");
}

// 清除输入缓冲区
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// 获取用户输入的字符串
void get_user_input(char *buffer, int size, const char *prompt)
{
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    // 去除换行符
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
}

// 主函数 - 交互式程序
int main()
{
    PlaylistManager manager;
    init_playlist_manager(&manager);
    load_songs_from_file(&manager, "song_list.txt");

    printf("=== 链表音乐播放器管理器 ===\n");
    printf("已加载 %d 首示例歌曲\n", manager.song_count);

    int choice;
    char input[100];

    do
    {
        display_menu();

        if (scanf("%d", &choice) != 1)
        {
            printf("无效输入，请输入数字\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice)
        {
        case 1:
        { // 添加歌曲
            char title[100], artist[50], filepath[300];

            get_user_input(title, sizeof(title), "请输入歌曲标题: ");
            get_user_input(artist, sizeof(artist), "请输入作者: ");
            get_user_input(filepath, sizeof(filepath), "请输入歌曲路径: ");

            add_song(&manager, title, artist, filepath);
            break;
        }
        case 2:
        { // 显示播放列表 (正向)
            display_playlist(&manager);
            break;
        }
        case 3:
        { // 删除歌曲 (按标题)
            char title[100];
            get_user_input(title, sizeof(title), "请输入要删除的歌曲标题: ");
            delete_songs_by_title(&manager, title);
            break;
        }
        case 4:
        { // 按歌曲名播放歌曲
            char title[100];
            get_user_input(title, sizeof(title), "请输入要播放的歌曲标题: ");
            int res = play_song_by_title(&manager, title);
            break;
        }
        case 5:
        {
            char path2export[300];
            get_user_input(path2export, sizeof(path2export), "请输入要导出的目标文件名: ");
            export_playlist(&manager, path2export);
            break;
        }
        case 6:
        {
            play_song_random(&manager);
            break;
        }
        case 7:
        {
            char title[100], artist[50], filepath[300];
            int position;
            get_user_input(title, sizeof(title), "请输入歌曲标题: ");
            get_user_input(artist, sizeof(artist), "请输入作者: ");
            get_user_input(filepath, sizeof(filepath), "请输入歌曲路径: ");
            printf("请输入歌曲插入位置: ");
            scanf("%d", &position);
            insert_song_at(&manager, position, title, artist, filepath);
            break;
        }
        case 8:
        {
            destroy_playlist(&manager);
            break;
        }
        case 0: // 退出程序
            // 在退出前自动导出，确保生成实验要求的文件
            export_playlist(&manager, "song_list_result.txt");
            break;
        }

        // 暂停，让用户看到结果
        if (choice != 0)
        {
            printf("\n按回车键继续...");
            getchar();
        }

    } while (choice != 0);

    // 清理内存
    destroy_playlist(&manager);

    return 0;
}
