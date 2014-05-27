#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "asmt6.h"

#define makeString(P, S) \
  (P) = malloc(sizeof(char) * 16); \
  strcpy((P), (S));

numV *interp(void *e);

numV* evalBinop (binopC *binop) {
   numV *numV1, *numV2, *val = malloc(sizeof(numV));
   char *msg = malloc(5);
   strcpy(msg, "numV");

   val->type = msg;
   if (strcmp(binop->operator, "+") == 0) {
      numV1 = interp(binop->bozor1);
      numV2 = interp(binop->bozor2);
      val->numVal = numV1->numVal + numV2->numVal;
      //note, must eval these binops later
   }
   else if (strcmp(binop->operator, "-") == 0) {
      numV1 = interp(binop->bozor1);
      numV2 = interp(binop->bozor2);
      val->numVal = numV1->numVal - numV2->numVal;
   }
   else if (strcmp(binop->operator, "*") == 0) {
      numV1 = interp(binop->bozor1);
      numV2 = interp(binop->bozor2);
      val->numVal = numV1->numVal * numV2->numVal;
   }
   else if (strcmp(binop->operator, "/") == 0) {
      numV1 = interp(binop->bozor1);
      numV2 = interp(binop->bozor2);
      val->numVal = numV1->numVal / numV2->numVal;
   }
   else if (strcmp(binop->operator, "<=") == 0) {
      numV1 = interp(binop->bozor1);
      numV2 = interp(binop->bozor2);
      val->numVal = numV1->numVal <= numV2->numVal;

   }
   else if (strcmp(binop->operator, "eq?") == 0) {
      numV1 = interp(binop->bozor1);
      numV2 = interp(binop->bozor2);
      val->numVal = numV1->numVal == numV2->numVal;
   }

   return val;
}

void* evalIfC(ifC* ifc) {
   if (interp(ifc->test)->numVal) {
      return ifc->then;
   }
   else 
      return ifc->els;
}

numV *interp(void *e) {
  char *type = ((numC *) e)->type;

  if (strcmp(type, "binopC") == 0) {
    binopC *expr = (binopC *) e;
    return evalBinop(expr);
  }
  else if (strcmp(type, "numC") == 0) {
    numV *result = malloc(sizeof(numV));
    makeString(result->type, "numV");

    numC *expr = (numC *) e;
    result->numVal = expr->numVal;
    return result;
  }
  else if (strcmp(type, "ifC") == 0) {
    ifC *expr = (ifC *) e;
    return interp(evalIfC(expr));
  }
  else if (strcmp(type, "boolC") == 0) {
    numV *result = malloc(sizeof(numV));
    makeString(result->type, "numV");
    result->numVal = ((boolC *) e)->val;
    return result;
  }

  return NULL;
}

double serialize(numV *n) {
  return n->numVal;
}

int main(int argc, char const* argv[])
{
  // test numC interp
  printf("** Testing numC interp **\n");
  numC *n = malloc(sizeof(numC));
  makeString(n->type, "numC");
  n->numVal = 3.14;
  assert(3.14 == serialize(interp(n)));


  // test boolC interp
  printf("** Testing boolC interp **\n");
  boolC *b = malloc(sizeof(boolC));
  makeString(b->type, "boolC");
  b->val = 0;
  assert(0 == serialize(interp(b)));
  b->val = 1;
  assert(1 == serialize(interp(b)));


  // test ifC interp
  printf("** Testing ifC interp **\n");
  ifC *i = malloc(sizeof(ifC));
  makeString(i->type, "ifC");

  numC *x = malloc(sizeof(numC));
  makeString(x->type, "numC");
  x->numVal = 5;

  numC *y = malloc(sizeof(numC));
  makeString(y->type, "numC");
  y->numVal = -5;

  i->test = b;
  i->then = x;
  i->els = y;

  assert(5 == serialize(interp(i)));
  b->val = 0;
  assert(-5 == serialize(interp(i)));

  return 0;
}
