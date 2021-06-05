#include"chat.h"
#include<stdio.h>						
#include<string.h>						
#include<sys/types.h>						
#include<sys/socket.h>						
#include<netinet/in.h>
#include<stdbool.h>
#include <arpa/inet.h>
#include <err.h>
#include <errno.h>


void chat(int socket){
  
  
	/*処理のループ用フラグ*/
	bool loopFlag;
	/*監視するファイルディスクリプタ*/
	fd_set readFileDescriptor;
	/*受信用バッファ*/
	char readBuffer[256] = {'\0'};
	/*受信したメッセージのサイズ*/
	ssize_t readLength;
	/*送信用バッファ*/
	char sendBuffer[256] = {'\0'};
	/*送信したメッセージのサイズ*/
	ssize_t sendLength;
	/*ファイルディスクリプタの最大値+1を入れるための変数*/
	int fileDescriptorMaxNumber;
	/*タイムアウト用設定値*/
	struct timeval timeout;
	/*確認処理入力用配列*/
	char read_confirmation[3] = {'\0'};
  
  
  
	printf("チャットログを開始します\n");
	printf("終了する場合はEXITと入力してください\n");

	loopFlag = false;
	/*falseの間以下の処理をループ*/
	while(loopFlag == false){
    
		/* fd_setの初期化*/
		FD_ZERO(&readFileDescriptor);
		/*監視対象のファイルディスクリプタを入れる*/
		FD_SET(socket, &readFileDescriptor);
		/*標準入力も監視対象にする*/
		FD_SET(0, &readFileDescriptor);
		/* select()の第一引数に使用,接続待ちソケット + 1 */
		fileDescriptorMaxNumber = socket +1;
		/*タイムアウト値の設定*/
		timeout.tv_sec = 3600;
		timeout.tv_usec = 0;
		/*ファイルディスクリプターの監視を開始*/
		switch (select(fileDescriptorMaxNumber, (fd_set *) &readFileDescriptor, NULL, NULL, &timeout)) {      
			/* エラーの場合 */
			case -1:
				printf("相手との通信がきれました操作を最初からやり直してください\n");
				break;
      
				/* タイムアウトの場合 */
			case 0:
				printf("タイムアウトしました最初から操作をやり直してください\n");     
				break;
      
				/*ソケットに何か受信した場合、または、標準入力をなにか検知した場合*/
			default:
				/*メッセージを受信した場合、受信処理を行い表示する。*/
				if(FD_ISSET(socket,&readFileDescriptor)){
					readLength=recv(socket,readBuffer,sizeof(readBuffer),0);
					/*エラーがおきた場合、ソケットを終了する*/
					if(readLength<=0){
						loopFlag = true;
						break;
					}
	
					/*メッセージ取得用の配列を初期化*/
					readBuffer[readLength]='\0';
					/*標準出力で表示すると他の機能で使用されている可能性があるため*/
					/*受信したメッセージをバッファリングされない、標準エラー出力に表示*/
					fprintf(stderr,"%s",readBuffer);
				}
				/*標準入力に入力があった場合*/
				if(FD_ISSET(0,&readFileDescriptor)){
					/*標準入力から文字列を読込む*/
					fgets(sendBuffer,sizeof(sendBuffer),stdin);
	
					/*入力された文字がEXITだけかどうかを判定*/
					if(strncmp(sendBuffer, "EXIT\n",5) == 0){
	  
						/*EXITが入力された場合、チャットログの終了確認を行う*/	  
						/*yまたはnが入力されるまでループ*/
						while( (read_confirmation[0] != 'Y' && read_confirmation[0] != 'N' )|| read_confirmation[1] != '\0'){
	    
							printf("チャットログを終了しますか？[Y/N]\n");
	    
							/*標準入力から2文字読み取る*/
							fgets(read_confirmation,3,stdin);
	    
							/*以下、標準入力を初期化処理*/
							/*文字配列の先頭から改行文字を探索した結果、存在した場合*/
							if(strchr(read_confirmation,'\n') != NULL){
	      
								/*標準入力用の配列の配列の長さ-1の場所に終了を表す文字をいれる*/
								read_confirmation[strlen(read_confirmation) -1 ] = '\0';
	      
							}else{
								/*改行文字が存在しない場合*/
								/*入力ストリームの文字を空読みする*/
								while(fgetc(stdin) != '\n');
							}
							/*標準入力の初期化処理終了*/
	    
							/*Y、N以外は入力をやりなおすようメッセージを出力する*/
							if((read_confirmation[0] != 'Y' && read_confirmation[0] != 'N')
							   || read_confirmation[1] != '\0'){
	      
								printf("'Y'または'N'を入力してください\n");
	      
							}else{
								/*Yが入力されていた場合、終了する*/
								if(read_confirmation[0] == 'Y'){
									printf("'チャットログを終了します\n");
									loopFlag = true;
								}
							}
						}
	  
						/*確認処理2回目用の初期化*/ 
						read_confirmation[0] = '\0';
						read_confirmation[1] = '\0';

					}else{
						/*標準入力が１文字以上の場合メッセージをおくる*/
						if(strlen(sendBuffer)>0){
							sendLength=send(socket,sendBuffer,strlen(sendBuffer),0);
							/*エラーが発生した場合、操作選択からやりなおす*/
							if(sendLength== -1){
								perror("send");
								printf("メッセージの送信に失敗しました\n");
								printf("操作を最初からやり直してください\n");
								loopFlag = true;
							}
						}
					}
				} 
				break;
		} 
	} 
}
