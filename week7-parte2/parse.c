#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>

#define LimitRequestLine 8190

bool parse(const char* line, char* abs_path, char* query);

int main (void)
{
    const char* LINE = "GET / HTTP/1.1";
    char* ABS_PATH = NULL;
    char* QUERY = NULL;
    if (parse(LINE, ABS_PATH, QUERY))
    {
        printf("Good!\n");
    }
    else
    printf("Bad!\n");
    return 0;
}
bool parse(const char* line, char* abs_path, char* query)
{
    // find the first space
    char* point_space1;
    point_space1 = strchr(line, ' ');
    if (point_space1 == NULL)
    {
        printf("error(400) line 32\n");
        return false;
    }
    
    //find the second space
    char* crawler = (point_space1 + 1);
    char* point_space2;
    point_space2 = strchr(crawler, ' ');
    if (point_space2 == NULL)
    {
        printf("error(400) line 42\n");
        return false;
    }
    
    // check if there are more spaces
    char* point_space3;
    crawler = (point_space2 + 1);
    point_space3 = strchr(crawler, ' ');
    if (point_space3 != NULL) // if there are 3 spaces return error
    {
        printf("error(400) line 52\n");
        return false;
    }
    
    // check if request-target starts with '/' 
    crawler = (point_space1 + 1);
    if (strncmp(crawler, "/", 1) != 0)
    {
        printf("error(501) line 60\n");
        return false;
    }
    
    
    int difference;
    char* point_char34 = NULL;
    // check for question mark
    char* point_question;
    point_question = strchr(point_space1, '?');
    if (point_question != NULL)
    {
        // check for " that in ASCII es 34
        point_char34 = strchr(point_question, (char)34);
        if (point_char34 != NULL)
        {
            printf("error(400) line 78\n");
            return false;
        }
        
        // asign to abs_path and query the corresponding string for each one from request-target
        difference = point_question - point_space1 - 1;
        strncpy(abs_path, crawler, difference);
        difference = point_space2 - point_question - 1;
        if (difference == 0)
        query = "";
        else
        strncpy(query, ++point_question, difference);
    }
    else // if is not point_question asign to abs_path all "request-target" and query '\0'
    {
        difference = point_space2 - point_space1 - 1;
        strncpy(abs_path, crawler, difference);
        query = "";
    }
    
    // check if method is GET
    difference = point_space1 - line;
    if (strncmp(line, "GET", difference) != 0)
    {
        printf("error(405) line 102\n");
        return false;
    }
    
    // check if HTTP-version is HTTP/1.1
    crawler = (point_space2 + 1);
    if (strncmp(crawler, "HTTP/1.1", 8) != 0)
    {
        printf("error(505) line 110\n");
        return false;
    }

    return true;
}