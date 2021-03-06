//생각해보자
//해당 코드에서 함수로 문자열 분할 안 할 경우의 문제점은??




// 4 translate ENG_INTO_KOR.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

#include <string.h>

#include "STRING_PROCESS.h"


//버퍼 관련
typedef enum BUF_SETTING
{
	//입력 버퍼 크기
	INPUT_BUF_SZ = 128,

}BUF_SETTING;


typedef enum TRANS_OPTION
{
	//#define INPUT_BUF_SZ 128	//총 입력 문자열 버퍼 크기
	AVAILABLE_TRANS_WORD_CNT = 2,	//총 번역 가능 단어 갯수

									//영어 문자열 최대 길이
									MAX_ENG_STRING_LENGTH = (INPUT_BUF_SZ - 1),

									//한글 문자열 최대 길이
									MAX_KOR_STRING_LENGTH = ((INPUT_BUF_SZ / 2) - 1),

									//영어 단어 최대 길이
									MAX_ENG_WORD_LENGTH = 30 - 1,

									//한글 단어 최대 문자열 길이
									MAX_KOR_WORD_LENGTH = (MAX_ENG_WORD_LENGTH / 2)



}TRANS_OPTION;


//번역 방식
typedef enum TRANS_OPERATION
{
	KOR_INTO_ENG,
	ENG_INTO_KOR

}TRANS_OPERATION;


