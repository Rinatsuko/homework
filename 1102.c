#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 歌曲节点结构体
typedef struct Song
{
    int id;
    char title[100];
    char artist[50];
    char filepath[300];
    struct Song *prev;
    struct Song *next;
} Song;

// 播放列表管理器
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
int delete_song_by_title(PlaylistManager *manager, const char *title); // 删除指定名字的音乐
int play_song_by_title(PlaylistManager *manager, const char *title);   // 根据名字播放音乐
void display_playlist(PlaylistManager *manager);                       // 显示播放列表（正向）
void display_playlist_reverse(PlaylistManager *manager);               // 显示播放列表（反向）
int export_playlist(PlaylistManager *manager, const char *filename);   // 导出歌单
void next_song(PlaylistManager *manager);                              // 下一首歌
void previous_song(PlaylistManager *manager);                          // 上一首歌
int play_song_random(PlaylistManager *manager);                        // 随机播放音乐（非必做）
void sort_by_title(PlaylistManager *manager);                          // 按照歌曲名排序（非必做）
void destroy_playlist(PlaylistManager *manager);                       // 清空播放列表

// 播放音频函数 (Windows版本)
void play_audio(const char *filename)
{
    char command[512];
    FILE *mp3File = fopen(filename, "rb");
    if (!mp3File)
    {
        printf("无法找到文件: %s\n", filename);
        return;
    }
    fclose(mp3File);

    // Windows: 使用 start 命令调用默认播放器
    snprintf(command, sizeof(command), "start \"\" \"%s\"", filename);
    int ret = system(command);
    if (ret != 0)
    {
        printf("播放失败，请检查文件关联或路径。\n");
    }
}

// 初始化播放管理器
void init_playlist_manager(PlaylistManager *manager)
{
    manager->head = NULL;
    manager->tail = NULL;
    manager->current = NULL;
    manager->song_count = 0;
}

// 0. 从文件读取内容构建双向链表
int load_songs_from_file(PlaylistManager *manager, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return -1;

    char title[100], artist[50], filepath[300];
    // 使用格式化读取，处理 CSV 格式
    while (fscanf(file, " %[^,],%[^,],%s", title, artist, filepath) == 3)
    {
        add_song(manager, title, artist, filepath);
    }
    fclose(file);
    return 0;
}

// 1. 在链表末尾添加歌曲
void add_song(PlaylistManager *manager, const char *title, const char *artist, const char *filepath)
{
    Song *newsong = (Song *)malloc(sizeof(Song));
    if (!newsong)
        return;

    newsong->id = manager->song_count + 1;
    strcpy(newsong->title, title);
    strcpy(newsong->artist, artist);
    strcpy(newsong->filepath, filepath);
    newsong->next = NULL;

    if (manager->head == NULL)
    {
        newsong->prev = NULL;
        manager->head = newsong;
        manager->tail = newsong;
    }
    else
    {
        newsong->prev = manager->tail;
        manager->tail->next = newsong;
        manager->tail = newsong;
    }
    manager->song_count++;
}

// 2. 按标题删除歌曲
int delete_song_by_title(PlaylistManager *manager, const char *title)
{
    Song *temp = manager->head;
    while (temp != NULL && strcmp(title, temp->title) != 0)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("未找到歌曲: %s\n", title);
        return -1;
    }

    if (temp == manager->current)
        manager->current = NULL;

    if (temp->prev)
        temp->prev->next = temp->next;
    else
        manager->head = temp->next;

    if (temp->next)
        temp->next->prev = temp->prev;
    else
        manager->tail = temp->prev;

    free(temp);
    manager->song_count--;
    printf("成功删除: %s\n", title);
    return 0;
}

// 3. 播放歌曲
int play_song_by_title(PlaylistManager *manager, const char *title)
{
    Song *temp = manager->head;
    while (temp != NULL && strcmp(title, temp->title) != 0)
    {
        temp = temp->next;
    }

    if (temp)
    {
        printf("正在播放: %s - %s\n", temp->title, temp->artist);
        play_audio(temp->filepath);
        manager->current = temp;
        return 0;
    }
    printf("未找到歌曲: %s\n", title);
    return -1;
}

// 4. 显示播放列表（正向遍历）
void display_playlist(PlaylistManager *manager)
{
    Song *temp = manager->head;
    if (!temp)
    {
        printf("播放列表为空\n");
        return;
    }
    printf("\n当前播放列表:\n");
    while (temp)
    {
        printf("[%d] %s - %s\n", temp->id, temp->title, temp->artist);
        temp = temp->next;
    }
}

// 5. 显示播放列表（反向遍历）
void display_playlist_reverse(PlaylistManager *manager)
{
    Song *temp = manager->tail;
    if (!temp)
    {
        printf("播放列表为空\n");
        return;
    }
    printf("\n当前播放列表 (逆序):\n");
    while (temp)
    {
        printf("[%d] %s - %s\n", temp->id, temp->title, temp->artist);
        temp = temp->prev;
    }
}

// 6. 将播放列表保存到文件
int export_playlist(PlaylistManager *manager, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
        return -1;
    Song *temp = manager->head;
    while (temp)
    {
        fprintf(file, "%s,%s,%s\n", temp->title, temp->artist, temp->filepath);
        temp = temp->next;
    }
    fclose(file);
    printf("列表已导出至 %s\n", filename);
    return 0;
}

