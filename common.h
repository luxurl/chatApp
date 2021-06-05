#ifndef COMMON_H
#define COMMON_H

#include<stdbool.h>

extern bool confirmation(void);
extern void Initialization(char *systemDotIn);

/*共通標準出力メッセージと設定値開始*/
#define MESSAGE_SOCKET_ERROR "ソケットの作成に失敗しました\n"
#define MESSAGE_RETURN_OPERATION_FORCED "操作を最初からやり直してください\n"
#define MESSAGE_INPUT_CORRECT_YN "'Y'または'N'を入力してください\n"

/*共通設定値*/
#define PROPERTY_PORT_NUMBER (49152)

#endif


