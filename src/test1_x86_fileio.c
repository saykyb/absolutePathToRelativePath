/*
 ============================================================================
 Name        : test1_x86_fileio.c
 Author      : kyb
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_STR_BUF 512
FILE *fp;
FILE *fp2;
char *strTmp1 = "./tmp1.txt";
char *strTmp2 = "./tmp2.txt";
char *strOutput = "./output.sh";

//char strExistingBasePath[MAX_STR_BUF] = "/home/kyb/workspace/test1_x86_fileio";
char strExistingBasePath[MAX_STR_BUF] = "/home2/yunbae.kim/work/fsl-community-bsp";

char str1[MAX_STR_BUF];
char str2[MAX_STR_BUF];
char str1_2[MAX_STR_BUF];
char strTmp[MAX_STR_BUF];
char buf[MAX_STR_BUF];

int main(void) {
/*
	1. 실행할 ln 명령어 형식으로 리스트 만들기
		find . -type l -printf "kyb_new%l %p\n"|grep "\/home2">tmp1.txt
	2. 절대경로를 상대경로로 문자열 변경
		sed -n 's/kyb_new\/home2\/yunbae.kim\/temp\/aaa/./gpw tmp2.txt' tmp1.txt
	3. set path
	4. make output.sh file
*/
	int lineCount;
	char strGet[MAX_STR_BUF];
	char strCmt[]="Search command : find . -type l -printf '%l %p\\n'|grep '\/home'>tmp1.txt";
	printf("\n%s\n",strCmt);
	while(1){
		memset(strGet,0,MAX_STR_BUF);
		printf("\n링크된 심볼릭 링크 base 경로(default) : %s\n",strExistingBasePath);
		//printf("find . -type l -printf '%\l \'%\'p\\n'|grep '\/home' 으로 검색 가능");
		printf("계속 진행하시겠습니까? [y/경로/q] : ",strExistingBasePath);
		gets(strGet);

		if(!strcmp(strGet,"y")){

			break;
		} else if(!strcmp(strGet,"q")){

			return 1;
		} else if(strGet[0]==0){
			continue;
		} else {
			memset(strExistingBasePath,0,MAX_STR_BUF);
			strcpy(strExistingBasePath,strGet);
			continue;
		}
	}
	// 1.
	system("find . -type l -printf '%l %p\n'|grep '\/home'>tmp1.txt");

	printf("output tmp1.txt\n");
	//=========================================================================
	// 2.
	fp=fopen(strTmp1,"r");
	if(!fp){
		printf("Error io 1~!\n");
		return 1;
	}
	fp2=fopen(strTmp2,"wt");
	if(!fp2){
		printf("Error io 2 ~!\n");
		return 1;
	}

	lineCount=0;
	while(!feof(fp)){
		int cnt=0;
		int i,strPathLen;
		memset(str1,0,MAX_STR_BUF);
		memset(str1_2,0,MAX_STR_BUF);
		memset(str2,0,MAX_STR_BUF);
		memset(strTmp,0,MAX_STR_BUF);

		//read
		fscanf(fp,"%s %s\n",str1,str2);

		// working
		strPathLen = strlen(strExistingBasePath);
		if(strncmp(str1,strExistingBasePath,strPathLen)){
			printf("skip line:%s, %s\n",str1,str2);
			//printf("check path ~!! input path:%s\n",strExistingBasePath);
			//return 1;
			continue;
		}

		strncpy(str1_2,&str1[strPathLen+1],MAX_STR_BUF-strPathLen);

		//write
		snprintf(buf,MAX_STR_BUF,"%s %s\n",str1_2,str2);
		fputs(buf,fp2);
		lineCount++;
	}
	printf("output tmp2.txt(total line:%d)\n", lineCount);
	fclose(fp);
	fclose(fp2);
	//=========================================================
	// 3. and 4.
	fp=fopen(strTmp2,"r");
	if(!fp){
		printf("Error io 3~!\n");
		return 1;
	}
	fp2=fopen(strOutput,"wt");
	if(!fp2){
		printf("Error io 4 ~!\n");
		return 1;
	}

	while(!feof(fp)){
		int cnt=0;
		int i;
		memset(str1,0,MAX_STR_BUF);
		memset(str1_2,0,MAX_STR_BUF);
		memset(str2,0,MAX_STR_BUF);
		memset(strTmp,0,MAX_STR_BUF);

		//read
        fscanf(fp,"%s %s\n",str1,str2);

        // working
        for(i=0;i<MAX_STR_BUF;i++){
        	if(str2[i]=='/'){
        		cnt++;
        	}else if(str2[i]=='\0'){
        		break;
        	}
        }
        cnt = cnt-1;
        if(cnt>0){
        	for(i=0;i<cnt;i++){
        		snprintf(&strTmp[i*3],MAX_STR_BUF,"../");
        	}
        	snprintf(str1_2,MAX_STR_BUF,"%s%s",strTmp,str1);
        } else {
        	snprintf(str1_2,MAX_STR_BUF,"%s",str1);
        }

        //write
		snprintf(buf,MAX_STR_BUF,"ln -Tfs %s %s\n",str1_2,str2);
		fputs(buf,fp2);
	}
	printf("output file : output.sh\n");
	printf("Done~!\n");
	fclose(fp);
	fclose(fp2);
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
