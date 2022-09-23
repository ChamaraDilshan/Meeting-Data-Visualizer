/*
* PROJECT 02: MEETING DATA VISUALIZER
* Author : SENEVIRATHNA M.D.C.D
* REG. NO : E/18/327
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Defining variables
#define byMeetings 1
#define byParticipants 2
#define byDuration 3
#define ON 1
#define OFF 0
#define DEFAULT 10

//defining new data type
typedef struct _ {
    char name[25];
    int participants;
    int hour;
    int min;
    int sec;
    int meetings;
    int total_p;
    int duration;

}meeting_data;

//Function prototype
void read_file(char *);
int count_data(meeting_data *, int);
void sortArray(int ,meeting_data *,int );
void visualize_data(meeting_data *, int , int ,int ,int );
int bar_size(meeting_data *, int , int , int ,int , int );
void print_name(meeting_data *, int ,int );
int sum_data(meeting_data *, int ,int );
int long_name(meeting_data *, int );
void chart(int mode,int length,int scale);
int isInt(char *);
int check_CSV(char *);


//global variables
meeting_data arrayMeeting[1000000], newArray[1000000], sorted_data[1000000];
int len = 0;

int main(int argc,char **argv)
{
    //Declaring the variables
    int i, arg_m, arg_p, arg_t, arg_scaled, arg_l, length = -1, no_file = 1, err = -1;
    char *argument[] = {"-m","-p","-t","--scaled","-l"};

    //checking the arguments
    for(i = 1; i < argc; i++)
    {
        char *str1 = argv[i];

        if( strcmp(argv[i],argument[0]) == 0 )
        {
            arg_m = 1;
        }
        if( strcmp(argv[i],argument[1]) == 0 )
        {
            arg_p = 1;
        }
        if( strcmp(argv[i],argument[2]) == 0 )
        {
            arg_t = 1;
        }
        if( strcmp(argv[i],argument[3]) == 0 )
        {
            arg_scaled = 1;
        }
        if( (strcmp(argv[i],argument[4]) == 0) && ( isInt(argv[i+1]) != -90909090 ) )
        {
            if( isInt(argv[i+1]) < 0 )
            {
                printf("Invalid option(negative) for [-l]\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
                exit(1);
            }
            arg_l = 1;
            length = isInt(argv[i+1]);
        }
        else if( (strcmp(argv[i],argument[4]) == 0) && (isInt(argv[i+1]) == -90909090) )
        {
            printf("Invalid option for [%s]\n", argv[i]);
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
            exit(1);
        }
        if( check_CSV(argv[i]) == 1 )
        {
            read_file(argv[i]);
            no_file = 0;
        }
        else if( (check_CSV(argv[i]) != 1) && (str1[0] != '-') && (length < 0) ) //if non CSV file was given as input
        {
            printf("Only .csv files should be given as inputs.\n");
            exit(1);
        }

        //Error Handling
        if( ((strcmp(str1,argument[0]) != 0) && (strcmp(str1,argument[1]) != 0)  && (strcmp(str1,argument[2]) != 0) && (strcmp(str1,argument[3]) != 0) && (strcmp(argv[i],argument[4]) != 0) && (str1[0] == '-') ) /*|| ((strcmp(argv[i],argument[4]) == 0) && (length < 0) )*/ )
        {
            err = i;
        }
        else if( strcmp(argv[argc - 1],argument[4]) == 0 )
        {
            printf("Not enough options for [-l]\n");
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
            exit(1);
        }

        if( (err != -1) && (length != 0) ) //if there is wrong argument
        {
            printf("Invalid option [%s]\n", argv[err]);
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
            exit(1);
        }
    }

    if( no_file == 1 ) //if there is no file input
    {
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
        exit(1);
    }

    //Printing the graph according to given arguments
    if( (arg_m == 1) && (arg_p != 1) && (arg_t != 1) ) //for meeting
    {
        if((arg_l == 1) && (length >= 0) && (arg_scaled == 1))
        {
            chart(byMeetings,length,ON);
        }
        else if( (arg_l != 1) && (arg_scaled == 1) )
        {
            chart(byMeetings,DEFAULT,ON);
        }
        else if( (arg_l == 1) && (length >= 0) && (arg_scaled != 1) )
        {
            chart(byMeetings,length,OFF);
        }
        else
        {
            chart(byMeetings,DEFAULT,OFF);
        }
    }

    else if( (arg_m != 1) && (arg_p == 1) && (arg_t != 1) ) //for participants
    {
        if((arg_l == 1) && (length >= 0) && (arg_scaled == 1))
        {
            chart(byParticipants,length,ON);
        }
        else if( (arg_l != 1) && (arg_scaled == 1) )
        {
            chart(byParticipants,DEFAULT,ON);
        }
        else if( (arg_l == 1) && (length >= 0) && (arg_scaled != 1) )
        {
            chart(byParticipants,length,OFF);
        }
        else
        {
            chart(byParticipants,DEFAULT,OFF);
        }
    }

    else if( (arg_m != 1) && (arg_p != 1) && (arg_t == 1) ) //for duration
    {
        if((arg_l == 1) && (length >= 0) && (arg_scaled == 1))
        {
            chart(byDuration,length,ON);
        }
        else if( (arg_l != 1) && (arg_scaled == 1) )
        {
            chart(byDuration,DEFAULT,ON);
        }
        else if( (arg_l == 1) && (length >= 0) && (arg_scaled != 1) )
        {
            chart(byDuration,length,OFF);
        }
        else
        {
            chart(byDuration,DEFAULT,OFF);
        }
    }

    else if( (arg_m != 1) && (arg_p != 1) && (arg_t != 1) ) //DEFAULT OPTIONS
    {
        if( (arg_scaled == 1) && (arg_l != 1) )
        {
            chart(byMeetings,DEFAULT,ON);
        }
        else if( (arg_l == 1) && (length >= 0) && (arg_scaled == 1) )
        {
            chart(byMeetings,length,ON);
        }
        else if( (arg_l == 1) && (length >= 0) && (arg_scaled != 1) )
        {
            chart(byMeetings,length,OFF);
        }
        else
        {
            chart(byMeetings,DEFAULT,OFF);
        }
    }

    else //if multiple parameters are given
    {
        printf("Cannot plot multiple parameters in same graph.\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
        exit(1);
    }

    if( argc == 1 ) //if there is no input
    {
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0] );
        exit(1);
    }


    return 0;
}

