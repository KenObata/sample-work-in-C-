#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"
#include <time.h>

/*
 * Function dt_format:
 * Given a date-time, creates a more readable version of the
 * calendar date by using some C-library routines. 
 * For example, 20190520T111500 is returned as "May 20, 2019 (Mon)".
 */
void dt_format(char *formatted_time, const char *dt_time, const int len)
{
    struct tm temp_time;
    time_t    full_time;

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
           &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)",
             localtime(&full_time));
}

/*
 * Function extract:
 * Given information of struct dt_start or dt_end, and year, month, day, hour,
 * minutes, seconds, am/pm, from the main function,
 * this function updates these dates information by using pointer.
 * Also, this function converts hours into 12-hour format with am/pm.
 */
void extract(char *dt, int *yy, int *mo, int *dd, int *hh, int *min, char *ampm)
{
    char *year = emalloc(5*sizeof(char));
    strncpy(year,dt,4);
    year[4]='\0';
    *yy=atoi(year);
    
    char *month = emalloc(3*sizeof(char));
    strncpy(month,&dt[4],2);
    month[2]='\0';
    *mo=atoi(month);
    
    char *day = emalloc(3*sizeof(char));
    strncpy(day,&dt[6],2);
    day[2]='\0';
    *dd=atoi(day);

    char *hour = emalloc(3*sizeof(char));
    strncpy(hour,&dt[9],2);
    hour[2]='\0';
    
    //Convert hours into 12-hour format
    if(atoi(hour)>12)
    {
        *hh=atoi(hour)-12;
        strncpy(ampm,"pm",2);
        ampm[2]='\0';
    }
    else if(atoi(hour)==12)
    {
        *hh=atoi(hour);
        strncpy(ampm,"pm",2);
        ampm[2]='\0';
    }
    else
    {
        *hh=atoi(hour);
        strncpy(ampm,"am",2);
        ampm[2]='\0';
    }
    
    char *minutes = emalloc(3*sizeof(char));
    strncpy(minutes,&dt[11],2);
    minutes[2]='\0';
    *min=atoi(minutes);
    
    free(year); free(month); free(day); free(hour); free(minutes);
    assert(year != NULL);
}

/*
 * Function getDiff:
 * Given information of dt_end, repeating date UNTIL from the main function,
 * this function calculates the date difference between dt_end and repeating date UNTIL
 * and updates int diffDay in the main function by using pointer.
 */
int getDiff(char *dt_end,char *repeat_until) {
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    
    char year[5];
    //char *year = emalloc(5*sizeof(char));
    strncpy(year,dt_end,4);
    year[4]='\0';
    tm.tm_year=atoi(year);
    
    char month[3];
    //char *month = emalloc(3*sizeof(char));
    strncpy(month,&dt_end[4],2);
    month[2]='\0';
    tm.tm_mon=atoi(month);
    
    char day[3];
    //char *day = emalloc(3*sizeof(char));
    strncpy(day,&dt_end[6],2);
    day[2]='\0';
    tm.tm_mday=atoi(day);
    
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    time_t time_dt_start = mktime(&tm);
    
    strncpy(year,repeat_until,4);
    year[4]='\0';
    tm.tm_year=atoi(year);
    
    strncpy(month,&repeat_until[4],2);
    month[2]='\0';
    tm.tm_mon=atoi(month);
    
    strncpy(day,&repeat_until[6],2);
    day[2]='\0';
    tm.tm_mday=atoi(day);
    
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    time_t time_dt_repeat_until = mktime(&tm);
    
    return (time_dt_repeat_until/86400)-(time_dt_start/86400);
    //free(year); free(month); free(day);
    //assert(year != NULL);
}

/*
 * Function dt_increment:
 * Given a date-time, it adds the number of days in a way that
 * results in the correct year, month, and day. For example,
 * if the string in "before" corresponds to: 20190520T111500, 
 * then the datetime string stored in "after", assuming that
 * "num_days" is 100, will be: 20190828T111500
 */
