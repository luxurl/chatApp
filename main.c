#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include"common.h"


int main(void){

	/*操作選択機能から、メッセージ送受信機能までをループさせるためのフラグ*/
	bool loopFlag;
	/*標準入力用配列*/
	char read_str[3] = {'\0'}; 
	/*操作選択番号*/
	int operationNumber;
	/*通信接続機能または接続待機機能の処理が成功しているかの判定用変数*/
	int success = -1;
	/*接続が完了したソケットの識別子を格納する変数*/
	int socket;
  
  
	loopFlag = true;
  
	/*loopFlgがtrueのあいだ以下の処理をループ*/
	while(loopFlag == true){
    
		printf("数字を入力し操作を選択してください\n");
		printf("1.IPアドレスを入力し、通信を行う\n");
		printf("2.通信を待機する\n");
		printf("3.このアプリケーションを終了する\n");
    
		/*操作選択番号の初期値を設定*/
		operationNumber = 0;
    
		/*1または2または3が入力されるまでループ*/
		while( operationNumber != 1 && operationNumber != 2 && operationNumber != 3){
      
			/*標準入力から2文字読み取る*/
			fgets(read_str,3,stdin);
			/*読み取った文字を数値に変換する、数字ではない文字が入力された場合、0になる*/
			sscanf(read_str,"%d",&operationNumber);

			/*1または2または3が入力されいない場合以下のメッセージを表示*/
			if( operationNumber != 1 && operationNumber != 2 && operationNumber != 3){
				printf("1~3の数字で入力してください\n");
			}

			/*以下、標準入力の初期化*/
			/*文字配列の先頭から改行文字を探索した結果、存在した場合*/
			if(strchr(read_str,'\n') != NULL){
				/*標準入力用の配列の配列の長さ-1の場所に終了を表す文字をいれる*/
				read_str[strlen(read_str) -1 ] = '\0';
			}else{
				/*改行文字が存在しない場合*/
				/*入力ストリームの文字を空読みする*/
				while(fgetc(stdin) != '\n');
			}
			/*標準入力初期化終了*/
		}
    
		/*入力値による操作選択*/
		switch(operationNumber){
			/*1が入力されていた場合、通信接続関数呼出し*/
			case 1:
				success = AccessCommunication(&socket);
				break;
				/*2が入力されていた場合、接続待機関数呼出し*/
			case 2:
				success =  ConnectionRequestWaiting(&socket);
				break;
				/*3が入力されていた場合、このアプリケーションを終了する*/
			case 3:
				printf("アプリケーションを終了します\n");
				/*チャット関数に入らないよう-1を設定*/
				success = -1;
				loopFlag = false;
				break;
				/*1.2.3以外の数字は入ることは無いが、念のため。操作選択処理にもどす*/
			default:
				printf("操作選択機能にて想定外のエラーが起きました\n");
				printf("最初から操作をやりなおしてください\n");
				break;
		}

		/*通信接続関数または、接続待機関数が通信の確立に成功した場合、メッセージ送受信関数呼出し*/
		if(success != -1){

			chat(socket);
			/*接続待機関数で作成した子プロセスの場合、操作選択には戻らず正常に終了させる*/ 
			if(operationNumber == 2){
				loopFlag = false;
			}
      

		}

		/*ソケットを終了*/
		shutdown(socket, SHUT_RDWR);
		close(socket);


	}
	return 0; 
}
