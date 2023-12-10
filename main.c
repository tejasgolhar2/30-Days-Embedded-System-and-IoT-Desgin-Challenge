/*
	Application Name: Online test
	Description: This application is developed for giving exams based on multiple-choice questions.
	Requirements: This application requires a text document that would contain the questions & answers as well. The format of the questions is to be followed strictly.

*/

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define LOWER_CASE ('a' ^ 'A')

#define TRUE 1U
#define FALSE 0U

// Give the Absolute Path of the file along with the file-name
#define QUE_FILENAME "questions.txt"

// Define file read and write modes
#define FILE_OPEN_MODE_READ "r"
#define FILE_OPEN_MODE_WRITE "w"

// Define a Character to determine the end of the file
#define QUESTION_END_CHAR '$'

// Time allocated for single question in minutes
#define TIME_PER_QUE 2U

// Store the User Specific Answer Filename
static char cAnswerFileName[32U];

static unsigned char getNumberOfQuestions(void);
static void askUserName(void);
static isAnswerValid(int);
static unsigned char timeElapsedMessage(unsigned long ulEndTime);
static void processExam(unsigned char queLeft, FILE *qFilePtr, FILE *ansFilePtr);


int main(void)
{
	unsigned char ucNumOfQuestions = 0;
	FILE *fQueFilePtr = NULL;
	FILE *fAnsFilePtr = NULL;
	

	// Purge data at input buffers
	fflush(stdin);
	

	printf("*******************Online Exam*********************\n");
	
	// COunt total numbe of Questions
	ucNumOfQuestions = getNumberOfQuestions();

	askUserName();

	// Open the User Answer File to store the answers of User
	fAnsFilePtr = fopen(cAnswerFileName,FILE_OPEN_MODE_WRITE);
	if(fAnsFilePtr == NULL)
	{
		printf("Failure to Open the Answer File\n");
		return 0;
	}

	// Start the Exam
	// Open the Question file in Read Mode -- The below gives address of the file
	fQueFilePtr = fopen(QUE_FILENAME, FILE_OPEN_MODE_READ);

	if( fQueFilePtr != NULL )
	{
		
		processExam(ucNumOfQuestions,fQueFilePtr,fAnsFilePtr);
		
		// Close the Question File before exiting the Application
		fclose(fQueFilePtr);

	}
	else
	{
		printf("Failed to Open File\n");
	}

	//fprintf(fAnsFilePtr,"\nMarks Obtained are: %d/%d",ucMarks,ucNumOfQuestions);
	// Close the Answer File Pointer before Submitting the Quiz
	fclose(fAnsFilePtr);
	return 0;
}

static void processExam(unsigned char queLeft, FILE *qFilePtr, FILE *ansFilePtr)
{
	unsigned char ucMarks = 0;
	unsigned char quesCount = 1;
	char cFileChar = 0;
	char answer = 0;

	// Initialize the timers
	unsigned long ulCurrTime = 0;
	unsigned long ulStartTime = 0;
	unsigned long ulEndTime = 0;

	// Store Current Time COunt
	ulStartTime = ulCurrTime = time(NULL);

	// Calculate the maximum time allowed
	ulEndTime = ulStartTime + ( 60 * TIME_PER_QUE * queLeft );


	// Look Until All Questions are Completed and EOF is reached 

	while((cFileChar = getc(qFilePtr)) != EOF )
	{
		char answer = 0;

		
		// When the character matches the End Question Character

		if( QUESTION_END_CHAR == cFileChar )
		{
			queLeft--;

			// Read the Current time
			if (timeElapsedMessage(ulEndTime)==TRUE) break;

			// Detect the key hit on the keypad by User for answer value
			while(1)
			{
				//Purge the data at input buffers
				fflush(stdin);

				if (kbhit())
				{
					// Get User Input for Answer Options
					answer = getch();
					printf("%c",answer);
					if(isAnswerValid(answer) == TRUE )
					{
						printf("\n\tRE-ENTER ANSWER : ");
						//continue;
					}

					// Store the answer received from the user
					fprintf(ansFilePtr,"(%d)\t%c", quesCount++, answer);
					break;
				}
			}

			if (timeElapsedMessage(ulEndTime)==TRUE) break;

			// Check the correctness of the answer
			cFileChar = getc(qFilePtr);

			if ( (answer | LOWER_CASE)==(cFileChar | LOWER_CASE))
			{
				++ucMarks;
				fprintf(ansFilePtr,"\tRight\n");
			}
			else fprintf(ansFilePtr,"\tWrong\n");

			if(queLeft)
			{
				ulCurrTime = time(NULL);
				printf("\n\n\t********************************\n");
				printf("\tQuestions Remained : %d\n",queLeft);
				printf("\tTime Remained : %d minute ",( ulEndTime - ulCurrTime )/60);
				printf("%d second\n",(ulEndTime - ulCurrTime )%60);
				printf("\t**********************************\n");
			}
		}
		else
		{
			// IF not Question End, Print each character one by one
			printf("%c",cFileChar);
		}	
	}
}
		
static unsigned char timeElapsedMessage(unsigned long ulEndTime)
{
	unsigned long ulCurrTime = 0;

	//read current time
	ulCurrTime = time(NULL);

	// Compare it with the maximum time allocated for the exam
	if(ulCurrTime>=ulEndTime)
	{
		printf("\n\n\t**********************************\n");
		printf("\nTime Elapsed\n");
		printf("\t***********************************\n");
		return TRUE;
	}
	return FALSE;
}

static unsigned char getNumberOfQuestions(void)
{
	FILE *fQueFilePtr = NULL;		// Stores address of file
	unsigned char ucNumOfQuestions = 0;		// Initialize number of Questions
	char cFileChar=0;


	//Open the file in Read Mode -- The below gives address of the file
	fQueFilePtr = fopen(QUE_FILENAME, FILE_OPEN_MODE_READ);

	if( fQueFilePtr != NULL )
	{
		// Loop through the file until its end is reached
		while((cFileChar = getc(fQueFilePtr)) != EOF )
		{
			// Checking for every question present in the file
			if(cFileChar == QUESTION_END_CHAR)
			{
				++ucNumOfQuestions;
			}
		}

		// Close the file 
		fclose(fQueFilePtr);

		// User Display Message
		printf("********************************\n");
		printf("Number of Questions: %d\n", ucNumOfQuestions);
		printf("Maximum Time Allocated: %d\n", TIME_PER_QUE * (ucNumOfQuestions));
		printf("********************************\n");

	}
	else
	{
		printf("Failed to Open File\n");
	}

	return ucNumOfQuestions;
}

static void askUserName(void)
{
	char cUserName[32];

	// Creting a file for storing answers from user
 	char *sAnswerFile = "_answer.txt";
	
	printf("Enter your name: ");
	gets(cUserName);

	strcat(cAnswerFileName,cUserName);
	strcat(cAnswerFileName,sAnswerFile);
}

static isAnswerValid(int answer)
{
	return ((answer | LOWER_CASE) < 'a' || (answer | LOWER_CASE) > 'e');
}