int main()
{

	/*------------------------
	통계 정보
	------------------------*/
	//총 입력된 단어 수
	int totalInputWordCnt = 0;

	//번역 실패한 단어 수 
	int failTransCnt = 0;

	//현재 번역 가능한 단어의 개수
	int nowAvailableCnt = AVAILABLE_TRANS_WORD_CNT;




	/*------------------------
	단어 사전 배열
	------------------------*/
	//번역 단어 등록
	//	-나중에!! 텍스트 파일에서 읽어서 동적할당 한다
	//	-텍스트 파일 형식 
	//		:한글 (공백) 영어
	//		:단어 읽을 때 maxKOR_LENGTH와 maxEng_LENGTH 체크 필요
	char dicArr[AVAILABLE_TRANS_WORD_CNT][2][MAX_KOR_WORD_LENGTH * 2] =
	{
		{ "boy", "소년" },
	{ "girl", "여자" },
	};
	//사용 예
	//	puts(dicArr[1][1]);  -->여자 출력



	/*------------------------
	번역 방식 설정 변수
	------------------------*/
	TRANS_OPERATION transOperation = KOR_INTO_ENG;
	char tmp = 0;


	/*------------------------
	메뉴 출력1

	-번역 할 대상 언어 선택
	------------------------*/
	while (true)
	{
		puts("[메뉴]번역 대상 선택");
		puts("	(1)영어");
		puts("	(2)한글");

		printf("[입력] <<");
		scanf_s("%c", &tmp);

		//영어 -> 한글
		if (tmp == '1')
		{
			transOperation = ENG_INTO_KOR;
			break;
		}

		//한글 -> 영어
		else if (tmp == '2')
		{
			transOperation = KOR_INTO_ENG;
			break;
		}

		//버퍼 비우기
		//	개행 문자 제거
		scanf_s("%c", &tmp);

		//엔터 출력
		puts("");
	}

	//버퍼 비우기
	//	개행 문자 제거
	scanf_s("%c", &tmp);



	/*------------------------
	입력 버퍼 설정

	-키보드로 입력받는 공간
	------------------------*/
	//입력 받는 문자열
	//	번역 대상 문자열
	char inputString[INPUT_BUF_SZ] = { '\0' };



	//gets_s 주의점
	//	입력 받을 문자열 길이를 미리 알 수 없다
	//	에러 시 프로그램에게 에러 띄움
	//결론 윈도우즈 입력을 이용해야 한다



	/*------------------------
	번역 할 문자열 입력
	문자열 입력시 주의사항 출력

	-입력 문자열이 영어인 경우와 한글인 경우에 따라 주의사항 출력이 다르다
	-입력 문자열이 영어인 경우와 한글인 경우에 따라 문자의 크기가 다르므로 입력 배열 총 크기에 주의
	------------------------*/
	//영어인 경우
	if (transOperation == KOR_INTO_ENG)
	{
		printf_s("[주의1]문장 내 모든 영어 문장 길이는 %d 자리 이하로 입력하세요\n", MAX_KOR_WORD_LENGTH);
		printf_s("[주의2]%d자 이하의 한글을 입력해주세요(끝 : 엔터)\n:", INPUT_BUF_SZ / 2);

		//번역할 문장 입력
		//	바로 엔터치면 0인덱스 요소부터 NULL
		gets_s(inputString, INPUT_BUF_SZ);

	}

	//한글인 경우
	//	-한글은 한글자당 2바이트 필요
	if (transOperation == ENG_INTO_KOR)
	{
		printf_s("[주의1]문장 내 모든 한글단어의 길이는 %d 자리 이하로 입력하세요\n", MAX_ENG_WORD_LENGTH);
		printf_s("[주의2]%d자 이하의 영어를 입력해주세요(끝 : 엔터)\n:", INPUT_BUF_SZ);


		//번역할 문장 입력
		//	바로 엔터치면 0인덱스 요소부터 NULL
		gets_s(inputString, INPUT_BUF_SZ / 2);
	}


	//의문
	//	버퍼 용량 초과하는 경우는 어떻게 처리하는가?
	//	예외 처리 필요


	/*------------------------
	입력 받은 문자열 길이 구하기
	------------------------*/
	int inputLength = 0;
	inputLength = strlen(inputString);



	/*------------------------
	문자열 분할에 이용하는 변수 및 플래그
	------------------------*/
	//분할 된 단어
	//	입력받은 문자열 중 단어 단위로 저장 할 때 쓰는 변수
	char wordSplit[MAX_ENG_WORD_LENGTH] = { '\0' };


	//분할 된 단어 길이
	int wordCnt = 0;


	//번역된 최종 문자열
	char resultString[INPUT_BUF_SZ] = { '\0' };


	//단어 별 번역 단어검색 플래그
	//	성공시 true
	//	실패시 false
	bool flagSuccessSearch = false;


	//모든 단어 번역 성공 여부
	//	성공시 true
	//  실패시 false
	bool flagSuccessAllTrans = true;


	//번역 중간 과정 문자열
	char midString[MAX_KOR_WORD_LENGTH] = { '\0' };



	//번역 단어 배열 인덱스
	int dicCheck = 0;


	//환경 설정 출력
	printf_s("\n[옵션] <<자동으로 소문자 변환이 이루어 집니다>>\n");

	//참고
	//scanf로 %s로 입력 받을 시 문자열 형식으로 입력받아서 마지막에 널문자가 삽입된다


	//번역
	//	입력된 문자열 길이만큼
	//  inputLength +1은 널 혹은 마지막 공백
	for (int idx = 0; idx < inputLength + 1; idx++)
	{

		//단어로 분할 되는 경우
		//	널, 공백
		if (inputString[idx] == ' ' || inputString[idx] == '\0' || inputString[idx] == '\n')
		{
			//예외 처리
			//	공백으로만 이루어진 문자인 경우
			if (wordCnt == 0)
				continue;


			//문자열 형식으로 변경
			wordSplit[wordCnt] = '\0';


			//번역 할 단어 검색
			for (dicCheck = 0; dicCheck < AVAILABLE_TRANS_WORD_CNT; dicCheck++)
			{
				//영어 번역
				if (transOperation == ENG_INTO_KOR)
				{//번역 할 단어를 찾은 경우
					if (strcmp(dicArr[dicCheck][0], wordSplit) == 0)
					{
						flagSuccessSearch = true;
						break;
					}
				}

				//한글 번역
				if (transOperation == KOR_INTO_ENG)
				{
					{//번역 할 단어를 찾은 경우
						if (strcmp(dicArr[dicCheck][1], wordSplit) == 0)
						{
							flagSuccessSearch = true;
							break;
						}
					}
				}
			}



			//번역 할 단어를 찾은 경우
			if (flagSuccessSearch == true)
			{

				//영단어인 경우
				//번역 단어의 길이 구하기
				int lenTransWord = 0;
				int selectLeng = -1;

				//영어 번역
				if (transOperation == ENG_INTO_KOR)
				{
					selectLeng = 1;
				}

				//한글 번역
				if (transOperation == KOR_INTO_ENG)
				{
					selectLeng = 0;
				}

				lenTransWord = strlen(dicArr[dicCheck][selectLeng]);



				//번역 단어를 중간 과정에 필요한 배열에 저장
				//	 길이 +1은 널문자(문자열 길이에 NULL 갯수까지 포함 안된다)
				strcpy_s(midString, lenTransWord + 1, dicArr[dicCheck][selectLeng]);


				//마지막 단어 끝 글자 문자열 처리
				midString[strlen(midString)] = ' ';
				midString[strlen(midString) + 1] = '\0';



				//번역 문자열에 번역한 단어를 붙인다
				//	마지막에 붙는다
				strcat_s(resultString, midString);


			}

			//번역 할 단어를 못찾은 경우
			else if (flagSuccessSearch == false)
			{
				//마지막 단어 끝 글자 문자열 처리
				wordSplit[wordCnt] = ' ';
				wordSplit[wordCnt + 1] = '\0';


				//번역 문자열에 등록
				strcat_s(resultString, wordSplit);


				//번역 실패한 단어 수 +1
				failTransCnt++;


				//모든 단어 번역 여부 플래그
				flagSuccessAllTrans = false;
			}


			//초기화
			//	번역 단어 배열 체크 
			//	읽은 단어의 인덱스
			//  대상 문자열에서 단어 추출 플래그
			wordCnt = 0;
			dicCheck = 0;
			flagSuccessSearch = false;


			//입력 받은 단어 총 갯수
			totalInputWordCnt++;

		}// if



		 //단어로 분할 되지 않는 경우
		 //	지속적으로 이어진 문자를 읽는 경우
		else
		{

			//단어에 읽은 문자 추가
			wordSplit[wordCnt] = inputString[idx];


			//알파벳 여부 검사
			if (IsAlphabet(wordSplit[wordCnt]))
			{
				//알파벳이 대문자인 경우 소문자로 변경
				ToLowerAlphabet(&wordSplit[wordCnt]);
			}

			//한글인 경우
			else
			{
				//해당 없다.
			}



			wordCnt++;

		}



	}//for



	 //출력 배열을 문자열 배열화
	 //번역 된 문자 배열의 마지막 인덱스에 null 부착
	resultString[strlen(resultString) - 1] = '\0';



	//모든 단어 번역 여부
	//	번역 통계 출력
	if (flagSuccessAllTrans == true)
	{
		if (totalInputWordCnt == 0)
			printf("[번역 정보] <<단어가 입력되지 않았습니다>>");
		else
			printf("[번역 정보] <<모든 단어(%d개) 번역 성공>>\n\n#", totalInputWordCnt);
	}
	else
		printf("[번역 정보] <<%d개의 단어 중 %d 개의 단어가 번역에 실패하였습니다>>\n\n[번역]#", totalInputWordCnt, failTransCnt);



	//최종 번역 문자열 출력
	puts(resultString);


	printf("\n\n<<번역 전 총 문자열 바이트: %d>>\n", strlen(inputString));
	printf("<<번역 후 총 문자열 바이트: %d>>\n", strlen(resultString));
	printf("<<총 버퍼 공간: %d>>\n", INPUT_BUF_SZ);
	printf("<<남은 버퍼 공간: %d>>\n", INPUT_BUF_SZ - strlen(resultString));

	while (true);


	return 0;


}

