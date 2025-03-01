/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
*/
/*
 *               Emotion Engine Library Sample Program
 *
 *                           - <scf> -
 *
 *                          Version <1.00>
 *                               SJIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            <scf.c>
 *                    <check system configuration>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.00            July,31,2000    kumagae     first version
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include <sifdev.h>
#include <sifrpc.h>
#include <libcdvd.h>
#include <libscf.h>
#include <eekernel.h>
#include <libdev.h>
#include <string.h>
#include <assert.h>
#include <sifcmd.h>

#define SCF_FILE "host0:/usr/local/sce/ee/sample/scf/T10000.scf"

#define SCREEN_WIDTH	640
#ifdef PAL
#define SCREEN_HEIGHT   256
#define SECOND 50
#else
#define SCREEN_HEIGHT	224
#define SECOND 60
#endif


#define OFFX		(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY		(((4096-SCREEN_HEIGHT)/2)<<4)

sceGsDBuff db;
#define MAX_LANGUAGE 8 
#define MAX_ASPECT 3
#define MAX_SPDIF 2
#define MAX_DATENOTATION 3
#define MAX_SUMMERTIME 2
#define MAX_TIMENOTATION 2
#define MAX_INDEX 7

#define INDEX_SPDIF 0
#define INDEX_ASPECT 1
#define INDEX_LANGUAGE 2
#define INDEX_TIMEZONE 3
#define INDEX_SUMMERTIME 4
#define INDEX_TIMENOTATION 5
#define INDEX_DATENOTATION 6


struct Index{
  int index;
  char *str;
};

const struct Index Index[MAX_INDEX]={ 
  {INDEX_SPDIF,"[SPDIF]"},
  {INDEX_ASPECT,"[ASPECT]"},
  {INDEX_LANGUAGE,"[LANGUAGE]"},
  {INDEX_TIMEZONE,"[TIMEZONE]"},
  {INDEX_SUMMERTIME,"[SUMMERTIME]"},
  {INDEX_TIMENOTATION,"[TIMENOTATION]"},
  {INDEX_DATENOTATION,"[DATENOTATION]"}
};
const struct Index Language[MAX_LANGUAGE+1]={
  {SCE_JAPANESE_LANGUAGE,"Japanese"},
  {SCE_ENGLISH_LANGUAGE,"English"},
  {SCE_FRENCH_LANGUAGE,"French"},
  {SCE_SPANISH_LANGUAGE,"Spanish"},
  {SCE_GERMAN_LANGUAGE,"German"},
  {SCE_ITALIAN_LANGUAGE,"Italian"},
  {SCE_DUTCH_LANGUAGE,"Dutch"},
  {SCE_PORTUGUESE_LANGUAGE,"Portuguese"},
  {MAX_LANGUAGE,""}
};
const struct Index Aspect[MAX_ASPECT+1]={
  {SCE_ASPECT_43,"4:3"},
  {SCE_ASPECT_FULL,"FULL"},
  {SCE_ASPECT_169,"16:9"},
  {MAX_ASPECT,""}
};
const struct Index  Spdif[MAX_SPDIF+1]={
  {SCE_SPDIF_ON,"ON"},
  {SCE_SPDIF_OFF,"OFF"},
  {MAX_SPDIF,""}
};
const struct Index DateNotation[MAX_DATENOTATION+1]={
  {SCE_DATE_YYYYMMDD,"YYYY/MM/DD"},
  {SCE_DATE_MMDDYYYY,"MM/DD/YYYY"},
  {SCE_DATE_DDMMYYYY,"DD/MM/YYYY"},
  {MAX_DATENOTATION,""}
};
const struct Index SummerTime[MAX_SUMMERTIME+1]={
  {SCE_SUMMERTIME_OFF,"OFF"},
  {SCE_SUMMERTIME_ON,"ON"},
  {MAX_SUMMERTIME,""}
};
const struct Index TimeNotation[MAX_TIMENOTATION+1]={
  {SCE_TIME_24HOUR,"24HOUR"},
  {SCE_TIME_12HOUR,"12HOUR"},
  {MAX_TIMENOTATION,""}
};


int ConsoleDraw(int console);
void Format(char *str, sceCdCLOCK *prtc, int dateformat, int timenotation);
void SetConfig(char *str);
char * MallocRead(char *pfilename);
int GetFileSize(int fd);

/* GetFileSize
 * ファイルのサイズを返す関数
 * int fd ファイル識別子
 * 戻り値 ファイルサイズ
 */
