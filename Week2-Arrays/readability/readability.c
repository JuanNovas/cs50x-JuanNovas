#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float l = letters / (words / 100.0);
    float s = sentences / (words / 100.0);

    float grade = 0.0588 * l - 0.296 * s - 15.8;
    grade = round(grade);
    int grade_int = grade / 10 * 10;

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade_int);
    }
}

int count_letters(string text)
{
    int len = strlen(text);
    int letters = 0;
    for (int i = 0; i <= len; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int len = strlen(text);
    int words = 1;
    for (int i = 0; i <= len; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int len = strlen(text);
    int sentences = 0;
    for (int i = 0; i <= len; i++)
    {
        if ((text[i] == '.') || (text[i] == '?') || (text[i] == '!'))
        {
            sentences++;
        }
    }
    return sentences;
}
