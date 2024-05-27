%{
#ifdef YYDEBUG
  yydebug = 1;

//her bir end mailde, is local olanları sil olmayanları silme böylece teki yeter.
#endif
extern int yylineno;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dorukbenli-hw3.h"
#include <stdbool.h>
void addVariable(const char* name, const char* value, bool isLocal, bool isSet);
bool isVariableDefinedAndSet(char* name);
void processMailBlock(ASTNode* mailBlockNode);
void processScheduleStatement(ASTNode* scheduleNode, char* sender);
void generateSendNotification(ASTNode* sendNode, char* sender);
void clearLocalVariables();
char* findVariableValue(const char* varName);
bool isLocal = false;
void yyerror (const char *msg){return; }
VariableNode* symbolTableHead = NULL;
ErrorNode* errorHead = NULL;
ASTNode* astRoot = NULL;
%}


%union{
    char* strVal;
    VariableNode* variable;
    ASTNode* astNode;
}

%token tMAIL tENDMAIL tSCHEDULE tENDSCHEDULE tSEND tTO tFROM tSET tCOMMA tCOLON tLPR tRPR tLBR tRBR tAT
%token <strVal> tSTRING 
%token <strVal> tIDENT    
%token <strVal> tADDRESS  
%token <strVal> tDATE  
%token <strVal> tTIME   
%type <astNode> mailBlock statementList sendStatements scheduleStatement setStatement sendStatement
%type <strVal>  recipient recipientList option //not sure about recipientList, i probably need another type for this, check later

%start program
%% /* Grammar rules and actions are as follow */

program : statements
;

statements : 
            | setStatement statements
            | mailBlock statements
;

mailBlock : tMAIL tFROM tADDRESS tCOLON statementList tENDMAIL
{
    ASTNode* mailNode = createMailBlockNode($3);
    if (mailNode != NULL) {
        mailNode->data.mail.statements = $5;
    }

    if (astRoot == NULL) {
        astRoot = mailNode;
    } else {
        linkASTNodes(astRoot, mailNode);
    }
    
    isLocal = false; // Back to global scope
    clearLocalVariables(); // Clear local variables
};




statementList : 
{
    $$ = NULL;
}
| setStatement statementList
{
    $$ = linkASTNodes($1, $2);
}
| sendStatement statementList
{
    $$ = linkASTNodes($1, $2);
}
| scheduleStatement statementList
{
    $$ = linkASTNodes($1, $2);
};

sendStatements : sendStatement
{
    $$ = $1;
}| sendStatement sendStatements
{
    $$ = linkASTNodes($1, $2); 
};



sendStatement : tSEND tLBR option tRBR tTO tLBR recipientList tRBR
{
    char *str = $3;
    if (str != NULL && str[0] != '"' && !isVariableDefinedAndSet(str)) {
        int msg_size = snprintf(NULL, 0, "%s is undefined", str);
        char *error_message = (char *)malloc(msg_size + 1);
        if (error_message != NULL) {
            sprintf(error_message, "%s is undefined", str);
            insertError(yylineno, error_message);
        }
    }
    else if (str != NULL) {
        $$ = createSendStatementNode($3, $7);
    }
};


option: tSTRING { $$ = $1; }
      | tIDENT  { $$ = findVariableValue($1);};


recipientList : recipient
            | recipient tCOMMA recipientList
;

recipient : tLPR tADDRESS tRPR{ $$ = strdup($2);}
            | tLPR tSTRING tCOMMA tADDRESS tRPR
            {
                $$ = malloc(strlen($2) + strlen($4) + 1); 
                strcpy($$, $2);
                strcat($$, $4);
            }
            | tLPR tIDENT tCOMMA tADDRESS tRPR
            {
                $$ = malloc(strlen($2) + strlen($4) + 1);
                strcpy($$, $2);
                strcat($$, $4);
            }
;

scheduleStatement : tSCHEDULE tAT tLBR tDATE tCOMMA tTIME tRBR tCOLON sendStatements tENDSCHEDULE
{
    bool dateValid = checkDateFormat($4);
    bool timeValid = checkTimeFormat($6);

    if (!dateValid || !timeValid) {
        if (!dateValid) {
            char *error_message;
            int msg_size = snprintf(NULL, 0, "ERROR at line %d: date object is not correct (%s)", yylineno, $4);
            error_message = (char*)malloc(msg_size + 1);
            sprintf(error_message, "ERROR at line %d: date object is not correct (%s)", yylineno, $4);
            insertError(yylineno, error_message);
        }
        if (!timeValid) {
            char *error_message;
            int msg_size = snprintf(NULL, 0, "ERROR at line %d: time object is not correct (%s)", yylineno, $6);
            error_message = (char*)malloc(msg_size + 1);
            sprintf(error_message, "ERROR at line %d: time object is not correct (%s)", yylineno, $6);
            insertError(yylineno, error_message);
        }
    } else {
        // If both date and time are valid then create a schedule statement node
        $$ = createScheduleStatementNode($4, $6);
        if ($$ != NULL) {
            $$->data.schedule.statements = $9;
        }
    }
};


