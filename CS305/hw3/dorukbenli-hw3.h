#ifndef __DORUKBENLI_HW3_H
#define __DORUKBENLI_HW3_H
#include <stdbool.h>

typedef enum { VARIABLE, STRING, ADDRESS, DATE, TIME } ExprType;

typedef struct VariableNode {
    char* name;
    char* value;
    bool isLocal;
    bool isSet;
    struct VariableNode* next;
} VariableNode;

typedef struct ErrorNode {
    char* errorMessage;
    int lineNumber;
    struct ErrorNode* next;
} ErrorNode;

// AST Node Types
typedef enum {
    NODE_MAILBLOCK,
    NODE_SCHEDULESTATEMENT,
    NODE_SENDSTATEMENT,
    NODE_SETSTATEMENT
} NodeType;

// Forward declaration of struct ASTNode
typedef struct ASTNode ASTNode;

typedef struct {
    char* from;
    ASTNode* statements;
} MailBlock;

typedef struct {
    char* date;
    char* time;
    ASTNode* statements;
} ScheduleStatement;

typedef struct {
    char* message;
    char** recipient;
} SendStatement;

typedef struct {
    char* variable;
    char* value;
} SetStatement;

// General AST node
struct ASTNode {
    NodeType type;
    union {
        MailBlock mail;
        ScheduleStatement schedule;
        SendStatement send;
        SetStatement set;
    } data;
    ASTNode* next;
};

bool checkDateFormat(char *date);
bool checkTimeFormat(const char *time);
void insertError(int lineNumber, const char* message);
void printErrors();
void freeErrorList();

// AST-related functions
ASTNode* createMailBlockNode(char* from);
ASTNode* createScheduleStatementNode(char* date, char* time);
ASTNode* createSendStatementNode(char* message, char* recipient);
ASTNode* createSetStatementNode(char* variable, char* value);
ASTNode* linkASTNodes(ASTNode* first, ASTNode* second);
void generateScheduleNotification(ASTNode* scheduleNode, char* sender);

void freeAST(ASTNode* root);

#endif
