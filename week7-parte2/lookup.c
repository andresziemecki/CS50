#include <stdio.h>
#include <strings.h>
#include <string.h>

const char* lookup (const char* path);

int main (int argc, const char* argv[])
{
    const char* PATH = "cat.html";
    PATH = lookup(PATH);
    if (PATH == NULL)
        printf("Good!\n");
    else
        printf("Bad!\n");
    return 0;
}

const char* lookup (const char* path)
{
    // checks if the there is no file
    if (path == NULL)
        return NULL;
    
    // search for the last dot
    const char* crawler = path;
    crawler = strrchr(path, '.');
    if (crawler == NULL)
        return NULL;
        
    crawler = (crawler + 1);
    
    // check what type it's path
    if (strcasecmp(crawler, "css") == 0)
    {
        return "text/css";
    }
    if (strcasecmp(crawler, "html") == 0)
    {
        return "text/html";
    }
    if (strcasecmp(crawler, "gif") == 0)
    {
        return "text/html";
    }
    if (strcasecmp(crawler, "ico") == 0)
    {
        return "image/x-icon";
    }
    if (strcasecmp(crawler, "jpg") == 0)
    {
        return "image/jpeg";
    }
    if (strcasecmp(crawler, "js") == 0)
    {
        return "text/javascript";
    }
    if (strcasecmp(crawler, "php") == 0)
    {
        return "text/x-php";
    }
    if (strcasecmp(crawler, "png") == 0)
    {
        return "image/png";
    }
    
    //return NULL in other case
    return NULL;
}