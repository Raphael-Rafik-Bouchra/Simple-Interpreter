#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MaxSize 1000

///------------------------------------[INFIX TO POSTFIX RELATED FUNCTIONS]--------------------------------------------------
typedef union
{
    float floatData;
    char charData;
    int intData;
} Item;

typedef struct
{
    Item items[MaxSize];
    int top;
} Stack;

Stack* Initialize()
{
    Stack* s=(Stack*)malloc(sizeof(Stack));
    if(!s)
        return NULL;
    s->top = 0;
    return s;
}
void Destruct(Stack* s)
{
    free(s);
}

void Push(Stack* s, Item x )
{
    s->items[s->top++] = x;
}

Item Pop(Stack* s)
{
    s->top--;
    return  s->items[s->top];

}

int isEmpty(Stack* s)
{
    if(s->top == 0)
        return 1;
    else
        return 0;
}

int isFull(Stack* s)
{
    if(s->top == MaxSize)
        return 1;

    else
        return 0;
}

Item Top(Stack* s)
{
    return s->items[s->top-1];
}

int isOperator(char x)
{
    if(x == '+' || x == '-' || x == '*' || x == '/' || x == '^')
        return 1;
    else
        return 0;
}

int precedence(char x)
{
    switch(x)
    {
    case '^':
        return 3;
    case '*':
    case '/':
    case '%':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }

}

int isOperand(char x)
{
    if((x >='a' && x <='z') || (x >= 'A' && x <= 'Z'))
        return 1;
    else
        return 0;
}