int GetFileSize(int fd)
{
  int curpos;
  int filesize;
  assert(fd >= 0);
  if((curpos = sceLseek(fd, 0, SCE_SEEK_CUR))<0)
    return 0;
  if((filesize = sceLseek(fd, 0, SCE_SEEK_END)) < 0)
    return 0;
  if(sceLseek(fd, curpos, SCE_SEEK_SET)< 0)
    return 0;
  return filesize;
}

/* MallocRead
 * ファイルを読む関数
 * char *pfilename ファイル名へのポインタ
 * char *pData データへのポインタ
 * 戻り値 成功時pData 
 *   ファイルオープン失敗時,メモリ割り当て失敗時 NULL
 */
char * MallocRead(char *pfilename)
{
  char *pData;
  int filesize;
  int fd;
  assert(pfilename);
  
  if((fd = sceOpen(pfilename, SCE_RDONLY)) <0){
    printf("Cannot open %s\n", pfilename);
    return NULL;
  }
  filesize = GetFileSize(fd);
  pData = malloc(filesize+1);
  if(pData!=NULL){
    if(sceLseek(fd, 0, SCE_SEEK_SET) < 0){
      printf("sceSeek fail(%s)\n",pfilename);
    }
    printf("File Read Start size=%d \n",filesize);
    if(sceRead(fd, pData, filesize) < 0){
      printf("sceRead fail(%s)\n",pfilename);
    }
    printf("File Read End\n");
    pData[filesize]='\0';
  }
  if(fd >= 0)
    sceClose(fd);
  return pData;
}
/* SkipComment
 * #以下をコメント行にする。
 * char *str 文字列
 * 戻り値 コメントの時改行したあとの文字列のポインタ,
 *        コメントでない時は引数を返す。
 */
void  SkipComment(char *str)
{
  while(*str !='\0'){
    if(*str == '#')
      while(*str != 0 && *str != '\n'){
	*str++=' ';
      }
    else
      str++;
   }
}
/* SetLanguage
 *  文字列から言語のindexを求める。
 * char *token  文字列へのポインタ
 */
int  SetLanguage(char *token){
  int i;
  for(i=0; i<MAX_LANGUAGE; i++){
    if(strcmp(token,Language[i].str)==0){
      break;
    }
  }
  if(i==MAX_LANGUAGE)
    printf("Can't set Language %s\n",token);
  return Language[i].index;
}
/* SetSpdif
 * 文字列からSPDIFのindexを求める。
 * char *token 文字列へのポインタ
 */
int SetSpdif(char *token){
  int i;
  for(i=0; i<MAX_SPDIF;i++){
    if(strcmp(token,Spdif[i].str)==0){
      break;
    }
  }
  if(i==MAX_SPDIF)
    printf("Can't set SPDIF %s\n",token);
  return Spdif[i].index;
}
/* SetAspect
 * 文字列からアスペクトのindexを求める。
 * char *token 文字列へのポインタ
 */
int SetAspect(char *token){
  int i;
  for(i=0; i<MAX_ASPECT;i++){
    if(strcmp(token,Aspect[i].str)==0){
      break;
    }
  }
  if(i==MAX_ASPECT)
    printf("Can't set ASPECT %s\n",token);
  return Aspect[i].index;
}
/* SetSummerTime
 * 文字列からサマータイムのindexを求める。
 * char *token 文字列へのポインタ
 */
