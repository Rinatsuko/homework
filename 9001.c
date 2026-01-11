#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 栈元素：存储括号类型（{/}/[/]）和所在索引（便于报错定位）
typedef struct
{
    char bracket; // 括号字符
    int position; // 括号在JSON字符串中的位置（从0开始）
} StackElem;

// 栈结构体（动态扩容，避免固定容量限制）
typedef struct
{
    StackElem *data;  // 栈数据数组
    int top;          // 栈顶指针（-1表示空栈）也是代表数组下表 比如说top是2 说明栈顶元素就是data[2]
    int capacity;     // 当前栈容量
    int max_capacity; // 最大容量
} Stack;

// 栈操作结果枚举（增强健壮性）
typedef enum
{
    STACK_OK,     // 操作成功
    STACK_EMPTY,  // 栈空（弹栈/取栈顶失败）
    STACK_FULL,   // 栈满（压栈失败）
    STACK_MEM_ERR // 内存分配失败
} StackResult;

// 前向声明
int stackIsEmpty(const Stack *stack);

// ----------------------------// 学生需实现的栈接口// ----------------------------
// 1. 初始化栈（指定初始容量和最大容量）
StackResult stackInit(Stack *stack, int init_capacity, int max_capacity)
{
    // 参数合法性检查：初始容量和最大容量必须为正数，且初始容量不能超过最大容量
    if (init_capacity <= 0 || max_capacity <= 0 || init_capacity > max_capacity)
    {
        return STACK_MEM_ERR; // 非法容量参数
    }
    // 为栈数据分配初始内存空间
    stack->data = (StackElem *)malloc(sizeof(StackElem) * init_capacity);
    if (stack->data == NULL)
    {
        return STACK_MEM_ERR; // 内存分配失败
    }
    // 初始化栈顶指针为-1（空栈状态）
    stack->top = -1;
    // 设置当前容量为初始容量
    stack->capacity = init_capacity;
    // 设置最大容量限制
    stack->max_capacity = max_capacity;
    return STACK_OK;
    // 学生实现：
    // 步骤1：检查init_capacity和max_capacity合法性（init <= max，且均>0）
    // 步骤2：为stack->data分配内存（malloc）
    // 步骤3：初始化top=-1，capacity=init_capacity，max_capacity=max_capacity
    // 步骤4：返回对应结果（STACK_OK/STACK_MEM_ERR）
}

// 2. 压栈（将数据存入栈）
StackResult stackPush(Stack *stack, char bracket, int position)
{
    // 检查栈是否已满（栈顶指针+1等于容量，说明下一个位置已经没有空间了）
    if (stack->top + 1 == stack->capacity)
    {
        // 栈满，尝试扩容
        // 检查是否已达最大容量限制
        if (stack->capacity == stack->max_capacity)
        {
            return STACK_FULL; // 已达最大容量，无法扩容
        }
        // 计算新容量：通常扩容为原来的2倍
        int new_capacity = stack->capacity * 2;
        // 防止扩容后超过最大容量限制
        if (new_capacity > stack->max_capacity) // 新容量大于最大容量 则更新
        {
            new_capacity = stack->max_capacity; // 不超过最大容量
        }
        // 使用realloc重新分配内存（在原地扩容或分配新空间并复制数据）
        StackElem *new_data = (StackElem *)realloc(stack->data, sizeof(StackElem) * new_capacity); // 扩建房子本身而不是扩大房产证
        if (new_data == NULL)
        {
            return STACK_MEM_ERR; // 内存分配失败
        }
        // 更新栈的数据指针和容量
        stack->data = new_data;
        stack->capacity = new_capacity;
        // 将新元素压入栈顶（top+1的位置）
        stack->data[stack->top + 1].bracket = bracket;   // 新的栈顶元素
        stack->data[stack->top + 1].position = position; // 新的栈顶元素索引
        // 栈顶指针上移
        stack->top++;
        return STACK_OK;
    }
    else
    {
        // 栈未满，直接将元素压入栈顶+1的位置
        stack->data[stack->top + 1].bracket = bracket;
        stack->data[stack->top + 1].position = position;
        // 栈顶指针上移
        stack->top++;
        return STACK_OK;
    }

    // 学生实现：
    // 步骤1：检查栈是否满（top+1 == capacity），若满则尝试扩容（需不超过max_capacity）
    // 步骤2：扩容失败返回STACK_FULL，成功则更新capacity
    // 步骤3：栈顶指针+1，存入bracket和position
    // 步骤4：返回STACK_OK
}

