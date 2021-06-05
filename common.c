#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"common.h"

/*
 *ユーザーの意思を確認する関数
 *引数 なし
 *戻値 char型 YまたはN
 *
 */

char confirmation(void){
  
  /*確認処理入力用配列*/
  char read_confirmation[3] = {0};
  
  /*yまたはnが入力されるまでループ*/
  while((strncmp(read_confirmation, "Y\0",2)) != 0 && (strncmp(read_confirmation, "N\0",2)) !=0){
    
    /*標準入力から2文字読み取る*/
    fgets(read_confirmation,3,stdin);
    
    /*標準入力の初期化処理*/
    Initialization(read_confirmation);
    
    
    if((strncmp(read_confirmation, "Y\0",2)) != 0 && (strncmp(read_confirmation, "N\0",2)) !=0){

      /*YまたはNを入力するよう注意するメッセージ*/
      printf("%s",INPUT_CORRECT_YN);
      
    }
  }
  
  return read_confirmation[0];
  
}

/*
 *標準入力を初期化する関数
 *引数 配列のポインタ
 *戻値 なし
 *
 */
void Initialization(char *Inputstdin){
  
  int dummyStdin = 0;
  
  /*文字配列の先頭から改行文字を探索した結果、存在した場合*/
  if(strchr(Inputstdin,'\n') != NULL){
    /*標準入力用の配列の配列の長さ-1の場所に終了を表す文字をいれる*/
    Inputstdin[strlen(Inputstdin) -1 ] = '\0';
  }else{
    
    dummyStdin = 0;
    /*改行文字が存在しない場合*/
    /*入力ストリームの文字を空読みする*/
    while(dummyStdin != '\n'){

      dummyStdin = fgetc(stdin);

      /*標準入力に改行文字が存在しなかった場合、EOFを読取り、ループから抜ける*/
      if(dummyStdin== EOF){

	break;

      }
    }
  }
  
}
