#ifndef MESSAGEPROPERTYCRW_H
#define MESSAGEPROPERTYCRW_H

/*メッセージ*/
#define MESSAGE_WAITING_START "接続の待受を開始します\n"
#define MESSAGE_NOW_WAITING "接続の待受中。。。\n"
#define MESSAGE_ERROR "エラーがおきました\n"
#define MESSAGE_TIMEOUT "タイムアウトしました\n"
#define MESSAGE_UNEXPECTED_ERROR "接続待機機能にて想定外のエラーが起きました\n"
#define MESSAGE_CONNECTED "IPアドレス：%sから接続がありました\n"
#define MESSAGE_BIND_FAILED "接続の受け付け開始に失敗しました\n"
#define MESSAGE_LISTEN_FAILED "接続の受け付け開始に失敗しました\n"


/*プロパティ*/
#define PROPERTY_FAILURE (-1)
#define PROPERTY_SUCCESS (0)
#define PROPERTY_TIMEOUT_SEC (5)
#define PROPERTY_TIMEOUT_USEC (0)
#define PROPERTY_SELECT_ERROR (-1)
#define PROPERTY_SELECT_TIMEOUT (0)
#define PROPERTY_CHILD_PID (0)


#endif
