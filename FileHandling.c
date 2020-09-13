#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


static void signalhandler(int _);
static int iCount = 0;
void handleExeImage(char *iSourceFile);
void handleText(char *iSourceFile);
void handleKeyboard();
void convertToBit(void *buffer, int length);

int main()
{
    char source_file[50];
    int choice, iFileType;

    FILE * fPtr;
    char *data, *temp;

    printf("Enter the type of input : 1 <Keyboard>, 2 <File>\n");

    /* Reading Input type */
   scanf("%d%*c",&choice);

   switch(choice)
    {
        case 1:
            /* Case to handle Keyboard input */
            handleKeyboard();
            break;
        case 2:
            /* Case to handle file input */
            printf("Enter the type of file : 1 <Text>, 2<exe>");
            scanf("%d",&iFileType);
            printf("Enter name of file to copy\n");
            scanf("%s",&source_file);
            if(iFileType == 1)
                handleText(source_file);
            else if(iFileType == 2)
                handleExeImage(source_file);
            else
                printf("Invalid choice. \n");
            break;
        default:
                            printf("Invalid choice. \n");
    }

    return 0;
}

static void signalhandler(int _)
{
   printf("Got control character. Considering it as the end of user input. Please Enter\n");
   iCount++;
   return;
}

void handleText(char *iSourceFile)
{
        FILE *source, *target;
        char ch;
        target = fopen("Output.txt", "w");
        if(target == NULL)
        {
           /* File not created hence exit */
           printf("Unable to create file.\n");
           exit(EXIT_FAILURE);
        }

        source = fopen(iSourceFile, "r");
        if( source == NULL )
        {
                printf("Unable to open source file.\n");
                exit(EXIT_FAILURE);
        }

        //Copying char by char from source to target
        while( ( ch = fgetc(source) ) != EOF )
                fputc(ch,target );

        printf("File copied successfully.\n");

        fclose(source);
        fclose(target);
}

void handleExeImage(char *iSourceFile)
{
        char *buffer;
        int fileLen;
        FILE *file = fopen(iSourceFile, "rb");

        fseek(file, 0, SEEK_END);
        fileLen=ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer=(char *)malloc(fileLen+1);
        fread(buffer, fileLen, 1, file);
        fclose(file);
        convertToBit(&buffer, fileLen);
        free(buffer);
}

// convert buffer data to bits and write them to a text file
void convertToBit(void *buffer, int length)
{
        int c=0;
        int SIZE = length * 8;
        unsigned char bits[SIZE + 1];
        unsigned char mask = 1;
        unsigned char byte ;
        int i = 0;
        FILE *bitWRT = fopen("Output.txt", "w");

        for (c=0;c<length;c++)
        {
           byte = ((char *)&buffer)[c];

           for(i = 0; i < 8; i++)
           {
                 bits[i] = (byte >> i) & mask;
                 /* Writing binary data into Output file */
                 fprintf(bitWRT, "%d\n", bits[i]);
           }
        }
        fclose(bitWRT);
}

void handleKeyboard()
{
        FILE * fPtr;
        char *data, *temp;

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
           scanf ("%m[^\n]%*c", &data);
           if( data )
           {
              temp = (char *)malloc(strlen(data));
              strcpy(temp,data);

              printf("%s\n",temp);

              /* Writing data into file */
              fputs(temp, fPtr);

              /* Adding new line char at the end of each line */
              fputc('\n', fPtr);

              /* Freeing the allocated memory */
              free(temp);
           }
        }
        /* Close file to save file data */
        fclose(fPtr);
        printf("File created and saved successfully. \n");
}