// 7. 下一首
void next_song(PlaylistManager *manager)
{
    if (!manager->head)
        return;
    if (!manager->current || !manager->current->next)
        manager->current = manager->head;
    else
        manager->current = manager->current->next;

    printf("切换至下一首: %s\n", manager->current->title);
    play_audio(manager->current->filepath);
}

// 8. 上一首
void previous_song(PlaylistManager *manager)
{
    if (!manager->head)
        return;
    if (!manager->current || !manager->current->prev)
        manager->current = manager->tail;
    else
        manager->current = manager->current->prev;

    printf("切换至上一首: %s\n", manager->current->title);
    play_audio(manager->current->filepath);
}

// 9. 随机播放歌曲（非必做）
int play_song_random(PlaylistManager *manager)
{
    if (manager->song_count == 0)
        return -1;
    int target = rand() % manager->song_count;
    Song *temp = manager->head;
    for (int i = 0; i < target; i++)
        temp = temp->next;
    manager->current = temp;
    printf("随机播放: %s\n", temp->title);
    play_audio(temp->filepath);
    return 0;
}

// 11. 清空播放列表
void destroy_playlist(PlaylistManager *manager)
{
    Song *curr = manager->head;
    while (curr)
    {
        Song *next = curr->next;
        free(curr);
        curr = next;
    }
    init_playlist_manager(manager);
    printf("播放列表已清空\n");
}

// 10. 按歌曲标题排序（非必做）
void sort_by_title(PlaylistManager *manager)
{
    if (manager->song_count < 2)
        return;
    int swapped;
    Song *ptr1;
    Song *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = manager->head;
        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->title, ptr1->next->title) > 0)
            {
                // 仅交换数据域，不破坏链表结构
                char t_title[100], t_artist[50], t_path[300];
                strcpy(t_title, ptr1->title);
                strcpy(t_artist, ptr1->artist);
                strcpy(t_path, ptr1->filepath);

                strcpy(ptr1->title, ptr1->next->title);
                strcpy(ptr1->artist, ptr1->next->artist);
                strcpy(ptr1->filepath, ptr1->next->filepath);

                strcpy(ptr1->next->title, t_title);
                strcpy(ptr1->next->artist, t_artist);
                strcpy(ptr1->next->filepath, t_path);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("按歌名排序完成。\n");
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void get_user_input(char *buffer, int size, const char *prompt)
{
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
}

void display_menu()
{
    printf("\n");
    printf("链表音乐播放器管理器\n");
    printf("==========================================\n");
    printf("1. 添加歌曲\n");
    printf("2. 删除歌曲 (按标题)\n");
    printf("3. 播放歌曲 (按标题)\n");
    printf("4. 显示播放列表 (正向)\n");
    printf("5. 显示播放列表 (逆向)\n");
    printf("6. 导出歌单\n");
    printf("7. 切换到下一首歌\n");
    printf("8. 切换到上一首歌\n");
    printf("9. 随机播放歌曲(非必做)\n");
    printf("10. 按照歌曲名排序(非必做)\n");
    printf("11. 清空播放列表\n");
    printf("0. 退出程序\n");
    printf("==========================================\n");
    printf("请选择操作 (0-11): ");
}

int main()
{
    srand((unsigned int)time(NULL));
    PlaylistManager manager;
    init_playlist_manager(&manager);
    load_songs_from_file(&manager, "song_list.txt");

    printf("=== 双向链表音乐播放器管理器 ===\n");
    printf("已加载 %d 首示例歌曲\n", manager.song_count);
    manager.current = manager.head;
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
            float duration;

            get_user_input(title, sizeof(title), "请输入歌曲标题: ");
            get_user_input(artist, sizeof(artist), "请输入作者: ");
            get_user_input(filepath, sizeof(filepath), "请输入歌曲路径: ");

            add_song(&manager, title, artist, filepath);
            break;
        }
        case 2:
        { // 删除歌曲 (按标题)
            char title[100];
            get_user_input(title, sizeof(title), "请输入要删除的歌曲标题: ");
            int res = delete_song_by_title(&manager, title);
            break;
        }
        case 3:
        { // 播放歌曲（按歌曲名）
            char title[100];
            get_user_input(title, sizeof(title), "请输入要播放的歌曲标题: ");
            int res = play_song_by_title(&manager, title);
            break;
        }
        case 4:
        { // 显示播放列表（正向）
            display_playlist(&manager);
            break;
        }
        case 5:
        { // 显示播放列表（逆向）
            display_playlist_reverse(&manager);
            break;
        }
        case 6:
        { // 导出播放列表
            char path2export[300];
            get_user_input(path2export, sizeof(path2export), "请输入要导出的目标文件名: ");
            int res = export_playlist(&manager, path2export);
            break;
        }
        case 7:
        { // 播放下一首歌曲
            next_song(&manager);
            break;
        }
        case 8:
        { // 播放上一首歌曲
            previous_song(&manager);
            break;
        }
        case 9:
        { // 随机播放歌曲(非必做)
            int res = play_song_random(&manager);
            break;
        }
        case 10:
        { // 按照歌曲名排序(非必做)
            sort_by_title(&manager);
            break;
        }
        case 11:
        { // 清空播放列表
            destroy_playlist(&manager);
            break;
        }
        case 0: // 退出程序
            printf("感谢使用链表音乐播放器管理器!\n");
            break;
        default:
            printf("? 无效选择，请重新输入\n");
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