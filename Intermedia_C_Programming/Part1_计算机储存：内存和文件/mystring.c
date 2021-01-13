#include "mystring.h"
#include <ctype.h>
int my_strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}
int my_countchar(const char *str, char ch)
{
    /*my code
    int counter = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
        {
            counter++;
        }
        return counter;
    }   
    */
    int counter = 0;
    while (*str != '\0')
    {
        if (*str == ch)
        {
            counter++;
        }
        str++;
    }
    return counter;
}
void my_strupper(char *str)
{
    while (*str != '\0')
    {
        *str = toupper(*str);
        str++;
    }
}
char *my_strchr(const char *str, char ch)
{
    int ind = 0;
    while (str[ind] != '\0')
    {
        if (str[ind] == ch)
        {
            return (&str[ind]);
        }
        ind++;
    }
}