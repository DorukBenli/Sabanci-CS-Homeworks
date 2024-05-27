%{
#ifdef YYDEBUG
  yydebug = 1;
#endif
#include <stdio.h>
void yyerror (const char *msg) /* Called by yyparse on error */ {return; }
%}
%token tMAIL tENDMAIL tSCHEDULE tENDSCHEDULE tSEND tTO tFROM tSET tCOMMA tCOLON tLPR tRPR tLBR tRBR tAT tSTRING tIDENT tADDRESS tDATE tTIME
%start program
%%

program : statements
;

statements : 
            | setStatement statements
            | mailBlock statements
;

mailBlock : tMAIL tFROM tADDRESS tCOLON statementList tENDMAIL
;

statementList : 
                | setStatement statementList
                | sendStatement statementList
                | scheduleStatement statementList
;

sendStatements : sendStatement
                | sendStatement sendStatements 
;

sendStatement : tSEND tLBR option tRBR tTO tLBR recipientList tRBR
;

option: tSTRING | tIDENT
;


recipientList : recipient
            | recipient tCOMMA recipientList
;

recipient : tLPR tADDRESS tRPR
            | tLPR tSTRING tCOMMA tADDRESS tRPR
            | tLPR tIDENT tCOMMA tADDRESS tRPR
;

scheduleStatement : tSCHEDULE tAT tLBR tDATE tCOMMA tTIME tRBR tCOLON sendStatements tENDSCHEDULE
;

setStatement : tSET tIDENT tLPR tSTRING tRPR
;


%%
int main () 
{
   if (yyparse())
   {
      printf("ERROR\n");
      return 1;
    } 
    else 
    {
        printf("OK\n");
        return 0;
    } 
}