//
// server.c
//
// Computer Science 50
// Problem Set 6
//

// feature test macro requirements
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#define _XOPEN_SOURCE_EXTENDED

// limits on an HTTP request's size, based on Apache's
// http://httpd.apache.org/docs/2.2/mod/core.html
#define LimitRequestFields 50
#define LimitRequestFieldSize 4094
#define LimitRequestLine 8190

// number of bytes for buffers
#define BYTES 512

// header files
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// types
typedef char BYTE;

// prototypes
bool connected(void);
void error(unsigned short code);
void freedir(struct dirent** namelist, int n);
void handler(int signal);
char* htmlspecialchars(const char* s);
char* indexes(const char* path);
void interpret(const char* path, const char* query);
void list(const char* path);
bool load(FILE* file, BYTE** content, size_t* length);
const char* lookup(const char* path);
bool parse(const char* line, char* path, char* query);
const char* reason(unsigned short code);
void redirect(const char* uri);
bool request(char** message, size_t* length);
void respond(int code, const char* headers, const char* body, size_t length);
void start(short port, const char* path);
void stop(void);
void transfer(const char* path, const char* type);
char* urldecode(const char* s);

// server's root
char* root = NULL;

// file descriptor for sockets
int cfd = -1, sfd = -1;

// flag indicating whether control-c has been heard
bool signaled = false;

int main(int argc, char* argv[])
{
    // a global variable defined in errno.h that's "set by system 
    // calls and some library functions [to a nonzero value]
    // in the event of an error to indicate what went wrong"
    
    errno = 0; //variable declarada en errno.h que se usa en bastantes funciones para indicar casos de errores

    // default to port 8080
    int port = 8080;

    // usage
    const char* usage = "Usage: server [-p port] /path/to/root";

    // parse command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "hp:")) != -1) // lee todos los elementos, hasta los opcionales que se pasan a argv como "-"
    {
        switch (opt)
        {
            // -h
            case 'h':
                printf("%s\n", usage);
                return 0;

            // -p port
            case 'p':
                port = atoi(optarg);
                break;
        }
    }

    // ensure port is a non-negative short and path to server's root is specified
    if (port < 0 || port > SHRT_MAX || argv[optind] == NULL || strlen(argv[optind]) == 0)
    {
        // announce usage
        printf("%s\n", usage);

        // return 2 just like bash's builtins
        return 2;
    }

    // start server
    start(port, argv[optind]);

    // listen for SIGINT (aka control-c). 
    // SIGINT (signal intelligents) si hubo alguna señal como control-C va a llamar una funcion handler que viene de algun lado
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);

    // a message and its length
    char* message = NULL;
    size_t length = 0;

    // path requested
    char* path = NULL;

    // accept connections one at a time
    while (true)
    {
        // libera toda la memoria que fue utilizada en alguna previa iteracion del while-loop
        // free last path, if any
        if (path != NULL)
        {
            free(path);
            path = NULL;
        }
        // free last message, if any
        if (message != NULL)
        {
            free(message);
            message = NULL;
        }
        length = 0;

        // we then check whether if we have been signaled via control-c to stop the server
        // close last client's socket, if any
        if (cfd != -1)
        {
            close(cfd);
            cfd = -1;
        }
        // check for control-c
        if (signaled)
        {
            stop();
        }

        // check whether client has connected
        if (connected()) //return true if a client ahs connected to the server
        {
            // check for request
            if (request(&message, &length))
            {
                // extract message's request-line
                // http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
                const char* haystack = message;
                const char* needle = strstr(haystack, "\r\n");
                if (needle == NULL)
                {
                    error(500);
                    continue;
                }
                char line[needle - haystack + 2 + 1];
                strncpy(line, haystack, needle - haystack + 2);
                line[needle - haystack + 2] = '\0';

                // log request-line
                printf("%s", line);

                // parse request-line
                char abs_path[LimitRequestLine + 1];
                char query[LimitRequestLine + 1];
                if (parse(line, abs_path, query)) //le pasa dos arrays, una con la ruta absoluta y el otro con la consulta
                {
                    // la mayoria de lo siguiente decodifica la algunos caracteres de la url como %20 para ver que archivo exactamente fue pedido
                    
                    // URL-decode absolute-path
                    char* p = urldecode(abs_path);
                    if (p == NULL)
                    {
                        error(500);
                        continue;
                    }

                    // resolve absolute-path to local path
                    path = malloc(strlen(root) + strlen(p) + 1);
                    if (path == NULL)
                    {
                        error(500);
                        continue;
                    }
                    strcpy(path, root);
                    strcat(path, p);
                    free(p);

                    // ensure path exists
                    if (access(path, F_OK) == -1)
                    {
                        error(404);
                        continue;
                    }

                    /* Below that, we ascertain whether that path leads to a directory or to a file and handle the request accordingly,
                       ultimately calling list, interpret, or transfer. */

                    /* For directories (that don’t have an index.php or index.html file inside them),
                       we call list in order to display the directory’s contents. */
                    // if path to directory
                    struct stat sb;
                    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
                    {
                        // redirect from absolute-path to absolute-path/
                        if (abs_path[strlen(abs_path) - 1] != '/')
                        {
                            char uri[strlen(abs_path) + 1 + 1];
                            strcpy(uri, abs_path);
                            strcat(uri, "/");
                            redirect(uri);
                            continue;
                        }

                        // use path/index.php or path/index.html, if present, instead of directory's path
                        char* index = indexes(path);
                        if (index != NULL)
                        {
                            free(path);
                            path = index;
                        }

                        // list contents of directory
                        else
                        {
                            list(path);
                            continue;
                        }
                    }

                    // For files ending in .php (whose "MIME type" is text/x-php), we call interpret
                    // look up MIME type for file at path
                    const char* type = lookup(path);
                    if (type == NULL)
                    {
                        error(501);
                        continue;
                    }

                    // For other (supported) files, we call transfer.
                    // interpret PHP script at path
                    if (strcasecmp("text/x-php", type) == 0)
                    {
                        interpret(path, query);
                    }

                    // transfer file at path
                    else
                    {
                        transfer(path, type);
                    }
                }
            }
            /* Notice there are a few uses of continue, the effect of which is to jump back to the start of that infinite loop. 
               Just before continue in some cases, too, is a call to error (another function we wrote) with an HTTP status code.
               Together, those lines allow the server to handle and respond to errors just before returning its attention to new requests.*/
        }
    }
}