// 3. 弹栈（取出栈顶元素，通过elem输出）
StackResult stackPop(Stack *stack, StackElem *elem)
{
    // 检查栈是否为空
    int a = stackIsEmpty(stack);
    if (a == 1)
    {
        return STACK_EMPTY; // 空栈无法弹出元素
    }
    // 将栈顶元素的括号字符赋值给输出参数elem
    elem->bracket = stack->data[stack->top].bracket;
    // 将栈顶元素的位置索引赋值给输出参数elem
    elem->position = stack->data[stack->top].position;
    // 栈顶指针下移（逻辑上移除栈顶元素）
    stack->top--;
    return STACK_OK;
    // 学生实现：
    // 步骤1：检查栈是否空（stackIsEmpty），空则返回STACK_EMPTY
    // 步骤2：将栈顶元素赋值给elem（bracket和position）
    // 步骤3：栈顶指针-1
    // 步骤4：返回STACK_OK
}
// 4. 查看栈顶（只查看不弹出，栈顶指针不变）
StackResult stackPeek(Stack *stack, StackElem *elem)
{
    // 检查栈是否为空
    int a = stackIsEmpty(stack);
    if (a == 1)
    {
        return STACK_EMPTY; // 空栈无法查看栈顶
    }
    // 将栈顶元素的括号字符赋值给输出参数elem（不删除）
    elem->bracket = stack->data[stack->top].bracket;
    // 将栈顶元素的位置索引赋值给输出参数elem（不删除）
    elem->position = stack->data[stack->top].position;
    return STACK_OK;
}

// 5. 判空（返回1表示空，0表示非空）
int stackIsEmpty(const Stack *stack)
{
    // 当栈顶指针为-1时，栈为空，返回1；否则返回0
    return stack->top == -1 ? 1 : 0;
    // 学生实现：返回stack->top == -1 ? 1 : 0
}

// 6. 销毁栈（释放内存）
void stackDestroy(Stack *stack)
{
    // 学生实现：释放stack->data的内存，重置top=-1、capacity=0
    // 释放动态分配的栈数据数组内存
    free(stack->data);
    // 重置栈顶指针为-1（空栈状态）
    stack->top = -1;
    // 重置容量为0
    stack->capacity = 0;
    // 重置最大容量为0
    stack->max_capacity = 0;
}
// 1为真 0为假
//  辅助函数
//  判断是否为左括号（{ 或 [）
int isLeftBracket(char ch)
{
    // 检查字符是否为 { 或 [
    return ch == '{' || ch == '[';
}
// 判断是否为右括号（} 或 ]）
int isRightBracket(char ch)
{
    // 检查字符是否为 } 或 ]
    return ch == '}' || ch == ']';
}
// 判断左右括号是否匹配
int isBracketMatch(char left, char right)
{
    // { 与 } 匹配，或者 [ 与 ] 匹配
    return (left == '{' && right == '}') || (left == '[' && right == ']');
}

// 栈基础版本实现（不考虑字符串内的括号）
int jsonBracketCheckBasic(const char *json_str)
{
    // 边界情况：空字符串或NULL视为合法
    if (json_str == NULL || strlen(json_str) == 0)
    {
        return 1; // 空字符串视为合法
    }
    // 初始化栈（初始容量10，最大容量100）
    Stack stack;
    stackInit(&stack, 10, 100);
    int len = strlen(json_str);
    // 遍历JSON字符串的每个字符
    for (int i = 0; i < len; i++)
    {
        // 遇到左括号：压入栈中
        if (json_str[i] == '{' || json_str[i] == '[')
        {
            stackPush(&stack, json_str[i], i);
        }
        // 遇到右括号：从栈中弹出左括号进行匹配
        else if (json_str[i] == '}' || json_str[i] == ']')
        {
            StackElem elem;
            StackResult res = stackPop(&stack, &elem);
            // 如果栈为空，说明右括号多余（没有对应的左括号）
            if (res == STACK_EMPTY)
            {
                stackDestroy(&stack);
                return 0; // 不合法
            }
            // 检查弹出的左括号与当前右括号是否匹配
            if (!isBracketMatch(elem.bracket, json_str[i]))
            {
                stackDestroy(&stack);
                return 0; // 括号类型不匹配
            }
        }
        // 其他字符（字母、数字、引号等）：跳过
    }
    // 遍历结束后，检查栈是否为空
    int empty = stackIsEmpty(&stack);
    stackDestroy(&stack);
    return empty; // 栈为空说明所有左括号都有对应的右括号，合法
}
// 学生实现：
// 步骤1：处理边界情况（json_str为NULL或空字符串，返回1）
// 步骤2：初始化栈（调用stackInit）
// 步骤3：遍历json_str的每个字符：
//        - 遇到左括号（{/[）：压栈（stackPush）
//        - 遇到右括号（}/]）：弹栈并检查匹配（isBracketMatch），不匹配则返回0
//        - 其他字符（如字母、数字、引号）：跳过
// 步骤4：遍历结束后，若栈为空则返回1（合法），否则返回0（左括号多余）
// 步骤5：销毁栈（stackDestroy）

