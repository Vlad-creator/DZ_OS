void skip_letters(char*** s , int* i);
void skip_tabulations(char*** s);
char* strtok_word(char** text);
void skip_string(char*** s , int* i);
char* strtok_string(char** text);

char* strtok_word(char** text)
{
    if ((**text) == '\0')
        return NULL;
    skip_tabulations(&text);
    char* wordstart = *text;
    int i = 0;
    skip_letters(&text , &i);
    char* word = (char*)calloc(sizeof(char) , i);
    for (int j = 0 ; j < i ; j++)
    	word[j] = *(wordstart + j);
    if ((**text) == '\0')
        return wordstart;
    (**text) = '\0';
    (*text)++;
    return word;
}

char* strtok_string(char** text)
{
    if ((**text) == '\0')
        return NULL;
    skip_tabulations(&text);
    char* stringstart = *text;
    int i = 0;
    skip_string(&text , &i);
    char* string = (char*)calloc(sizeof(char) , i + 2);
    for (int j = 0 ; j < i ; j++)
    	string[j] = *(stringstart + j);
    string[i] = '\0';
    if ((**text) == '\0')
        return stringstart;
    (**text) = '\0';
    (*text)++;
    return string;
}

void skip_tabulations(char*** s)
{
    while (((***s) == ' ') || ((***s) == '\n'))
        (**s)++;
}

void skip_string(char*** s , int* i)
{
    while (((***s) != '\n'))
    {
        if ((***s) == '\0')
            break;
        else
        {
        	(*i)++;
            (**s)++;
        };
    }
}

void skip_letters(char*** s , int* i)
{
    while (((***s) != '\n') && ((***s) != ' '))
    {
        if ((***s) == '\0')
            break;
        else
        {
        	(*i)++;
            (**s)++;
        };
    }
}