void dt_increment(char *after, char *before, int num_days)
{
    struct tm temp_time;
    time_t    full_time;
    
    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
           &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;
    
    full_time = mktime(&temp_time);
    
    after[0] = '\0';
    strftime(after, 9, "%Y%m%d", localtime(&full_time));
    strncpy(after + 8, before + 8, 7);

}

/*
 * Function dashes:
 * print necessary length of dashes.
 */

void dashes(char *output)
{
    /* declare to output necessary length of "-----". */
    int i = 0;
    int count = strlen(output);
    
    for (i = 0; i < count; i++)
    {
        printf("-");
    }
    printf("\n");
}

/*
 * Function print_event:
 *  Given a linked list, this function prints events information.
 *  By getting previous event, this method decides 
 *  if printing event is the new date or same date as previous event.
 */
void print_event(node_t *n, void *arg, char *yyyymmdd_from)
{
    assert(n != NULL);
    event_t *event = n->val;
    
    /* get dt_format */
    char *output= emalloc(22*sizeof(char));
    dt_format(output, event->dtstart, 22);
    
    /* get from dt_start to check if event dates are new dates*/
    int s_yyyy, s_mo, s_dd, s_hh, s_min, e_hh, e_min;
    char ampm[3], e_ampm[3]; 
    int prev_yyyy, prev_mo, prev_dd, prev_hh, prev_min;
    
    if( n->previous != NULL){
        event_t *prev_event = n->previous->val;
        extract(prev_event->dtstart, &prev_yyyy, &prev_mo, &prev_dd, &prev_hh, &prev_min, ampm);
    }
    
    //get Year, Month, day from dt_start
    extract(event->dtstart, &s_yyyy, &s_mo, &s_dd, &s_hh, &s_min, ampm);
    extract(event->dtend, &s_yyyy, &s_mo, &s_dd, &e_hh, &e_min, e_ampm);
    
    // if event is the first event to be output, no blank line.
    if( n->previous == NULL)
    {
        printf("%s\n",output);
        dashes(output);
    }
    else if(n->previous != NULL )
    {   // if event is the first event to be output, no blank line.
        if( strncmp(n->previous->val->dtstart, yyyymmdd_from, 8) < 0 
        && strncmp(yyyymmdd_from, event->dtstart ,8) <= 0 )
        {
            printf("%s\n",output);
            dashes(output);
        }
        //if event is new date, print blank line.
        else if(prev_dd != s_dd){
            printf("\n%s\n",output);
            dashes(output);
        }
    }
    /*print location, summary with blank line if it's new date event */
    printf("%2d:%02d %s to %2d:%02d %s: %s [%s]\n", 
          s_hh, s_min, ampm, e_hh, e_min, e_ampm, event->summary, event->location);
    
    free(output);
}