/**
 * Checks (without blocking) whether a client has connected to server. 
 * Returns true iff so.
 */
bool connected(void)
{
    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr)); // memset () rellena los primeros n bytes del área de memoria apuntada por s con el byte constante c.
    socklen_t cli_len = sizeof(cli_addr);
    cfd = accept(sfd, (struct sockaddr*) &cli_addr, &cli_len); // accept: accept a new connection on a socket
    if (cfd == -1)
    {
        return false;
    }
    return true;
}

/**
 * Responds to client with specified status code.
 * It is that function via which we respond to browsers with errors (e.g., 404)
 */
void error(unsigned short code)
{
    // determine code's reason-phrase
    const char* phrase = reason(code);
    if (phrase == NULL)
    {
        return;
    }

    // template for response's content
    char* template = "<html><head><title>%i %s</title></head><body><h1>%i %s</h1></body></html>";

    // render template
    char body[(strlen(template) - 2 - ((int) log10(code) + 1) - 2 + strlen(phrase)) * 2 + 1];
    int length = sprintf(body, template, code, phrase, code, phrase);
    if (length < 0)
    {
        body[0] = '\0';
        length = 0;
    }

    // respond with error
    char* headers = "Content-Type: text/html\r\n";
    respond(code, headers, body, length);
}

/**
 * Frees memory allocated by scandir.
 */
void freedir(struct dirent** namelist, int n)
{
    if (namelist != NULL)
    {
        for (int i = 0; i < n; i++)
        {
            free(namelist[i]);
        }
        free(namelist);
    }
}
 
/**
 * Handles signals. This function (called whenever a user hits control-c)
   essentially tells main to call stop by setting signaled, a global variable, to true.
 */
void handler(int signal)
{
    // control-c
    if (signal == SIGINT)
    {
        signaled = true;
    }
}

/**
 * Escapes string for HTML. Returns dynamically allocated memory for escaped
 * string that must be deallocated by caller.
 * los caracteres de escape de html "<>" se los llama desde una lista
 */