//Defining functions
void read_file(char *filename) // Read the file and assign the data to an array
{
    FILE *pfile;
    char line[100], *splitter;

    pfile = fopen(filename,"r"); //open file to read the data

    if( pfile == NULL ) //if the file can't open
    {
        printf("Cannot open one or more given files\n");
        exit(1);
    }

    while(fgets(line,100,pfile))
    {
        //Read the name
        splitter = strtok(line,",");
        strcpy(arrayMeeting[len].name,splitter);

        //Read the no:of participants
        splitter = strtok(NULL,",");
        arrayMeeting[len].participants = atoi(splitter);

        //Read the duration
        splitter = strtok(NULL,":");
        arrayMeeting[len].hour = atoi(splitter);

        splitter = strtok(NULL,":");
        arrayMeeting[len].min = atoi(splitter);

        splitter = strtok(NULL,",");
        arrayMeeting[len].sec = atoi(splitter);

        len++;

    }

    fclose(pfile);
}

int isInt(char *str1) //check the argument for integer
{
    int i, count = 1, number = -90909090;

    for(i = 1; i < strlen(str1); i++)
    {
        if( ((str1[0] == 45) || ((str1[0] >= 48) && (str1[0] <= 57)) ) && ((str1[i] >= 48) && (str1[i] <= 57)) )
        {
            count++;
        }
    }

    if((str1[0] == 45) && (strlen(str1) == 1))
    {
        count = 0;
    }

    if( count == strlen(str1) )
    {
        number = atoi(str1);
    }

    return number;

}

int check_CSV(char *str1) //check the argument for csv files
{
    int last_index, found = 0;

    last_index = strlen(str1);

    if( (str1[last_index - 4] == '.') && (str1[last_index - 3] == 'c') && (str1[last_index - 2] == 's') && (str1[last_index - 1] == 'v') )
    {
        found = 1;
    }

    return found;
}