// 栈进阶版本实现（考虑字符串内的括号和转义字符）
// 学生任务：学习状态管理，理解字符串内的括号不应该参与匹配，同时处理转义字符
int jsonBracketCheckAdvanced(const char *json_str)
{
    // 边界情况：空字符串或NULL视为合法
    if (json_str == NULL || strlen(json_str) == 0)
    {
        return 1;
    }
    // 初始化栈（初始容量10，最大容量100）
    Stack stack;
    stackInit(&stack, 10, 100);
    int len = strlen(json_str);
    int in_string = 0; // 标记是否在字符串内（0=不在，1=在）
    int escape = 0;    // 标记是否在转义状态（0=不在，1=在）
    // 遍历JSON字符串的每个字符
    for (int i = 0; i < len; i++)
    {
        char ch = json_str[i];
        // 如果当前在字符串内
        if (in_string)
        {
            // 如果上一个字符是转义符 \
            if (escape)
            {
                escape = 0; // 取消转义状态（当前字符被转义，无特殊含义）
            }
            else // 不在转义状态
            {
                if (ch == '\\') // 捕捉到转义符 \ （C语言中需要写成 \\\\）
                {
                    escape = 1; // 进入转义状态（下一个字符会被转义）
                }
                else if (ch == '"') // 遇到未转义的引号
                {
                    in_string = 0; // 退出字符串状态
                }
                // 字符串内的其他字符（包括括号）都忽略
            }
        }
        else // 不在字符串内
        {
            if (ch == '"') // 遇到引号
            {
                in_string = 1; // 进入字符串状态
            }
            else if (isLeftBracket(ch)) // 遇到左括号
            {
                stackPush(&stack, ch, i); // 压入栈中
            }
            else if (isRightBracket(ch)) // 遇到右括号
            {
                StackElem elem;
                StackResult res = stackPop(&stack, &elem);
                // 如果栈为空，说明右括号多余
                if (res == STACK_EMPTY)
                {
                    stackDestroy(&stack);
                    return 0; // 不合法
                }
                // 检查括号是否匹配
                if (!isBracketMatch(elem.bracket, ch))
                {
                    stackDestroy(&stack);
                    return 0; // 括号类型不匹配
                }
            }
            // 其他字符（字母、数字、冒号等）：跳过
        }
    }
    // 遍历结束后，检查栈是否为空 且 不在字符串内
    int empty = stackIsEmpty(&stack);
    stackDestroy(&stack);
    return empty && (!in_string); // 两个条件都满足才合法

    // 学生实现：
    // 步骤1：处理边界情况（json_str为NULL或空字符串，返回1）
    // 步骤2：初始化栈（调用stackInit）
    // 步骤3：初始化状态标记（in_string标记字符串状态，escape标记转义状态）
    // 步骤4：遍历json_str的每个字符：
    //        - 如果在字符串内（in_string为1）：
    //             * 如果在转义状态（escape为1）：取消转义状态，继续
    //             * 如果遇到转义字符'\\'：进入转义状态
    //             * 如果遇到引号'"'且不在转义状态：退出字符串状态
    //             * 字符串内的所有括号都不处理
    //        - 如果不在字符串内（in_string为0）：
    //             * 如果遇到引号'"'：进入字符串状态
    //             * 遇到左括号（{/[）：压栈（stackPush）
    //             * 遇到右括号（}/]）：弹栈并检查匹配（isBracketMatch），不匹配则返回0
    //             * 其他字符（如字母、数字、冒号等）：跳过
    // 步骤5：遍历结束后，检查栈是否为空且不在字符串内，两者都满足则返回1，否则返回0
    // 步骤6：销毁栈（stackDestroy）
}

int main()
{
    char str[100];
    // 输入JSON字符串
    scanf("%s", str);
    // 栈基础版
    int result1 = jsonBracketCheckBasic(str);
    // 栈进阶版
    int result2 = jsonBracketCheckAdvanced(str);
    // 返回JSON括号匹配结果，基础版只能处理简单情况，进阶版总能返回正确答案
    printf("%d %d", result1, result2);
}