char* htmlspecialchars(const char* s)
{
    // ensure s is not NULL
    if (s == NULL)
    {
        return NULL;
    }

    // allocate enough space for an unescaped copy of s
    char* t = malloc(strlen(s) + 1);
    if (t == NULL)
    {
        return NULL;
    }
    t[0] = '\0';

    // iterate over characters in s, escaping as needed
    for (int i = 0, old = strlen(s), new = old; i < old; i++)
    {
        // escape &
        if (s[i] == '&')
        {
            const char* entity = "&amp;";
            new += strlen(entity);
            t = realloc(t, new);
            if (t == NULL)
            {
                return NULL;
            }
            strcat(t, entity);
        }

        // escape "
        else if (s[i] == '"')
        {
            const char* entity = "&quot;";
            new += strlen(entity);
            t = realloc(t, new);
            if (t == NULL)
            {
                return NULL;
            }
            strcat(t, entity);
        }

        // escape '
        else if (s[i] == '\'')
        {
            const char* entity = "&#039;";
            new += strlen(entity);
            t = realloc(t, new);
            if (t == NULL)
            {
                return NULL;
            }
            strcat(t, entity);
        }

        // escape <
        else if (s[i] == '<')
        {
            const char* entity = "&lt;";
            new += strlen(entity);
            t = realloc(t, new);
            if (t == NULL)
            {
                return NULL;
            }
            strcat(t, entity);
        }

        // escape >
        else if (s[i] == '>')
        {
            const char* entity = "&gt;";
            new += strlen(entity);
            t = realloc(t, new);
            if (t == NULL)
            {
                return NULL;
            }
            strcat(t, entity);
        }

        // don't escape
        else
        {
            strncat(t, s + i, 1);
        }
    }

    // escaped string
    return t;
}

/**
 * Checks, in order, whether index.php or index.html exists inside of path.
 * Returns path to first match if so, else NULL.
 */
char* indexes(const char* path)
{
    // count how many characters has directory
    int i = strlen(path);
    
    //create a variable to store directory and index.html
    char* index1 = malloc(i*sizeof(BYTE) + 10);
    if (index1 == NULL)
        return NULL;
    
    // store path in index1
    strcpy(index1, path);
    
    // append to index1 and put index.php
    strcat(index1, "/index.php");
    
    // try to open this file
    FILE* fp = NULL;
    fp = fopen(index1, "r");
    if (fp != NULL)
        {
            fclose(fp);
            return index1;
        }
    
    // if doesn't work, free the memory and proced the same with the other file
    free(index1);
    char* index2 = malloc(i*sizeof(BYTE) + 11);
    if (index2 == NULL)
        return NULL;
    strcpy(index2, path);
    strcat(index2, "/index.html");
    fp = fopen(index2, "r");
    if (fp != NULL)
    {
        fclose(fp);
        return index2;
    }
    
    // if no-one of the file could open, free and terminate
    free(index2);
    return NULL;
}

/**
 * Interprets PHP file at path using query string.
 * This function enables the server to interpret PHP files
 * the effect of which is to pass the contents of hello.php to PHP’s interpreter
 */
void interpret(const char* path, const char* query)
{
    // ensure path is readable
    if (access(path, R_OK) == -1)
    {
        error(403);
        return;
    }

    // open pipe to PHP interpreter
    char* format = "QUERY_STRING=\"%s\" REDIRECT_STATUS=200 SCRIPT_FILENAME=\"%s\" php-cgi";
    char command[strlen(format) + (strlen(path) - 2) + (strlen(query) - 2) + 1];
    if (sprintf(command, format, query, path) < 0)
    {
        error(500);
        return;
    }
    FILE* file = popen(command, "r");
    if (file == NULL)
    {
        error(500);
        return;
    }

    // load interpreter's content
    char* content;
    size_t length;
    if (load(file, &content, &length) == false)
    {
        error(500);
        return;
    }

    // close pipe
    pclose(file);

    // subtract php-cgi's headers from content's length to get body's length
    char* haystack = content;
    char* needle = strstr(haystack, "\r\n\r\n");
    if (needle == NULL)
    {
        free(content);
        error(500);
        return;
    }

    // extract headers
    char headers[needle + 2 - haystack + 1];
    strncpy(headers, content, needle + 2 - haystack);
    headers[needle + 2 - haystack] = '\0';

    // respond with interpreter's content
    respond(200, headers, needle + 4, length - (needle - haystack + 4));

    // free interpreter's content
    free(content);
}