setStatement : tSET tIDENT tLPR tSTRING tRPR
{
    addVariable($2, $4, isLocal, true);
    $$ = createSetStatementNode($2, $4);
};


%%
void insertError(int lineNumber, const char* message) {
    ErrorNode* new_node = (ErrorNode*)malloc(sizeof(ErrorNode));
    new_node->lineNumber = lineNumber;
    new_node->errorMessage = strdup(message);
    new_node->next = NULL;

    if (errorHead == NULL) {
        errorHead = new_node;
    } else {
        ErrorNode* current = errorHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

ASTNode* linkASTNodes(ASTNode* first, ASTNode* second) {
    if (first == NULL) {
        return second;
    }

    ASTNode* current = first;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = second;
    
    return first;
}

//AST NODE CREATION FUNCTIONS

ASTNode* createMailBlockNode(char* from) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node != NULL) {
        node->type = NODE_MAILBLOCK;
        node->data.mail.from = strdup(from);
        node->data.mail.statements = NULL;
        node->next = NULL;
    }
    return node;
}


ASTNode* createScheduleStatementNode(char* date, char* time) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node != NULL) {
        node->type = NODE_SCHEDULESTATEMENT;
        node->data.schedule.date = strdup(date);
        node->data.schedule.time = strdup(time);
        node->data.schedule.statements = NULL;
        node->next = NULL;
    }
    return node;
}

ASTNode* createSendStatementNode(char* message, char* recipient) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node != NULL) {
        node->type = NODE_SENDSTATEMENT;
        node->data.send.message = strdup(message);

        // Allocate space for an array of two pointers (recipient and NULL)
        node->data.send.recipient = malloc(2 * sizeof(char*));
        if (node->data.send.recipient != NULL) {
            node->data.send.recipient[0] = strdup(recipient); // Duplicate and store the recipient
            node->data.send.recipient[1] = NULL;             // End of the list
        }

        node->next = NULL;
    }
    return node;
}

ASTNode* createSetStatementNode(char* variable, char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node != NULL) {
        node->type = NODE_SETSTATEMENT;
        node->data.set.variable = strdup(variable);
        node->data.set.value = strdup(value);
        node->next = NULL;
    }
    return node;
}

//AST PROCESSING FUNCTIONS
void traverseASTAndGenerateNotifications(ASTNode* root) {
    if (root == NULL) return;
    switch (root->type) {
        case NODE_MAILBLOCK:
            processMailBlock(root);
            break;
    }

    traverseASTAndGenerateNotifications(root->next);
}

void processMailBlock(ASTNode* mailBlockNode) {
    char* sender = mailBlockNode->data.mail.from;

    ASTNode* statement = mailBlockNode->data.mail.statements;
    while (statement != NULL) {
        switch (statement->type) {
            case NODE_SENDSTATEMENT:
                generateSendNotification(statement, sender);
                break;
            case NODE_SCHEDULESTATEMENT:
                generateScheduleNotification(statement, sender);
                break;
        }
        statement = statement->next;
    }
}


