#include <iostream>
#include "calc.h"
#include <cstring>

using namespace std;

// Write functions in this order.  Constructor and destructor will be built as the
// functions it invokes are written

Calc::Calc(char *argvIn)
{
    if (!argvIn)
    {
        cout << "ERROR: No expression provided" << endl;
        exit(EXIT_FAILURE);
    }

    int length = strlen(argvIn);
    inFix = new char[length + 1];
    strcpy(inFix, argvIn);

    postFix = new char[length * 2 + 1];
    postFix[0] = '\0';
    stk = new Stack;

    if (!CheckTokens() || !CheckParens())
    {
        cout << "ERROR: Unexpected character found in input" << endl;
        exit(EXIT_FAILURE);
    }

    Parse();
    InFixToPostFix();
}

Calc::~Calc()
{
    delete[] valueTbl;
    delete stk;
    delete[] inFix;
    delete[] postFix;
}

bool Calc::CheckTokens()
{
    int i = 0;
    while (inFix[i] != '\0')
    {
        char ch = inFix[i];
        if (((ch < 'A' || ch > 'Z') && (ch < '0' || ch > '9') && ch != '(' && ch != ')' && ch != '*' && ch != '/' && ch != '+' && ch != '-' && ch != ' '))
        {
            return false;
        }
        i++;
    }
    return true;
}

void Calc::MakeValueTbl()
{
    valueIdx = 0;
    valueTbl = new int[26];
    for (int i = 0; i < 26; i++)
    {
        valueTbl[i] = 0;
    }
}
void Calc::Parse()
{
    MakeValueTbl();
    int length = strlen(inFix);
    char *temp = new char[length + 1];

    int i = 0;
    int j = 0;

    while (inFix[i] != '\0')
    {
        if (inFix[i] == '(' || inFix[i] == ')' || inFix[i] == '+' || inFix[i] == '-' || inFix[i] == '*' || inFix[i] == '/' || (inFix[i] >= 'A' && inFix[i] <= 'Z'))
        {
            temp[j] = inFix[i];
        }
        else if (inFix[i] >= '0' && inFix[i] <= '9')
        {
            int last = FindLast(i);
            int valLength = last - i + 2;
            char *val = new char[valLength];

            int k = 0;
            while (k < valLength - 1)
            {
                val[k] = inFix[i + k];
                k++;
            }
            val[valLength - 1] = '\0';

            valueTbl[valueIdx] = atoi(val);
            temp[j] = (char)('A' + valueIdx);
            valueIdx++;
            i = last;
            delete[] val;
        }
        i++;
        j++;
    }
    temp[j] = '\0';
    int tempLength = j + 1;
    delete[] inFix;
    inFix = new char[tempLength];

    strcpy(inFix, temp);
    delete[] temp;
}

bool Calc::CheckParens()
{
    int i = 0;
    Stack tempstk;

    while (inFix[i] != '\0')
    {
        if (inFix[i] == '(')
        {
            tempstk.Push(inFix[i]);
        }
        else if (inFix[i] == ')')
        {
            if (tempstk.IsEmpty())
                return false;
            tempstk.Pop();
        }
        i++;
    }
    return tempstk.IsEmpty();
}

int Calc::FindLast(int cur)
{
    int i = cur;
    while (inFix[i] >= '0' && inFix[i] <= '9')
    {
        i++;
    }
    return i - 1;
}

void Calc::DisplayInFix()
{
    int i = 0;
    while (inFix[i] != '\0')
    {
        cout << inFix[i];
        i++;
    }
    cout << endl;
}

void Calc::InFixToPostFix()
{
    int postfxIdx = 0;
    int infxIdx = 0;
    char *temp = new char[strlen(inFix) + 1];
    strcpy(temp, inFix);

    while (temp[infxIdx] != '\0')
    {
        char ch = temp[infxIdx];

        if (ch >= 'A' && ch <= 'Z')
        {
            postFix[postfxIdx++] = ch;
        }
        else if (ch == '(')
        {
            stk->Push(ch);
        }
        else if (ch == ')' || ch == '*' || ch == '-' || ch == '/' || ch == '+')
        {
            while (!stk->IsEmpty() && stk->Peek() != '(')
            {
                postFix[postfxIdx++] = stk->Peek();
                stk->Pop();
            }
            if (ch == ')')
            {
                if (!stk->IsEmpty())
                    stk->Pop();
            }
            else
                stk->Push(ch);
        }
        infxIdx++;
    }

    while (!stk->IsEmpty())
    {
        postFix[postfxIdx++] = stk->Peek();
        stk->Pop();
    }
    postFix[postfxIdx] = '\0';
}

void Calc::DisplayPostFix()
{
    int i = 0;
    while (postFix[i] != '\0')
    {
        cout << postFix[i];
        i++;
    }
    cout << endl;
}

int Calc::Evaluate()
{
    int result = 0;
    for (int i = 0; postFix[i] != '\0'; i++)
    {
        char ch = postFix[i];

        if (ch >= 'A' && ch <= 'Z')
        {
            stk->Push(valueTbl[ch - 'A']);
        }
        else
        {
            if (stk->IsEmpty())
            {
                cout << "ERROR:  Stack Underflow" << endl;
                exit(EXIT_FAILURE);
            }
            int A = stk->Peek();
            stk->Pop();

            if (stk->IsEmpty())
            {
                cout << "ERROR:  Stack Underflow" << endl;
                exit(EXIT_FAILURE);
            }
            int B = stk->Peek();
            stk->Pop();

            if (postFix[i] == '+')
            {
                stk->Push(A + B);
            }
            else if (postFix[i] == '-')
            {
                stk->Push(B - A);
            }
            else if (postFix[i] == '*')
            {
                stk->Push(A * B);
            }
            else if (postFix[i] == '/')
            {
                stk->Push(B / A);
            }
        }
    }
    result = stk->Peek();
    return result;
}