/**
 * Responds to client with directory listing of path.
 * En otras palabras genera el listado del directorio
 */
void list(const char* path)
{
    // ensure path is readable and executable
    if (access(path, R_OK | X_OK) == -1)
    {
        error(403);
        return;
    }

    // open directory
    DIR* dir = opendir(path);
    if (dir == NULL)
    {
        return;
    }

    // buffer for list items
    char* list = malloc(1);
    list[0] = '\0';

    // iterate over directory entries
    struct dirent** namelist = NULL;
    int n = scandir(path, &namelist, NULL, alphasort);
    for (int i = 0; i < n; i++)
    {
        // omit . from list
        if (strcmp(namelist[i]->d_name, ".") == 0)
        {
            continue;
        }

        // escape entry's name
        char* name = htmlspecialchars(namelist[i]->d_name);
        if (name == NULL)
        {
            free(list);
            freedir(namelist, n);
            error(500);
            return;
        }

        // append list item to buffer
        char* template = "<li><a href=\"%s\">%s</a></li>";
        list = realloc(list, strlen(list) + strlen(template) - 2 + strlen(name) - 2 + strlen(name) + 1);
        if (list == NULL)
        {
            free(name);
            freedir(namelist, n);
            error(500);
            return;
        }
        if (sprintf(list + strlen(list), template, name, name) < 0)
        {
            free(name);
            freedir(namelist, n);
            free(list);
            error(500);
            return;
        }

        // free escaped name
        free(name);
    }

    // free memory allocated by scandir
    freedir(namelist, n);

    // prepare response
    const char* relative = path + strlen(root);
    char* template = "<html><head><title>%s</title></head><body><h1>%s</h1><ul>%s</ul></body></html>";
    char body[strlen(template) - 2 + strlen(relative) - 2 + strlen(relative) - 2 + strlen(list) + 1];
    int length = sprintf(body, template, relative, relative, list);
    if (length < 0)
    {
        free(list);
        closedir(dir);
        error(500);
        return;
    }

    // free buffer
    free(list);

    // close directory
    closedir(dir);

    // respond with list
    char* headers = "Content-Type: text/html\r\n";
    respond(200, headers, body, length);
}

/**
 * Loads a file into memory dynamically allocated on heap.
 * Stores address thereof in *content and length thereof in *length.
 */
bool load(FILE* file, BYTE** content, size_t* length)
{
    if (file==NULL) { return false; }

    // obtain file size:
    fseek (file , 0 , SEEK_END);
    long lSize;
    lSize = ftell (file);
    rewind (file);

    // allocate memory to contain the whole file:
    char* buffer = malloc(sizeof(char)*lSize + 1);
    if (buffer == NULL) 
        return false; 

    // copy the file into the buffer:
    *length = fread (buffer,1,lSize,file);

    // the whole file is now loaded in the memory buffer

    // terminate
    *content = buffer;
    return true;
}

/**
 * Returns MIME type for supported extensions, else NULL.
 */
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
        return "image/gif";
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

/**
 * Parses a request-line, storing its absolute-path at abs_path 
 * and its query string at query, both of which are assumed
 * to be at least of length LimitRequestLine + 1.
 */
bool parse(const char* line, char* abs_path, char* query)
{
    // find the first space
    char* point_space1;
    point_space1 = strchr(line, ' ');
    if (point_space1 == NULL)
    {
        error(400);
        return false;
    }
    
    //find the second space
    char* crawler = (point_space1 + 1);
    char* point_space2;
    point_space2 = strchr(crawler, ' ');
    if (point_space2 == NULL)
    {
        error(400);
        return false;
    }
    
    // check if there are more spaces
    char* point_space3;
    crawler = (point_space2 + 1);
    point_space3 = strchr(crawler, ' ');
    if (point_space3 != NULL) // if there are 3 spaces return error
    {
        error(400);
        return false;
    }
    
    // check if request-target starts with '/' 
    crawler = (point_space1 + 1);
    if (strncmp(crawler, "/", 1) != 0)
    {
        error(501);
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
            error(400);
            return false;
        }
        
        // asign to abs_path and query the corresponding string for each one from request-target
        difference = point_question - point_space1 - 1;
        strncpy(abs_path, crawler, difference);
        *(abs_path + difference) = '\0';
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
        *(abs_path + difference) = '\0';
        query = "";
    }
    
    // check if method is GET
    difference = point_space1 - line;
    if (strncmp(line, "GET", difference) != 0)
    {
        error(405);
        return false;
    }
    
    // check if HTTP-version is HTTP/1.1
    crawler = (point_space2 + 1);
    if (strncmp(crawler, "HTTP/1.1", 8) != 0)
    {
        error(505);
        return false;
    }

    return true;
}


