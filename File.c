#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static void signalhandler(int _);
static int iCount = 0;

int main()
{
    /* Variable to store user data */
    char *cData, *cTemp;

    /* File pointer to hold reference to Output.txt file */
    FILE * fPtr;

    /* Open Output.txt file in write mode */
    fPtr = fopen("Output.txt", "w");

    /* Condition check for validity of File pointer */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    /* Input contents from user to store in file */
    printf("Enter contents to store in file. Enter Ctrl+C at the end of input data : \n");

    /* Signal handling to handle ctrl+c which indicates the end of input data*/
    signal(SIGINT, signalhandler);
    while (1)
    {
    /* Static variable to find if control char hits to stop getting the user input */
      if(iCount > 0)
        break;

      /* Reading User input */
      scanf ("%m[^\n]%*c", &cData);
      if( cData )
      {
        cTemp = (char *)malloc(strlen(cData));
        strcpy(cTemp,cData);

        /* Writing data into file */
        fputs(cTemp, fPtr);

        /* Adding new line char at the end of each line */
        fputc('\n', fPtr);

        /* Freeing the allocated memory */
        free(cTemp);
      }
    }
    /* Close file to save file data */
    fclose(fPtr);

    /* Success message */
    printf("File created and saved successfully. \n");

    return 0;
}

static void signalhandler(int _)
{
   printf("Got control character. Considering it as the end of user input. Please Enter\n");
   iCount++;
   return;
}