int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i= 0;
    
    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 7) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_d, &from_m, &from_y);
        } else if (strncmp(argv[i], "--end=", 5) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_d, &to_m, &to_y);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }
    
    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr,
                "usage: %s --start=dd/mm/yyyy --end=dd/mm/yyyy --file=icsfile\n",
                argv[0]);
        exit(1);
    }
    
    /* concatenate dd mm yyyy to compare command line date and dt_start */
    char *unixDates_from_concate = emalloc(9*sizeof(char));
    sprintf(unixDates_from_concate,"%d%02d%02d",from_y,from_m,from_d);
    
    char *unixDates_to_concate = emalloc(9*sizeof(char));
    sprintf(unixDates_to_concate,"%d%02d%02d",to_y,to_m,to_d);
    
    /* declare for getline */
    FILE *pfile;
    char *buffer = NULL;
    size_t buffer_len = 0;
    size_t read;
    int diff = 0;//diff between dtstart and rrule
    //file open
    pfile = fopen(filename, "r");
    if (pfile == NULL)
    {
        fprintf(stderr, "cannot open %s\n", filename);
        exit(1);
    }
    
    /* declare node for linked list */
    event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;
    node_t *current = NULL;
    event_t *dummy = NULL;
    dummy = emalloc(sizeof(event_t));
    head = new_node(dummy);
    node_t *previous = NULL;
    node_t *repeat_node = NULL;
    
    /* declare for repeating event */
    char *dt_start_repeat = emalloc(16*sizeof(char));
    char *dt_end_repeat = emalloc(16*sizeof(char));
    
    /* read line by line */
    while ( ( read = getline(&buffer, &buffer_len, pfile)) != -1)
    {
        if (strncmp(buffer,"DTSTART",7) == 0)
        {
            //create event in each start of evenet
            temp_event = emalloc(sizeof(event_t));
            strncpy(temp_event->dtstart, buffer + 8, 15);
            temp_event->dtstart[16]='\0';
        }
        else if (strncmp(buffer,"DTEND",5) == 0)
        {
            strncpy(temp_event->dtend, buffer + 6, 15);
            temp_event->dtend[16]='\0';
        }
        else if (strncmp(buffer,"RRULE",5) == 0)
        {
            strncpy(temp_event->rrule, buffer + 32, 15);
            temp_event->dtend[16]='\0';
        }
        else if (strncmp(buffer,"LOCATION",8) == 0)
        {
            strncpy(temp_event->location, buffer + 9, strlen(buffer)-9);
            temp_event->location[strlen(buffer)-10]='\0';
        }
        else if (strncmp(buffer,"SUMMARY",7) == 0)
        {
            strncpy(temp_event->summary, buffer + 8, strlen(buffer)-8 );
            temp_event->summary[strlen(buffer)-9]='\0';
        }
        else if (strncmp(buffer,"END:VEVENT", 10) == 0)
        {
            temp_node = new_node(temp_event);
            
            previous = head;
            current = head->next;
            /* search position and insert */
            temp_node = insert(temp_node, current, previous);
            
            /* create repeating event */
            if(strlen(temp_event->rrule) != 0)
            {
                diff = getDiff(temp_event->dtstart, temp_event->rrule);
                for(i = 7; i <= diff ; i += 7)
                {
                    event_t *repeat_event= NULL;
                    repeat_event = emalloc(sizeof(event_t));
                    
                    //repeat dtstart
                    dt_increment(dt_start_repeat, temp_event->dtstart, i);
                    dt_start_repeat[16]='\0';
                    strncpy(repeat_event->dtstart,dt_start_repeat,15);
                    repeat_event->dtstart[16]='\0';
                    
                    //repeat dtend
                    dt_increment(dt_end_repeat, temp_event->dtend, i);
                    dt_end_repeat[16]='\0';
                    strncpy(repeat_event->dtend,dt_end_repeat,15);
                    repeat_event->dtend[16]='\0';
                    
                    //repeat location into struct
                    strncpy(repeat_event->location, temp_event->location,
                            strlen(temp_event->location));
                    repeat_event->location[strlen(temp_event->location)]='\0';
                    
                    //repeat summary info into struct
                    strncpy(repeat_event->summary, temp_event->summary, 
                            strlen(temp_event->summary));
                    repeat_event->summary[strlen(temp_event->summary)]='\0';
                    
                    repeat_node = new_node(repeat_event);

                    /* search position and insert */
                    previous = head;
                    current = head->next;
                    repeat_node = insert(repeat_node, current, previous);
                }//end of repeat
            }
            temp_event = NULL;
        }//end of elseif END:VEVENT
    }// end of getline

    /* free buffer */
    if(buffer)
        {
            free(buffer);        
        }
        
    //print
    apply(head, print_event, NULL ,unixDates_from_concate, unixDates_to_concate);
    
    /* Free up the memory. */
    //assert(temp_node != NULL);
    //assert(temp_event != NULL);

    freeList(head);
    free(unixDates_from_concate); free(unixDates_to_concate);
    free(dt_start_repeat); free(dt_end_repeat);
    
    fclose(pfile);
    exit(0);
}//end of main