/**
 * Returns status code's reason phrase.
 *
 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6
 * https://tools.ietf.org/html/rfc2324
 * 
 * En otras palabras, asigna una cadena de string a cada codigo
 */
const char* reason(unsigned short code)
{
    switch (code)
    {
        case 200: return "OK";
        case 301: return "Moved Permanently";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 414: return "Request-URI Too Long";
        case 418: return "I'm a teapot";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 505: return "HTTP Version Not Supported";
        default: return NULL;
    }
}

/**
 * Redirects client to uri.
 * This function redirects a client to another location (i.e., URL) by sending a status code of 301 plus a Location header.
 */
void redirect(const char* uri)
{
    char* template = "Location: %s\r\n";
    char headers[strlen(template) - 2 + strlen(uri) + 1];
    if (sprintf(headers, template, uri) < 0)
    {
        error(500);
        return;
    }
    respond(301, headers, NULL, 0);
}

/**
 * Reads (without blocking) an HTTP request's headers into memory dynamically allocated on heap.
 * Stores address thereof in *message and length thereof in *length.
 */
bool request(char** message, size_t* length)
{
    // ensure socket is open
    if (cfd == -1)
    {
        return false;
    }

    // initialize message and its length
    *message = NULL;
    *length = 0;

    // read message 
    while (*length < LimitRequestLine + LimitRequestFields * LimitRequestFieldSize + 4)
    {
        // read from socket
        BYTE buffer[BYTES];
        ssize_t bytes = read(cfd, buffer, BYTES);
        if (bytes < 0)
        {
            if (*message != NULL)
            {
                free(*message);
                *message = NULL;
            }
            *length = 0;
            break;
        }

        // append bytes to message 
        *message = realloc(*message, *length + bytes + 1);
        if (*message == NULL)
        {
            *length = 0;
            break;
        }
        memcpy(*message + *length, buffer, bytes);
        *length += bytes;

        // null-terminate message thus far
        *(*message + *length) = '\0';

        // search for CRLF CRLF
        int offset = (*length - bytes < 3) ? *length - bytes : 3;
        char* haystack = *message + *length - bytes - offset;
        char* needle = strstr(haystack, "\r\n\r\n");
        if (needle != NULL)
        {
            // trim to one CRLF and null-terminate
            *length = needle - *message + 2;
            *message = realloc(*message, *length + 1);
            if (*message == NULL)
            {
                break;
            }
            *(*message + *length) = '\0';

            // ensure request-line is no longer than LimitRequestLine
            haystack = *message;
            needle = strstr(haystack, "\r\n");
            if (needle == NULL || (needle - haystack + 2) > LimitRequestLine)
            {
                break;
            }

            // count fields in message
            int fields = 0;
            haystack = needle + 2;
            while (*haystack != '\0')
            {
                // look for CRLF
                needle = strstr(haystack, "\r\n");
                if (needle == NULL)
                {
                    break;
                }

                // ensure field is no longer than LimitRequestFieldSize
                if (needle - haystack + 2 > LimitRequestFieldSize)
                {
                    break;
                }

                // look beyond CRLF
                haystack = needle + 2;
            }

            // if we didn't get to end of message, we must have erred
            if (*haystack != '\0')
            {
                break;
            }

            // ensure message has no more than LimitRequestFields
            if (fields > LimitRequestFields)
            {
                break;
            }

            // valid
            return true;
        }
    }

    // invalid
    if (*message != NULL)
    {
        free(*message);
    }
    *message = NULL;
    *length = 0;
    return false;
}

/**
 * Responds to a client with status code, headers, and body of specified length.
 */