int isNumeric (char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

float evaluate(char x, float op1, float op2)
{
    if(x == '^')
        return (pow(op1, op2));
    else if(x == '*')
        return (op1*op2);
    else if(x == '/')
        return (op1/op2);
    else if(x == '%')
        return (((int)op1)%((int)op2));
    else if(x == '+')
        return (op1+op2);
    else if(x == '-')
        return (op1-op2);
}


void infixToPostfix(char* infix, char* postfix)
{
    int i=0;
    Stack* oStack = Initialize();

    char* token = strtok(infix, " ");

    if(isOperand(*token) || isNumeric(token))
    {
        strcpy(postfix, token);
        i=strlen(token);
    }

    else if((*token) == '(')
    {
        Item item1;
        item1.charData='(';
        Push(oStack, item1);
    }
    else    //Infix Validation
    {
        printf("Invalid Infix expression\n");
        exit(-1);
    }

    token = strtok(NULL, " ");

    while(token != NULL)
    {

        if(isOperand(*token) || isNumeric(token))
        {
            strcat(postfix, " ");
            i++;
            strcat(postfix, token);
            i+=strlen(token);
        }

        else if((*token) == '(')
        {
            Item item2;
            item2.charData='(';
            Push(oStack, item2);
        }
        else if((*token) == ')')
        {

            while(!isEmpty(oStack) && Top(oStack).charData != '(')
            {
                postfix[i++]=' ';
                postfix[i++]=Pop(oStack).charData;
            }
            if(isEmpty(oStack))
            {
                printf("Invalid Infix Expression\n");
                exit(-1);
            }
            Pop(oStack);

        }

        else if(isOperator(*token))
        {
            while(!isEmpty(oStack) && (precedence(*token)<=precedence(Top(oStack).charData)) && (Top(oStack).charData != '('))
            {
                postfix[i++]=' ';
                postfix[i++]=Pop(oStack).charData;

            }
            Item item4;
            item4.charData=(*token);
            Push(oStack, item4);
        }


        token = strtok(NULL, " ");


    }



    while(!isEmpty(oStack))
    {
        postfix[i++]=' ';
        postfix[i++]=Pop(oStack).charData;

    }
    Destruct(oStack);

}
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
float evaluatePostfix(char* postfix)
{

    Stack* eStack = Initialize();
    float op1,op2,value;

    char* token = strtok(postfix, " ");
    while(token != NULL)
    {
        if(isNumeric(token))
        {
            Item item1;
            item1.floatData=atof(token);
            Push(eStack, item1);
        }
        else if(isOperator(*token))
        {
            op2=Pop(eStack).floatData;
            op1=Pop(eStack).floatData;
            value=evaluate((*token), op1, op2);
            Item item2;
            item2.floatData=value;
            Push(eStack, item2);
        }
        token = strtok(NULL, " ");
    }
    value=Pop(eStack).floatData;
    Destruct(eStack);
    return value;
}
///------------------------------------[END OF INFIX TO POSTFIX RELATED FUNCTIONS]--------------------------------------------------

///----------------------------------------------[FILE RELATED FUNCTIONS]-----------------------------------------------------------
int isAlphaOrNum(char x)
{
    if(x>='0' && x<='9')
        return 1;
    if(x>='a' && x<='z')
        return 1;
    if(x>='A' && x<='Z')
        return 1;
    if(x=='.')
        return 1;
    if(isspace(x))
        return 1;
    return 0;
}
int isOperatorOrBrackets(char x)
{
    if (x=='+' || x=='-' ||x=='*' || x=='/' || x=='^' || x=='(' ||x==')' ||x=='%')
        return 1;
    return 0;
}
void addSpace(char x[])
{
    char token[255];
    int i=0,j=0;
    if (x[i]=='(')
    {
        token[j++]=x[i++];
        token[j++]=' ';

    }
    else
        token[j++]=x[i++];
    while(x[i]!='\0')
    {
        if (isAlphaOrNum(x[i]))
        {
            if(i>=2)
            {
                if (x[i-1]=='-'&& isOperatorOrBrackets(x[i-2]) && x[i-2]!=')')
                {
                    token[--j]=x[i++];
                    j++;
                }

                else
                    token[j++]=x[i++];
            }
            else
                token[j++]=x[i++];
        }
        else
        {
            if(isOperatorOrBrackets(x[i]))
            {
                if(isOperatorOrBrackets(x[i-1]))
                {
                    token[j++]=x[i++];
                    token[j++]=' ';

                }
                else
                {
                    token[j++]=' ';
                    token[j++]=x[i++];
                    token[j++]=' ';
                }
            }

        }



    }
    if (token[j-1]==' ')
        token[j-1]='\0';
    else
        token[j]='\0';
    i=0;
    while(token[i]!='\0')
    {
        x[i]=token[i];
        i++;
    }
    x[i]='\0';


}
void assign_variables_values(char* variables, int variables_length, float* values, int values_length, char* postfix)
{
    char temp[255]="";
    char* token = strtok(postfix, " ");
    while(token != NULL)
    {
        if(isOperand(*token))
        {
            int i=0;
            for(i; i < variables_length; i++)
            {
                if((*token) == variables[i])
                {
                    break;
                }
            }
            if(i == variables_length)
            {
                puts("Invalid Expression");
                exit(-1);
            }
            else
            {
                char value[255]="";
                if(i >= values_length)
                {
                    puts("Invalid Expression");
                    exit(-1);
                }
                else
                {
                    ftoa(values[i], value, 2);
                    strcat(temp, value);
                    strcat(temp, " ");
                }
            }

        }
        else
        {
            strcat(temp, token);
            strcat(temp, " ");
        }
        token = strtok(NULL, " ");
    }
    strcpy(postfix, temp);
    printf("%s      [ASSIGNING VARIABLES VALUES]\n",temp);
}
int is_expression_space_delimited()
{
    int answer = 0;
    do
    {
        puts("Is The Expressions are space delimited?");
        puts("  1- YES              EX: ( X + Y ) * 5");
        puts("  2- NO               EX: (X+Y)*5");
        printf("Enter Your Choice --> ");
        scanf("%d",&answer);
    }
    while((answer != 1) && (answer != 2) );
    puts("---------------------------------------------------------");
    printf("\n\n");
    if(answer == 1)
        return 1;
    else
        return 0;

}
int check_parentheses(char expression[])
{
    int r=0;
    int l=0;
    int i=0;
    while(expression[i] != NULL)
    {
        if(expression[i] == '(')
            r++;
        else if(expression[i] == ')')
            l++;
        i++;
    }
    if(r == l)
        return 1;
    else
        return 0;
}
void load_variables_values(char* source, char* variables, float* values)
{
    FILE * fp = fopen(source, "r");
    char buff[255]="";
    int index=0;
    int index2=0;
    int flag=0;
    int foundIndex =0;
    if (fp)
    {
        puts("--------------File Loaded Successfully------------------!");
    }

    else
    {
        puts("File Not Found!");
        puts("----------------------------------");
        exit(-1);
    }
    int answer = is_expression_space_delimited();
    puts("Converting EXPRESSIONS FROM INFIX TO POSTFIX & Evaluation");

    while(!feof(fp))
    {
        flag=0;
        char infix[255]="";
        char postfix[255]="";
        fgets(buff, 255, fp);
        buff[strlen(buff) - 1] = '\0';
        char* token = strtok(buff, " ");
        if(isOperand(*token))
        {
            int i;
            for(i=0; i < index; i++)
            {
                if((*token) == variables[i])
                {
                    flag=1;
                    foundIndex = i;
                    break;
                }
            }
            if(flag == 0)
            {
                variables[index] = *token;
                index++;
            }
        }
        else
        {
            puts("Invalid Expression");
            exit(-1);
        }

        token = strtok(NULL, " ");
        if(strcmp(token, "=") != 0)
        {
            puts("Invalid Expression");
            exit(-1);
        }

        token = strtok(NULL,"");
        if(check_parentheses(token) == 1)
        {
            if(flag == 1)
            {
                strcpy(infix, token);
                if(answer == 0)
                    addSpace(infix);
                puts("---------------------------------------------------------");
                puts("***Duplication found***");
                printf("Old value of %c = %.2f\n",variables[foundIndex],values[foundIndex]);
                puts("Calculating new value");
                printf("%c = %s      [INFIX]\n",variables[foundIndex], infix);
                infixToPostfix(infix, postfix);
                printf("%c = %s      [POSTFIX]\n",variables[foundIndex], postfix);

                printf("%c = ",variables[foundIndex]);
                assign_variables_values(variables, index, values, index2, postfix);
                values[foundIndex] = evaluatePostfix(postfix);
                printf("New value of %c = %.2f      [CALCULATING EQUATION]\n",variables[foundIndex], values[foundIndex]);

            }
            else if(flag == 0)
            {
                strcpy(infix, token);
                if(answer == 0)
                    addSpace(infix);
                puts("---------------------------------------------------------");
                printf("%c = %s      [INFIX]\n",variables[index - 1], infix);
                infixToPostfix(infix, postfix);
                printf("%c = %s      [POSTFIX]\n",variables[index - 1], postfix);

                printf("%c = ",variables[index - 1]);
                assign_variables_values(variables, index, values, index2, postfix);
                values[index2] = evaluatePostfix(postfix);
                printf("%c = %.2f      [CALCULATING EQUATION]\n",variables[index - 1], values[index2]);

                index2++;
            }
        }
        else
        {
            puts("Invalid Expression");
            exit(-1);
        }
    }
    puts("---------------------------------------------------------");
    printf("\n\n");

}
///----------------------------------------------[END OF FILE RELATED FUNCTIONS]----------------------------------------------------

///--------------------------------------------------[BST RELATED FUNCTIONS]--------------------------------------------------------
typedef struct
{
    char* key;
    float value;
} Data;
typedef struct
{
    Data* data;
    struct Node* left,*right;
} Node;

Node* constructNode(char* str, float value)
{
    Node* p=malloc(sizeof( Node));
    p->data = (Data*)malloc(sizeof(Data));
    p->data->key=(char*)malloc(strlen(str)+1);
    strcpy(p->data->key,str);
    p->data->value = value;
    p->left=p->right=NULL;
    return p;
}
void freeNode(Node *node)
{
    free(node->data->key);
    free(node);
}
int count(Node *node)
{
    if (node)
        return 1+count(node->left)+count(node->right);
    return 0;
}
Node* search(Node* root, char *key)
{
    while (root!=NULL)
    {
        if (strcmpi(key, root->data->key) == 0)
        {
            return root;
        }
        else if (strcmpi(key, root->data->key) > 0)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;


}
Node* put(Node* node, Data data)
{
    Node* root = search(node, data.key);
    if(root == NULL)
    {
        if (node == NULL)
            return(constructNode(data.key, data.value));

        if (strcmpi(data.key,node->data->key)<0)
            node->left  = put(node->left, data);
        else if (strcmpi(data.key,node->data->key)>0)
            node->right = put(node->right, data);
    }
    else
    {
        root->data->value = data.value;
    }
    // Equal keys are not allowed in BST
    return node;
}

Node* load_data_into_BST(char* variables, float* values)
{
    int i;
    Data *datas = (Data*)malloc(sizeof(Data)*strlen(variables));
    Node* root = NULL;
    for(i=0; i<strlen(variables); i++)
    {
        datas[i].key = (char*)malloc(strlen(&variables[i]) + 1);
        char* var = &variables[i];
        strcpy(datas[i].key, var);
        datas[i].value = values[i];
        root = put(root, datas[i]);
    }
    return root;
}
void printInorder(Node* node)
{
    if (node == NULL)
        return;

    /* first recur on left child */
    printInorder(node->left);

    /* then print the data of node */
    printf("%c = %.2f\n",*node->data->key, node->data->value);

    /* now recur on right child */
    printInorder(node->right);
}
///-----------------------------------------------[END OF BST RELATED FUNCTIONS]-----------------------------------------------------

///--------------------------------------------------[Heap RELATED FUNCTIONS]--------------------------------------------------------
void swap(Data* data1, Data* data2)
{
    Data temp;
    temp = *data1;
    *data1 = *data2;
    *data2 = temp;
}

void heapify(Data arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2*i + 1; // left = 2*i + 1
    int r = 2*i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l].value > arr[largest].value)
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r].value > arr[largest].value)
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void heapSort(Data arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i=n-1; i>0; i--)
    {
        // Move current root to end
        swap(&arr[0], &arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}
void printArray(Data arr[], int n)
{
    for (int i=0; i<n; ++i)
        printf("%c = %.2f\n",*arr[i].key,arr[i].value);
}
void load_data_into_heap(char* variables, float* values)
{
    int i;
    Data *datas = (Data*)malloc(sizeof(Data)*strlen(variables));
    for(i=0; i<strlen(variables); i++)
    {
        datas[i].key = (char*)malloc(strlen(&variables[i]) + 1);
        char* var = &variables[i];
        strcpy(datas[i].key, var);
        datas[i].value = values[i];
    }
    heapSort(datas, i);
    printArray(datas, i);
}
///-----------------------------------------------[End Of Heap RELATED FUNCTIONS]----------------------------------------------------

int main()
{
    char variables[255] = "";
    float values[255];
    load_variables_values("Src.txt", variables, values); //This function load the variables and their values

    Node *root;
    root = load_data_into_BST(variables, values);        //Loads data into a binary search tree
    puts("      [Sorted By Variable Name (BST)]");
    puts("---------------------------------------------------------");
    printInorder(root);                                  //Print BST In Order
    puts("---------------------------------------------------------");
    printf("\n\n");
    puts("      [Sorted By Value (Heap Sort)]");
    puts("---------------------------------------------------------");
    load_data_into_heap(variables, values);              //Loads data into heap and display it sorted by value
    puts("---------------------------------------------------------");
}
