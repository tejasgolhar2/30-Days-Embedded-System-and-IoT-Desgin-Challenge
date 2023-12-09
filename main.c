/*
	Application Name: Online test
	Description: This application is developed for giving exams based on multiple-choice questions.
	Requirements: This application requires a text document that would contain the questions & answers as well. The format of the questions is to be followed strictly.

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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

int main(void)
{
	unsigned char ucNumOfQuestions = 0;
	FILE *fQueFilePtr = NULL;
	FILE *fAnsFilePtr = NULL;
	unsigned char ucMarks = 0;

	// Purge data at input buffers
	fflush(stdin);
	

	printf("*******************Online Exam*********************\n");
	
	// COunt total numbe of Questions
	ucNumOfQuestions = getNumberOfQuestions();

	askUserName();

	// Open the User Answer File to store the answers of User
	fAnsFilePtr = fopen(cAnswerFileName,FILE_OPEN_MODE_WRITE);
	if(NULL==fAnsFilePtr)
	{
		printf("Failure to Open the Answer File\n");
		return 0;
	}

	// Start the Exam
	// Open the Question file in Read Mode -- The below gives address of the file
	fQueFilePtr = fopen(QUE_FILENAME, FILE_OPEN_MODE_READ);

	if( NULL != fQueFilePtr )
	{
		
		unsigned char ucNumOfQueLeft = 0;
		char cFileChar = 0;
		ucNumOfQueLeft = ucNumOfQuestions;

		while((cFileChar = getc(fQueFilePtr)) != EOF )
		{
			printf("%c",cFileChar);
		}
		
		// Look Until All Questions are Completed and EOF is reached 

		while(ucNumOfQueLeft && ((cFileChar = getc(fQueFilePtr)) != EOF ))
		{
			char answer = 0;
			
			// When the character matches the End Question Character

			if( QUESTION_END_CHAR == cFileChar )
			{
				ucNumOfQueLeft--;

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
						if(TRUE == isAnswerValid(answer))
						{
							prinf("\n\tRE-ENTER ANSWER : ");
							continue;
						}

						// Store the answer received from the user
						fprintf(fAnsFilePtr,"(%d)\t%c", ucNumOfQuestions - ucNumOfQueLeft, answer);
						break;
					}

					//printf("Number of Questions Left: %d\n", ucNumOfQueLeft);
				}

				cFileChar = getc(fQueFilePtr);

				if ( (answer | LOWER_CASE)==(cFileChar | LOWER_CASE))
				{
					++ucMarks;
					fprintf(fAnsFilePtr,"\tRight\n");
				}
				else fprintf(fAnsFilePtr,"\tWrong\n");
			}
			else
			{
				// IF not Question End, Print each character one by one
				printf("%c",cFileChar);
			}	
		}

		// Close the Question File before exiting the Application
		fclose(fQueFilePtr);

	}
	else
	{
		printf("Failed to Open File\n");
	}

	fprintf(fAnsFilePtr,"\nMarks Obtained are: %d/%d",ucMarks,ucNumOfQuestions);
	// Close the Answer File Pointer before Submitting the Quiz
	fclose(fAnsFilePtr);
	return 0;
}

static unsigned char getNumberOfQuestions(void)
{
	FILE *fQueFilePtr = NULL;		// Stores address of file
	unsigned char ucNumOfQuestions = 0;		// Initialize number of Questions
	char cFileChar=0;


	//Open the file in Read Mode -- The below gives address of the file
	fQueFilePtr = fopen(QUE_FILENAME, FILE_OPEN_MODE_READ);

	if( NULL != fQueFilePtr )
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
	// Creting a file for storing answers from user
	char *sAnswerFile = "_answer.txt";
	printf("Enter your name: ")
	gets(cUserName);

	strcat(cAnswerFileName,cUserName);
	strcat(cAnswerFileName,sAnswerFile);
}

static isAnswerValid(int answer)
{
	return ((answer | LOWER_CASE) < 'a' || (answer | LOWER_CASE) > 'e');
}