int SetSummerTime(char *token){
  int i;
  for(i=0; i<MAX_SUMMERTIME;i++){
    if(strcmp(token,SummerTime[i].str)==0){
      break;
    }
  }
  if(i==MAX_SUMMERTIME)
    printf("Can't set SUMMERTIME %s\n",token);
  return SummerTime[i].index;
}
/* SetDateNotation
 * 文字列から日付の表示形式を求める。
 * char *token 文字列へのポインタ
 */
int SetDateNotation(char* token){
  int i;
  for(i=0; i<MAX_DATENOTATION; i++){
    if(strcmp(token,DateNotation[i].str)==0){
      break;
    }
  }
  if(i==MAX_DATENOTATION)
    printf("Can't set DATENOTATION %s\n",token);
  return DateNotation[i].index;
}
/* SetTimeNotation
 * 文字列から時刻表示形式のindexを求める。
 * char *token 文字列へのポインタ
 */
int SetTimeNotation(char* token){
  int i;
  for(i=0; i<MAX_TIMENOTATION; i++){
    if(strcmp(token,TimeNotation[i].str)==0){
      break;
    }
  }
  if(i==MAX_TIMENOTATION)
    printf("Can't set TIMENOTATION %s\n",token);
  return TimeNotation[i].index;
}

/* SetTimeZone
 * 文字列を数字に変換
 * char *token 文字列へのポインタ
 */
int SetTimeZone(char *token){
  return atoi(token);
}

/* SetConfig
 * DTL-T10000の設定
 * char *str DTL-T10000用の設定文字列
 */
void SetConfig(char *str)
{
  char sep[]="\n\t ";
  char *token ;
  
  sceScfT10kConfig config;
  int i,PreIndex=MAX_INDEX;
  SkipComment(str);  
  
    token = strtok(str,sep);  
  while(token != NULL){
    for(i=0; i<MAX_INDEX; i++){
      if(strcmp(token,Index[i].str)==0){
	break;
      }
    }

    switch(PreIndex){
    case INDEX_SPDIF:
      config.Spdif = SetSpdif(token);
      break;
    case INDEX_ASPECT:
      config.Aspect = SetAspect(token);
      break;
    case INDEX_LANGUAGE:
      config.Language = SetLanguage(token);
      break;
    case INDEX_TIMEZONE:
      config.TimeZone = SetTimeZone(token);
      break;
    case INDEX_SUMMERTIME:
      config.SummerTime = SetSummerTime(token);
      break;
    case INDEX_TIMENOTATION:
      config.TimeNotation = SetTimeNotation(token);
      break;
    case INDEX_DATENOTATION:
      config.DateNotation = SetDateNotation(token);
      break;
    default:
      
      if(PreIndex == MAX_INDEX && i == MAX_INDEX)
	printf("Phase error %s\n",token);
    } 
    PreIndex = i;
    token = strtok(NULL,sep);  
  }
  sceScfSetT10kConfig(&config);
}


void Format(char *str, sceCdCLOCK *prtc, int datenotation, int timenotation) 
{
  switch(datenotation){
  case SCE_DATE_YYYYMMDD:
    sprintf(str,"20%02x/%02x/%02x ",prtc->year, prtc->month, prtc->day);
    break;
  case SCE_DATE_MMDDYYYY:
    sprintf(str,"%02x/%02x/20%02x ",prtc->month, prtc->day, prtc->year);  
    break;
  case SCE_DATE_DDMMYYYY:
    sprintf(str,"%02x/%02x/20%02x ",prtc->day, prtc->month, prtc->year);
    break;
  }
  str += strlen(str);
  switch(timenotation){
  case SCE_TIME_24HOUR:
    sprintf(str,"%02x:%02x:%02x",prtc->hour, prtc->minute, prtc->second);
    break;
  case SCE_TIME_12HOUR:
    if(prtc->hour < 0x12)
      sprintf(str,"%02x:%02x:%02x AM",prtc->hour, prtc->minute, prtc->second);
    else{
      sprintf(str,"%02d:%02x:%02x PM",prtc->hour/16 * 10 + prtc->hour%16 - 12 , prtc->minute, prtc->second);
    }
    break;
  }
}

