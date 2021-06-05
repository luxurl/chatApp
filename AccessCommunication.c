
#include"AccessCommunication.h"
#include"./common.h"
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>

/*
 *通信接続関数
 *引数 ソケットのファイルディスクリプタ
 *戻値 通信接続失敗の場合：1 成功の場合：0
 **/
int AccessCommunication(int *socketFileDescriptor){
  
  	/*InterNet用ソケットのアドレス情報を表す構造体*/
  	struct sockaddr_in connectionProperty;
	/*処理ループ用フラグ*/
  	bool loopFlag = true;
	/*ipアドレス入力用配列*/
	char read_ipAddInput[16] = {0};   
	/*確認関数用変数*/
	char confirmationResult = 'N';
	/*構造体の初期化ー先頭から構造体のサイズ分を0で埋める*/
	memset(&connectionProperty,0,sizeof(connectionProperty));
	/*IPアドレスチェック結果*/
	bool IPaddCheck = false;
	/*文字列からネットワークバイトオーダーに変換後のIPアドレスの格納場所*/
	struct in_addr IpAdress;
	/*構造体の初期化ー先頭から構造体のサイズ分を0で埋める*/
	memset(&IpAdress,0,sizeof(IpAdress));
	/*戻値*/
	int connectionCheck = -1;
	
	
	/*ソケットの作成ー失敗した場合-１をリターンする*/
	/*失敗した場合-１をリターンするー成功した場合ソケット用のアドレスにソケットのファイルディスクリプターを格納する*/
	if((*socketFileDescriptor = socket(AF_INET,SOCK_STREAM,0))== -1){
		perror("socket");
		/*ソケットエラー用メッセージ*/
		printf("%s",SOCKET_ERROR_MESSE);
		/*操作選択に戻るメッセージ*/
		printf("%s",RETURN_OPERATION_FORCED);
		return -1;
	}
	
	/*InterNet用アドレスファミリーの設定*/
	connectionProperty.sin_family = AF_INET;
	
	/*ホストバイト順序で表現された16ビット整数をネットワークバイト順序に変換*/
	connectionProperty.sin_port = htons(PORT_NUMBER);
	
	loopFlag = true;
	
	/*loopFalgがtrueのあいだ以下の処理をループ*/
	while(loopFlag == true){
	  
		/*IPアドレスの入力をうながすメッセージ*/
		printf("%s",IP_ADD_INPUT_MESSE);
		
		/*標準入力から17文字読み取る*/
		fgets(read_ipAddInput,16,stdin);
		
		/*標準入力初期化関数*/
		Initialization(read_ipAddInput);
		
		printf("IPアドレスは%sでよろしいでしょうか[Y/N]\n",read_ipAddInput);
		
		/*確認関数*/
		confirmationResult =confirmation();
		
		
		/*'Y'が入力されているか*/
		switch (confirmationResult){
		  /*確認処理がYESの場合*/
			case 'Y':
				/*入力されたIPアドレスのチェック*/
				IPaddCheck = inet_pton(AF_INET,(const char *)read_ipAddInput, &IpAdress);
				
				if(IPaddCheck == 1){
					/*IPアドレスのチェックがOKである旨のメッセージ*/
					printf("%s",IP_CHECK_CONPRETION);
					/*接続先のIPアドレスを設定*/
					connectionProperty.sin_addr =IpAdress;
					/*接続開始のメッセージ*/
					printf("%s",CONNECT_START);
	
					/*入力されたIPアドレスのチェックがOKならconnection関数呼出*/
					connectionCheck =connect(*socketFileDescriptor, (struct sockaddr *) &connectionProperty, sizeof(connectionProperty));
					if(connectionCheck == -1){
						/*接続に失敗した旨のメッセージ*/
						printf("%s",CONNECTION_FAILED);
					}
				}else{
					/*入力されたIPアドレスのチェックがNGならエラー表示*/
					printf("%s",INPUT_CORRECT_IP_ADD);
					connectionCheck = -1;
				}
				break;      

				/*確認処理がNOの場合*/
	  		case 'N':
				/*操作選択にもどるかの確認処理に入る*/
				connectionCheck = -1;
				break;
      
	  		default:
				break;
		}
    
		if(connectionCheck == -1){
			
			/*操作選択まで戻るか確認メッセージ*/
			printf("%s",CONFIRMATION_RETURN_OPERATION);
			/*確認関数*/
			confirmationResult =confirmation();
			
			if(confirmationResult == 'Y'){
				/*操作選択に戻るメッセージ*/
				printf("%s",RETURN_OPERATION);
				/*ループから抜ける*/
				loopFlag = false;
			}

		}else{
      
      		/*接続が成功した旨のメッセージ*/
      		printf("%s",CONNECTION_SUCCESS);
			/*connection関数が成功している場合ループを抜ける*/
			loopFlag = false;
			
		} 
	}
	return connectionCheck;
}