void respond(int code, const char* headers, const char* body, size_t length)
{
    // determine Status-Line's phrase
    // http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.1
    const char* phrase = reason(code);
    if (phrase == NULL)
    {
        return;
    }

    // respond with Status-Line
    if (dprintf(cfd, "HTTP/1.1 %i %s\r\n", code, phrase) < 0)
    {
        return;
    }

    // respond with headers
    if (dprintf(cfd, "%s", headers) < 0)
    {
        return;
    }

    // respond with CRLF
    if (dprintf(cfd, "\r\n") < 0)
    {
        return;
    }

    // respond with body
    if (write(cfd, body, length) == -1)
    {
        return;
    }

    // log response line
    if (code == 200)
    {
        // green
        printf("\033[32m");
    }
    else
    {
        // red
        printf("\033[33m");
    }
    printf("HTTP/1.1 %i %s", code, phrase);
    printf("\033[39m\n");
}

/**
 * Starts server on specified port rooted at path.
 * start is the function that configures the server to listen for connections on a particular TCP port!
 */
void start(short port, const char* path)
{
    // path to server's root
    root = realpath(path, NULL);
    if (root == NULL)
    {
        stop();
    }

    // ensure root is executable
    if (access(root, X_OK) == -1)
    {
        stop();
    }

    // announce root
    printf("\033[33m");
    printf("Using %s for server's root", root);
    printf("\033[39m\n");

    // create a socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        stop();
    }

    // allow reuse of address (to avoid "Address already in use")
    int optval = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // assign name to socket
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("\033[33m");
        printf("Port %i already in use", port);
        printf("\033[39m\n");
        stop();
    }

    // listen for connections
    if (listen(sfd, SOMAXCONN) == -1)
    {
        stop();
    }

    // announce port in use
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    if (getsockname(sfd, (struct sockaddr*) &addr, &addrlen) == -1)
    {
        stop();
    }
    printf("\033[33m");
    printf("Listening on port %i", ntohs(addr.sin_port));
    printf("\033[39m\n");
}

/**
 * Stop server, deallocating any resources.
 * And stop does the opposite, freeing all memory and ultimately compelling the server to exit, without even returning control to main.
 */
void stop(void)
{
    // preserve errno across this function's library calls
    int errsv = errno;

    // announce stop
    printf("\033[33m");
    printf("Stopping server\n");
    printf("\033[39m");

    // free root, which was allocated by realpath
    if (root != NULL)
    {
        free(root);
    }

    // close server socket
    if (sfd != -1)
    {
        close(sfd);
    }

    // stop server
    exit(errsv);
}

/**
 * Transfers file at path with specified type to client.
 * This function’s purpose in life is to transfer a file from the server to a client
 */
void transfer(const char* path, const char* type)
{
    // ensure path is readable
    if (access(path, R_OK) == -1)
    {
        error(403);
        return;
    }

    // open file
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        error(500);
        return;
    }

    // load file's content
    BYTE* content;
    size_t length;
    if (load(file, &content, &length) == false)
    {
        error(500);
        return;
    }

    // close file
    fclose(file);

    // prepare response
    char* template = "Content-Type: %s\r\n";
    char headers[strlen(template) - 2 + strlen(type) + 1];
    if (sprintf(headers, template, type) < 0)
    {
        error(500);
        return;
    }

    // respond with file's content
    respond(200, headers, content, length);

    // free file's content
    free(content);
}

/**
 * URL-decodes string, returning dynamically allocated memory for decoded string
 * that must be deallocated by caller.
 * This function, also named after a PHP function, URL-decodes a string, converting special characters like %20 back to their original values.
 */
char* urldecode(const char* s)
{
    // check whether s is NULL
    if (s == NULL)
    {
        return NULL;
    }

    // allocate enough (zeroed) memory for an undecoded copy of s
    char* t = calloc(strlen(s) + 1, 1);
    if (t == NULL)
    {
        return NULL;
    }
    
    // iterate over characters in s, decoding percent-encoded octets, per
    // https://www.ietf.org/rfc/rfc3986.txt
    for (int i = 0, j = 0, n = strlen(s); i < n; i++, j++)
    {
        if (s[i] == '%' && i < n - 2)
        {
            char octet[3];
            octet[0] = s[i + 1];
            octet[1] = s[i + 2];
            octet[2] = '\0';
            t[j] = (char) strtol(octet, NULL, 16);
            i += 2;
        }
        else if (s[i] == '+')
        {
            t[j] = ' ';
        }
        else
        {
            t[j] = s[i];
        }
    }

    // escaped string
    return t;
}