/* ConsoleDraw
 * オープンしたコンソールに表示する関数
 * int console コンソール識別子
 */
int ConsoleDraw(int console)
{
  sceCdCLOCK rtc;
  sceCdCLOCK rtcback;
  char str[30];

  if(sceCdReadClock(&rtc)==0){
    printf("Can't get clock\n");
    return 1;
  }
  if(rtc.stat!=0){
    printf("rtc.stat!=0\n");
      return 2;
  }
  memcpy(&rtcback, &rtc, sizeof(rtc));

  sceDevConsMessage(console, 2,1, "PlayStation 2 System Configuration Sample Program Ver1.0");

  sceDevConsLocate( console, 0, 4 );
  
  sceDevConsPrintf(console,"====== System Configuration ======\n\n");
  sceDevConsPrintf(console,"Language      = %s\n",Language[sceScfGetLanguage()].str); 
  sceDevConsPrintf(console,"Aspect        = %s\n",Aspect[sceScfGetAspect()].str);
  sceDevConsPrintf(console,"Spdif         = %s\n",Spdif[sceScfGetSpdif()].str);
  sceDevConsPrintf(console,"Summer Time   = %s\n",SummerTime[sceScfGetSummerTime()].str);
  sceDevConsPrintf(console,"Date Notation = %s\n",DateNotation[sceScfGetDateNotation()].str);
  sceDevConsPrintf(console,"Time Notation = %s\n",TimeNotation[sceScfGetTimeNotation()].str);
  sceDevConsPrintf(console,"Time Zone     = %d min\n\n",sceScfGetTimeZone());
  
  sceDevConsPrintf(console,"====== Clock ======\n\n");

  Format(str, &rtc, sceScfGetDateNotation(), sceScfGetTimeNotation()); 
  sceDevConsPrintf(console,"RTC(JST)   = %s\n\n", str);

  sceScfGetGMTfromRTC(&rtc);
  Format(str, &rtc, sceScfGetDateNotation(), sceScfGetTimeNotation()); 
  sceDevConsPrintf(console,"GMT(JST-9) = %s\n\n",str);
  
  memcpy(&rtc, &rtcback, sizeof(rtc));
     
  sceScfGetLocalTimefromRTC(&rtc);
  Format(str, &rtc, sceScfGetDateNotation(), sceScfGetTimeNotation()); 
  sceDevConsPrintf(console,"Local Time = %s\n\n",str);

  sceDevConsDraw( console );

  return 0;
}

int main(void)
{
  int frame=0;
  int console;
  char *str;
  
  sceSifInitRpc(0);

    
  if(sceCdInit(SCECdINIT)==0)
    printf("CD-ROM Initialization failed\n");

  
  sceDmaReset(1);
  sceGsResetPath();  

  
  if((str=MallocRead(SCF_FILE))!=NULL){
    SetConfig(str);
    free(str);
  }

#ifdef PAL
  sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_PAL, SCE_GS_FRAME);
#else
  sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
#endif

  sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		   SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

  sceDevConsInit();
  console = sceDevConsOpen(OFFX + (16<<5), OFFY + (16<<4), 70, 30);
  

  while(1)
  {
    ConsoleDraw(console);    
    if( frame & 1 )
    {	/* interrace half pixcel adjust */
      sceGsSetHalfOffset( &db.draw1, 2048, 2048, sceGsSyncV( 0 ) ^ 0x01 );
    } else {
      sceGsSetHalfOffset( &db.draw0, 2048, 2048, sceGsSyncV( 0 ) ^ 0x01 );
    }
    FlushCache( 0 );
    sceGsSyncPath( 0, 0 );
    sceGsSwapDBuff( &db, frame++ );
  }
  sceDevConsClose(console);
  sceCdInit(SCECdEXIT);
  sceSifExitCmd();
  return 0;
}