int count_data(meeting_data *arr, int arrlen) //Find no:of meetings,participants and time duration a person had
{
    int i, j, k = 0;

    for(i = 0; i < arrlen; i++)
    {
        int count = 0, total_t = 0, people = 0;
        for(j = 0; j < arrlen; j++)
        {
            if( (strcmp(arr[i].name,arr[j].name) == 0) && ( arr[j].participants != -999) )
            {
                count++;
                total_t = total_t + arr[j].hour*60 + arr[j].min;
                people = people + arr[j].participants;
                arr[j].participants = -999;
            }
        }

        strcpy(newArray[k].name,arr[i].name);
        newArray[k].meetings = count;
        newArray[k].total_p = people;
        newArray[k].duration = total_t;

        if(count != 0)
        {
            k++;
        }

    }

    return k;
}

void sortArray(int size,meeting_data *array,int mode) //sort an array
{
    int i, j, k, count, largest;

    //sort by no:of meetings
    if( mode == 1 )
    {
        for(k = 0; k < size; k++)
        {
            for(i = size-1; i >= 0; i--)
            {
                count = 0;
                for(j = size-1; j >= 0; j--)
                {
                    if( array[i].meetings >= array[j].meetings )
                        {
                            count++;
                            if(count == size)
                                {
                                    largest = i;
                                    strcpy(sorted_data[k].name, array[i].name);
                                    sorted_data[k].meetings = array[i].meetings;
                                    sorted_data[k].total_p = array[i].total_p;
                                    sorted_data[k].duration = array[i].duration;
                                }
                        }
                }
            }
            array[largest].meetings = -1;
        }
    }

    //sort by no:of participants
    if( mode == 2 )
    {
        for(k = 0; k < size; k++)
        {
            for(i = size-1; i >= 0; i--)
            {
                count = 0;
                for(j = size-1; j >= 0; j--)
                {
                    if( array[i].total_p >= array[j].total_p )
                        {
                            count++;
                            if(count == size)
                                {
                                    largest = i;
                                    strcpy(sorted_data[k].name, array[i].name);
                                    sorted_data[k].meetings = array[i].meetings;
                                    sorted_data[k].total_p = array[i].total_p;
                                    sorted_data[k].duration = array[i].duration;
                                }
                        }
                }
            }
            array[largest].total_p = -1;
        }
    }

    //sort by duration
    if( mode == 3 )
    {
        for(k = 0; k < size; k++)
        {
            for(i = size-1; i >= 0; i--)
            {
                count = 0;
                for(j = size-1; j >= 0; j--)
                {
                    if( array[i].duration >= array[j].duration )
                        {
                            count++;
                            if(count == size)
                                {
                                    largest = i;
                                    strcpy(sorted_data[k].name, array[i].name);
                                    sorted_data[k].meetings = array[i].meetings;
                                    sorted_data[k].total_p = array[i].total_p;
                                    sorted_data[k].duration = array[i].duration;
                                }
                        }
                }
            }
            array[largest].duration = -1;
        }
    }
}

