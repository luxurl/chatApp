#include"ConnectionRequestWaiting.h"
#include"./common.h"
#include"./MessagePropertyCRW.h"
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

/**接続受付関数 プロトタイプ宣言*/
static int WaitingConnection(int socketFileDescriptor , int *acceptSocket);

/**
 *接続待機関数
 *引数 ソケットのファイルディスクリプタ
 *戻値 接続受付失敗の場合：-1 成功の場合：0
 */
int ConnectionRequestWaiting(int *acceptSocket){
  
	/*接続要求待ち用のソケット*/
	int socketFileDescriptor = 0;
	/*InterNet用ソケットのアドレス情報を表す構造体*/
	struct sockaddr_in serverProperty = {0};
	/*戻値*/
	int connectionResult = PROPERTY_FAILURE;

	/*ソケットの作成*/
	if((socketFileDescriptor = socket(AF_INET,SOCK_STREAM,0)) != PROPERTY_FAILURE){

		/*パラメータ設定*/
		serverProperty.sin_family = AF_INET;  
		serverProperty.sin_port = htons(PROPERTY_PORT_NUMBER);  
		serverProperty.sin_addr.s_addr = htonl(INADDR_ANY);
	
		/*サーバ側のアドレスやポートの設定をソケットに登録*/
		if(bind(socketFileDescriptor,(struct sockaddr *)&serverProperty, sizeof(serverProperty)) !=  PROPERTY_FAILURE){
	
			printf(MESSAGE_WAITING_START);

			/*接続の受け付け開始*/
			if (listen(socketFileDescriptor, SOMAXCONN) !=  PROPERTY_FAILURE) {
				
				connectionResult = WaitingConnection(socketFileDescriptor,acceptSocket);

			}else{
				printf(MESSAGE_LISTEN_FAILED);
				printf(MESSAGE_RETURN_OPERATION_FORCED);
				*acceptSocket = socketFileDescriptor;
				connectionResult = PROPERTY_FAILURE;
			}
		}else{
			printf(MESSAGE_BIND_FAILED);
			printf(MESSAGE_RETURN_OPERATION_FORCED);
			*acceptSocket = socketFileDescriptor;
			connectionResult = PROPERTY_FAILURE;
		}
	}else{
		printf(MESSAGE_SOCKET_ERROR);
		printf(MESSAGE_RETURN_OPERATION_FORCED);
		connectionResult = PROPERTY_FAILURE;
	}

	return connectionResult;

}	

/**接続受付関数 
 *
 *引数1 接続受付用のソケット
 *引数2 通信用のソケット
 *戻値  接続受付が成功して場合：0 失敗した場合：-1
*/
static int WaitingConnection(int socketFileDescriptor, int *acceptSocket){


	/*クライアントのアドレス情報を格納する構造体*/
	struct sockaddr_in cliantProperty = {0};
	/*クライアントのアドレス情報の長さ*/
	socklen_t cliantPropertylength = sizeof(cliantProperty);
	/*監視するファイルディスクリプタ*/
	fd_set readFileDescriptor ;
	/*ファイルディスクリプタの最大値+1を入れるための変数*/
	int fileDescriptorMaxNumber = 0;
	/*タイムアウト用設定値*/
	struct timeval timeoutSeccond = {0};
	/*プロセスID格納用*/
	pid_t pid = 0;
	/*wait関数の引数用*/
	int status = 0;
	/*判定用*/
	int acceptResult = PROPERTY_FAILURE;
	/*判定用*/
	bool loopFlag = true;

	/* fd_setの初期化*/
	FD_ZERO(&readFileDescriptor);

	while(1){
					
		printf(MESSAGE_NOW_WAITING);
		
		/*監視用の設定*/
		FD_ZERO(&readFileDescriptor);
		FD_SET(socketFileDescriptor, &readFileDescriptor);
		fileDescriptorMaxNumber = socketFileDescriptor +1;
		timeoutSeccond.tv_sec = PROPERTY_TIMEOUT_SEC;
		timeoutSeccond.tv_usec = PROPERTY_TIMEOUT_USEC;

		/*ソケットが受信するまで処理を停止*/
		switch (select(fileDescriptorMaxNumber, (fd_set *) &readFileDescriptor, NULL, NULL, &timeoutSeccond)) {
				
			/* エラーの場合 */
			case PROPERTY_SELECT_ERROR:
				printf(MESSAGE_ERROR);
				printf(MESSAGE_RETURN_OPERATION_FORCED);
				*acceptSocket = socketFileDescriptor;
				break;
				
			/* タイムアウトの場合 */
			case PROPERTY_SELECT_TIMEOUT:
				printf(MESSAGE_TIMEOUT);
				printf(MESSAGE_RETURN_OPERATION_FORCED);
				*acceptSocket = socketFileDescriptor;
				break;
				
			/*接続の受付*/
			default:							
				cliantPropertylength = (socklen_t)sizeof(cliantProperty);
				if ((*acceptSocket = accept(socketFileDescriptor, (struct sockaddr *)&cliantProperty, &cliantPropertylength)) == PROPERTY_FAILURE) {
					printf(MESSAGE_UNEXPECTED_ERROR);
					*acceptSocket = socketFileDescriptor;
				}else{
				acceptResult = PROPERTY_SUCCESS;
				printf(MESSAGE_CONNECTED,inet_ntoa(cliantProperty.sin_addr));
				}
				break;
		}

		if(PROPERTY_SUCCESS == acceptResult){
		
			/*子プロセスを作成する*/
			if ((pid = fork()) == PROPERTY_CHILD_PID) {
				/*子プロセスの処理 */
				close(socketFileDescriptor);
				loopFlag = false;

			} else if (pid > 0) {
				/* fork()成功：親プロセスの処理 */
				/* 通信用ソケットのクローズ */
				(void) close(*acceptSocket);
			
				/*子プロセスが終了するまで親プロセスは待機*/
				pid = wait(&status); 
						
				if (pid == PROPERTY_FAILURE){
					printf(MESSAGE_UNEXPECTED_ERROR);
					printf(MESSAGE_RETURN_OPERATION_FORCED);
					(void) close(*acceptSocket);
					*acceptSocket = socketFileDescriptor;
					acceptResult = PROPERTY_FAILURE;
					loopFlag = false;
				}
			
			/* fork()失敗 */
			} else {
				(void) close(*acceptSocket);
				*acceptSocket = socketFileDescriptor;
				acceptResult = PROPERTY_FAILURE;
				loopFlag = false;
			}
		}else{
			loopFlag = false;
		}

		if(!loopFlag){
			break;
		}
	}

	return acceptResult;

}
