%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s) {
    printf("ERROR\n");
}
%}

%token tMAIL tENDMAIL tSCHEDULE tENDSCHEDULE tSEND tSET tTO tFROM tAT tCOMMA tCOLON tLPR tRPR tLBR tRBR tIDENT tSTRING tADDRESS tDATE tTIME

%%
program:
    | program component
    ;

component:
    mail_block
    | set_statement
    ;

mail_block:
    tMAIL tFROM tADDRESS tCOLON statement_list tENDMAIL
    ;

statement_list:
    | statement_list statement
    ;

statement:
    set_statement
    | send_statement
    | schedule_block
    ;

set_statement:
    tSET tIDENT tLPR tSTRING tRPR
    ;

send_statement:
    tSEND tLBR message tRBR tTO tLBR recipient_list tRBR
    ;

message:
    tIDENT
    | tSTRING
    ;

recipient_list:
    recipient
    | recipient_list tCOMMA recipient
    ;

recipient:
    tLPR tADDRESS tRPR
    | tLPR message tCOMMA tADDRESS tRPR
    ;

schedule_block:
    tSCHEDULE tAT tLBR tDATE tCOMMA tTIME tRBR tCOLON send_statements tENDSCHEDULE
    ;

send_statements:
    send_statement
    | send_statements send_statement
    ;

%%

int main() {
    if(yyparse())
    {
        return 1;
    }
    else
    {
        printf("OK\n");
        return 0;
    }
}