void visualize_data(meeting_data *arr, int length, int mode,int size,int scale ) //Printing the meeting data as a horizontal bar chart
{
    int i, bar, row, col, arraysize;
    int bar_space, name_space;

    if( length <= size )
    {
        arraysize = length;
    }
    else
    {
        arraysize = size;
    }

    if(length != 0)
    {
        putchar('\n');
    }

    name_space = (long_name(arr, arraysize) + 1);

    if( mode == 1 ) //for meetings
    {
        for(bar = 0; bar < arraysize; bar++)
        {
            for(row = 1; row <= 4; row++)
            {
                if(row == 2)
                {
                    print_name(arr, bar, arraysize);
                    printf("\u2502");
                    bar_space = bar_size(arr,bar,mode,size,name_space,scale);
                    for(col = 0; col < bar_space ; col++)
                    {
                        printf("\u2591");
                    }
                    printf("%d", arr[bar].meetings);

                }
                else if(row == 4)
                {
                    for(i = 0; i <= name_space; i++ )
                    {
                        printf(" ");
                    }

                    printf("\u2502");

                }
                else
                {
                    for(i = 0; i <= name_space; i++ )
                    {
                        printf(" ");
                    }
                    printf("\u2502");
                    bar_space = bar_size(arr,bar,mode,size,name_space,scale);
                    for(col = 0; col < bar_space ; col++)
                    {
                        printf("\u2591");
                    }

                }
                putchar('\n');
            }
        }
    }

    if( mode == 2 ) //for participants
    {
        for(bar = 0; bar < arraysize; bar++)
        {
            for(row = 1; row <= 4; row++)
            {
                if(row == 2)
                {
                    print_name(arr, bar, arraysize);
                    printf("\u2502");
                    bar_space = bar_size(arr,bar,mode,size,name_space,scale);
                    for(col = 0; col < bar_space ; col++)
                    {
                        printf("\u2591");
                    }
                    printf("%d", arr[bar].total_p);

                }
                else if(row == 4)
                {
                    for(i = 0; i <= name_space; i++ )
                    {
                        printf(" ");
                    }
                    printf("\u2502");

                }
                else
                {
                    for(i = 0; i <= name_space; i++ )
                    {
                        printf(" ");
                    }
                    printf("\u2502");
                    bar_space = bar_size(arr,bar,mode,size,name_space,scale);
                    for(col = 0; col < bar_space ; col++)
                    {
                        printf("\u2591");
                    }

                }
                putchar('\n');
            }
        }
    }

    if( mode == 3 ) //for duration
    {
        for(bar = 0; bar < arraysize; bar++)
        {
            for(row = 1; row <= 4; row++)
            {
                if(row == 2)
                {
                    print_name(arr, bar, arraysize);
                    printf("\u2502");
                    bar_space = bar_size(arr,bar,mode,size,name_space,scale);
                    for(col = 0; col < bar_space ; col++)
                    {
                        printf("\u2591");
                    }
                    printf("%d", arr[bar].duration);


                }
                else if(row == 4)
                {
                    for(i = 0; i <= name_space; i++ )
                    {
                        printf(" ");
                    }
                    printf("\u2502");

                }
                else
                {
                    for(i = 0; i <= name_space; i++ )
                    {
                        printf(" ");
                    }
                    printf("\u2502");
                    bar_space = bar_size(arr,bar,mode,size,name_space,scale);
                    for(col = 0; col < bar_space ; col++)
                    {
                        printf("\u2591");
                    }

                }
                putchar('\n');
            }
        }
    }

    if( (length != 0) && (size != 0) )
    {
            for(i = 0; i <= name_space; i++ )
            {
                printf(" ");
            }
            printf("\u2514");

            for(col = 0; col < (78 - name_space); col++)
            {
                printf("\u2500");
            }

            putchar('\n');
    }


}