//helper
char* findVariableValue(const char* varName) {
    VariableNode* current = symbolTableHead;
    while (current != NULL) {
        if (strcmp(current->name, varName) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; 
}


void formatDate(const char* date, char* formattedDate) {
    // Month names array
    char* months[] = {"January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December"};

    // Variables to store day, month, and year
    int day, month, year;

    // Parse the input date string
    sscanf(date, "%2d/%2d/%4d", &day, &month, &year);

    // Format the date
    sprintf(formattedDate, "%s %d, %d", months[month - 1], day, year);
}



//NOTIFICATIONS
void generateSendNotification(ASTNode* sendNode, char* sender) {
    char* message = sendNode->data.send.message;
    char** recipients = sendNode->data.send.recipient;

    int i = 0;
    while (recipients[i] != NULL) {
        printf("E-mail sent from %s to %s: %s\n", sender, recipients[i], message);
        i++;
    }
}



void generateScheduleNotification(ASTNode* scheduleNode, char* sender) {
    char* date = scheduleNode->data.schedule.date;
    char* time = scheduleNode->data.schedule.time;
    ASTNode* sendStatementNode = scheduleNode->data.schedule.statements;

    while (sendStatementNode != NULL) {
        if (sendStatementNode->type == NODE_SENDSTATEMENT) {
            char* message = sendStatementNode->data.send.message;
            char** recipients = sendStatementNode->data.send.recipient;

            int i = 0;
            while (recipients[i] != NULL) {
                char formattedDate[50];
                formatDate(date, formattedDate);
                //sort the linked list
                
                printf("E-mail scheduled to be sent from %s on %s, %s to %s: %s\n", 
                       sender, formattedDate, time, recipients[i], message);
                i++;
            }
        }
        sendStatementNode = sendStatementNode->next;
    }
}


//Function for adding variables to variable list to keep the track of setted/created vars.
void addVariable(const char* name, const char* value, bool isLocal, bool isSet) {
    if (name == NULL) {
        return;
    }

    VariableNode* new_var = (VariableNode*)malloc(sizeof(VariableNode));

    if (new_var == NULL) {
        printf("Allocation Error\n");
        return;
    }
    new_var->name = strdup(name);

    if (new_var->name == NULL) {
        free(new_var);
        return;
    }
    new_var->value = (value != NULL) ? strdup(value) : NULL;
    new_var->isLocal = isLocal;
    new_var->isSet = isSet;
    new_var->next = symbolTableHead;
    symbolTableHead = new_var;
}



void freeErrorList() {
    ErrorNode* current = errorHead;
    while (current != NULL) {
        ErrorNode* next = current->next;
        free(current->errorMessage);
        free(current);
        current = next;
    }
    errorHead = NULL;
}


bool check_and_print_errors() {
    if (errorHead != NULL) {
        printf("inside if");
        ErrorNode* ptr = errorHead;
        while (ptr != NULL) {
            printf("ERROR at line%d: %s\n", ptr->lineNumber, ptr->errorMessage);
            ptr = ptr->next;
        }
        return true;
    }
    return false;
}


bool isVariableDefinedAndSet(char* name) {
    VariableNode* current = symbolTableHead;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->isSet) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void clearLocalVariables() {
    VariableNode *current = symbolTableHead;
    VariableNode *prev = NULL;

    while (current != NULL) {
        if (current->isLocal) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                symbolTableHead = current->next;
            }

            VariableNode *temp = current;
            current = current->next;

            free(temp->name);
            free(temp->value);
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool checkDateFormat(char * date)
{
    int day, month, year;
    sscanf(date, "%2d%*[^0-9]%2d%*[^0-9]%4d", &day, &month, &year);
    if (month < 1 || month > 12){
        return false;
    }
    int daysInMonth[] = {31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day < 1 || day > daysInMonth[month - 1])
    {
        return false;
    }
    return true;
}

bool checkTimeFormat(const char *time) {
    int hours, minutes;
    if (sscanf(time, "%2d:%2d", &hours, &minutes) != 2) {
        return false;
    }
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        return false;
    }
    return true;
}


void clearSymbolTable() {
    VariableNode* current = symbolTableHead;
    while (current != NULL) {
        VariableNode* next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
    symbolTableHead = NULL;
}

void freeAST(ASTNode* root) {
    while (root != NULL) {
        ASTNode* temp = root;
        root = root->next;
        switch (temp->type) {
            case NODE_MAILBLOCK:
                free(temp->data.mail.from);
                freeAST(temp->data.mail.statements);
                break;
            case NODE_SCHEDULESTATEMENT:
                free(temp->data.schedule.date);
                free(temp->data.schedule.time);
                freeAST(temp->data.schedule.statements);
                break;
            case NODE_SENDSTATEMENT:
                free(temp->data.send.message);
                if (temp->data.send.recipient) {
                    int i = 0;
                    while (temp->data.send.recipient[i] != NULL) {
                        free(temp->data.send.recipient[i]);
                        i++;
                    }
                    free(temp->data.send.recipient);
                }
                break;
            case NODE_SETSTATEMENT:
                free(temp->data.set.variable);
                free(temp->data.set.value);
                break;
        }
        free(temp);
    }
}


void execute_script() {
    if (errorHead != NULL) {
        check_and_print_errors();
    } else {
        traverseASTAndGenerateNotifications(astRoot);
    }
}



int main() {
    yylineno = 1;
    if (yyparse()) {
        printf("ERROR.\n");
        return 1;
    } else {
        execute_script();
    }
    freeErrorList();
    clearSymbolTable();
    freeAST(astRoot);
    return 0;
}
