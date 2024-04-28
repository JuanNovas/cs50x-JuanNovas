#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool valid_key(string key);
string to_cipher(string key, string plaintext);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (valid_key(argv[1]))
    {
        string plaintext = get_string("plaintext: ");
        string ciphertext = to_cipher(argv[1], plaintext);
        printf("ciphertext: %s\n", ciphertext);
        return 0;
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}

bool valid_key(string key)
{
    // checking len
    if (strlen(key) == 26)
    {
        for (int i = 0; key[i] != '\0'; i++)
        {
            if ((key[i] >= 'a' && key[i] <= 'z') || (key[i] >= 'A' && key[i] <= 'Z'))
            {
                key[i] = toupper(key[i]);
            }
            else
            {
                return false;
            }
        }
        // checking duplicates
        for (int i = 0; i < 26; i++)
        {
            for (int x = i + 1; x < 26; x++)
            {
                if (key[i] == key[x])
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

string to_cipher(string key, string plaintext)
{
    int position = 0;

    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            position = plaintext[i] - 97;
            plaintext[i] = key[position];
            plaintext[i] = tolower(plaintext[i]);
        }
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            position = plaintext[i] - 65;
            plaintext[i] = key[position];
            plaintext[i] = toupper(plaintext[i]);
        }
    }

    return plaintext;
}