int bar_size(meeting_data *array, int index, int mode, int size,int nameSpace, int scale) //find the bar length
{
    int allData = sum_data(array,size,mode), quantitySpace;
    float percentage;

    if(scale == 1) //if the bar graph is scaled
    {
        if( mode == 1 ) //for meetings
        {
            if(array[0].meetings < 10)
            {
                quantitySpace = 1;
            }
            else if((array[0].meetings >= 10) && (array[0].meetings < 100))
            {
                quantitySpace = 2;
            }
            else if((array[0].meetings >= 100) && (array[0].meetings < 1000))
            {
                quantitySpace = 3;
            }
            else if((array[0].meetings >= 1000) && (array[0].meetings < 10000))
            {
                quantitySpace = 4;
            }
            else
            {
                quantitySpace = 5;
            }

            percentage = (float)(78 - nameSpace - quantitySpace) *  array[index].meetings / array[0].meetings ;
        }
        if( mode == 2 ) //for participants
        {
            if(array[0].total_p < 10)
            {
                quantitySpace = 1;
            }
            else if((array[0].total_p >= 10) && (array[0].total_p < 100))
            {
                quantitySpace = 2;
            }
            else if((array[0].total_p >= 100) && (array[0].total_p < 1000))
            {
                quantitySpace = 3;
            }
            else if((array[0].total_p >= 1000) && (array[0].total_p < 10000))
            {
                quantitySpace = 4;
            }
            else
            {
                quantitySpace = 5;
            }

            percentage = (float)(78 - nameSpace - quantitySpace) * array[index].total_p / array[0].total_p ;
        }
        if( mode == 3 ) //for duration
        {
            if(array[0].duration < 10)
            {
                quantitySpace = 1;
            }
            else if((array[0].duration >= 10) && (array[0].duration < 100))
            {
                quantitySpace = 2;
            }
            else if((array[0].duration >= 100) && (array[0].duration < 1000))
            {
                quantitySpace = 3;
            }
            else if((array[0].duration >= 1000) && (array[0].duration < 10000))
            {
                quantitySpace = 4;
            }
            else
            {
                quantitySpace = 5;
            }

            percentage = (float)(78 - nameSpace - quantitySpace) * array[index].duration / array[0].duration ;
        }
    }

    if(scale == 0) //if the bar graph is not scaled
    {
        if( mode == 1 ) //for meetings
        {
            if(array[index].meetings < 10)
            {
                quantitySpace = 1;
            }
            else if((array[index].meetings >= 10) && (array[index].meetings < 100))
            {
                quantitySpace = 2;
            }
            else if((array[index].meetings >= 100) && (array[index].meetings < 1000))
            {
                quantitySpace = 3;
            }
            else if((array[index].meetings >= 1000) && (array[index].meetings < 10000))
            {
                quantitySpace = 4;
            }
            else
            {
                quantitySpace = 5;
            }

            percentage = (float)(78 - nameSpace - quantitySpace) *  array[index].meetings / allData ;
        }
        if( mode == 2 ) //for participants
        {
            if(array[0].total_p < 10)
            {
                quantitySpace = 1;
            }
            else if((array[index].total_p >= 10) && (array[index].total_p < 100))
            {
                quantitySpace = 2;
            }
            else if((array[index].total_p >= 100) && (array[index].total_p < 1000))
            {
                quantitySpace = 3;
            }
            else if((array[index].total_p >= 1000) && (array[index].total_p < 10000))
            {
                quantitySpace = 4;
            }
            else
            {
                quantitySpace = 5;
            }

            percentage = (float)(78 - nameSpace - quantitySpace) * array[index].total_p / allData ;
        }
        if( mode == 3 ) //for duration
        {
            if(array[index].duration < 10)
            {
                quantitySpace = 1;
            }
            else if((array[index].duration >= 10) && (array[index].duration < 100))
            {
                quantitySpace = 2;
            }
            else if((array[index].duration >= 100) && (array[index].duration < 1000))
            {
                quantitySpace = 3;
            }
            else if((array[index].duration >= 1000) && (array[index].duration < 10000))
            {
                quantitySpace = 4;
            }
            else
            {
                quantitySpace = 5;
            }

            percentage = (float)(78 - nameSpace - quantitySpace) * array[index].duration / allData ;
        }
    }

    return (percentage / 1);
}

void print_name(meeting_data *array, int index, int length) //print the name with spaces to align columns
{
    int i, name_len;
    char *str1 = array[index].name;

    name_len = long_name(array,length);

    printf(" ");
    for(i = 0; i <= name_len; i++)
    {
        if(i < strlen(str1))
        {
            printf("%c", str1[i]);

        }
        else
        {
            printf(" ");
        }

    }
}

int sum_data(meeting_data *array, int size,int mode) //getting the sum of all the data
{
    int i,sum = 0;

    if( mode == 1 ) //for meetings
    {
        for(i = 0; i < size; i++)
        {
            sum = sum + array[i].meetings;
        }
    }

    if( mode == 2 ) //for participants
    {
        for(i = 0; i < size; i++)
        {
            sum = sum + array[i].total_p;
        }
    }

    if( mode == 3 ) //for duration
    {
        for(i = 0; i < size; i++)
        {
            sum = sum + array[i].duration;
        }
    }

    return sum;
}

int long_name(meeting_data *array, int length) //find the longest length of the name from given list of names
{
    int i, j, nameLen;
    char *pname1 = NULL;
    char *pname2 = NULL;

    for(i = 0; i < length; i++)
    {
        pname1 = array[i].name;
        int count = 0;
        for(j = 0; j < length; j++)
        {
            pname2 = array[j].name;
            if( strlen(pname1) >= strlen(pname2) )
            {
                count++;
            }
        }
        if( count == length )
        {
            nameLen = strlen(pname1);
        }
    }

    return nameLen;
}

void chart(int mode,int length,int scale) //print the chart
{

    int limit = count_data(arrayMeeting,len);

    if(limit == 0) //if the csv file is empty
    {
        printf("No data to process\n");
        exit(1);
    }

    sortArray(limit,newArray,mode);

    visualize_data(sorted_data,length,mode,limit,scale);


}















