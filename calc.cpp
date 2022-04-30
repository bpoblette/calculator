#include <iostream>
using namespace std;

#include "calc.h"
#include <cstring>


//Write functions in this order.  Constructor and destructor will be built as the
//functions it invokes are written

Calc::Calc(char* argvIn)
{
    int length = strlen(argvIn);
    inFix = new char[length+1];
    strcpy(inFix, argvIn);
    stk = new Stack;

    if(!CheckTokens())
    {
        cout << "ERROR: Unexpected character found in input" << endl;
        exit(EXIT_FAILURE);
    }
    if(!CheckParens())
    {
        cout << "ERROR: Unexpected character found in input" << endl;
        exit(EXIT_FAILURE);
    }
    Parse();
}


Calc::~Calc()
{
    delete valueTbl;
    delete stk;
    delete inFix;
    delete postFix;
}

bool Calc::CheckTokens()
{
    int i = 0;
    while (inFix[i] != '\0')
    {
        char ch = inFix[i];
        if(((ch < 'A' || ch > 'Z') && (ch < '0' ||  ch > '9') && ch != '(' && ch != ')' && ch != '*' && ch != '/' && ch != '+' && ch != '-' && ch != ' ')) 
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
   char* temp = new char[length+1];

   int i =0;
   int j = 0;

   while (inFix[i] != '\0')
   {
       if (inFix[i] == '(' || inFix[i] == ')' || inFix[i] == '+' || inFix[i] == '-' || inFix[i] == '*' || inFix[i] == '/' || (inFix[i] >= 'A' && inFix[i] <= 'Z'))
       {
           temp[j] = inFix[i];
       }
       else
       {
           int last = FindLast(i);
           int valLength = last - i + 2;
           char* val = new char[valLength];
           val[valLength - 1] = '\0';

           int k = 0;
           while (k < valLength - 1)
           {
               val[k] = inFix[i + k];
               k++;
           }

            valueTbl[valueIdx] = atoi(val);
            temp[j] = (char)('A' + valueIdx);
            valueIdx++;
            i = last;
            delete val;
       }
       i++;
       j++;
   }
   temp[j] = '\0';
   int tempLength = j + 1;
   delete inFix;
   inFix = new char[tempLength];
   int count = 0;
   while (temp[count] != '\0')
   {
       inFix[count] = temp[count];
       count++;
   }
   inFix[count] = '\0';
   delete temp;

}
bool Calc::CheckParens()
{
    int i = 0;
    if (inFix[i] != '(')
    {
        return false;
    }
    
    while (inFix[i] != '\0')
    {
        if (inFix[i] == '(')
        {
            stk->Push(inFix[i]);
        }
        else if(inFix[i] == ')')
        {
            stk->Pop();
        }
        i++;
    }
    return stk->IsEmpty();
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
    char* temp = new char[strlen(postFix)];
    int postfxIdx = 0;
    int infxIdx = 0;
    while (temp[infxIdx] != '\0') {

        if (temp[infxIdx] >= 'A' && temp[infxIdx] <= 'Z') {
            postFix[postfxIdx] = temp[infxIdx];
            postfxIdx++;
        } else if (temp[infxIdx] == '(' || temp[infxIdx] == '*' || temp[infxIdx] == '-'|| temp[infxIdx] == '/' || temp[infxIdx] == '+') {
            stk->Push(temp[infxIdx]);
        } else {
            while (stk->Peek() != '\0') {
                postFix[postfxIdx] = stk->Peek();
                postfxIdx++;
                stk->Pop();
            }
            stk->Pop();
        }
        infxIdx++;
    }
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
    int A = 0;
    int B = 0;
    int result = 0;
    for (int i = 0; postFix[i] != '\0'; i++)
    {
        if (postFix[i] >= 'A' && postFix[i] <= 'Z')
        {
            stk->Push(valueTbl[postFix[i] - 'A']);
        }
        else
        {
            A = stk->Peek();
            stk->Pop();
            B = stk->Peek();
            stk->Pop();
            if (postFix[i] == '+')
            {
                stk->Push(A + B);
            }
            if (postFix[i] == '-')
            {
                stk->Push(B - A);
            }
            if (postFix[i] == '*')
            {
                stk->Push(A * B);
            }
            if (postFix[i] == '/')
            {
                stk->Push(B / A);
            }
        }
    }
    result = stk->Peek();
    return result;
}
