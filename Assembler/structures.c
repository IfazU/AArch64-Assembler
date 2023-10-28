#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "structures.h"
#define MAX_SYMBOLS 100


// Symbol entry and table functions
symbolEntry createSymEntry(char *label, uint32_t memoryAddress) {
  symbolEntry newEntry = malloc(sizeof(struct symbolEntry));
  if (newEntry == NULL) {
    printf("Malloc failed when allocating newEntry");
    return NULL;
  }

  strcpy(newEntry->label, label);
  newEntry->memoryAddress = memoryAddress;

  return newEntry;
}

symbolTable createSymTable(int count, symbolEntry *table) {
  symbolTable newTable = malloc(sizeof(struct symbolTable));
  if (newTable == NULL) {
    printf("Malloc failed when allocating newTable");
    return NULL;
  }

  newTable->count = count;
  for (int i = 0; i < count; i++) {
    newTable->table[i] = table[i];
  }

  return newTable;
}


uint32_t getAddress(symbolTable symtable, char *label) {
  for (int i = 0; i < (symtable->count); i++) {
    if (strcmp(symtable->table[i]->label, label) == 0) {
      return symtable->table[i]->memoryAddress;
    }
  }
  printf("Label not found!");
  return -1;
}

void printSymTable(symbolTable symTable) {
  for(int i = 0; i < symTable->count; i ++) {
    printf("Label : %s, Address: %d\n"
           , symTable->table[i]->label, symTable->table[i]->memoryAddress);
  }
}




// Function Ptr entry and table functions
funcPtrEntry createFuncEntry(char *name, nodeFunc func) {
  funcPtrEntry funcPtr = malloc(sizeof (struct funcPtrEntry));
  if (funcPtr == NULL) {
    printf("Malloc failed when allocating funcPtr");
    return NULL;
  }
  funcPtr->name = strdup(name);
  if (name == NULL) {
    printf("Malloc failed when allocating FuncEntry name");
    free(funcPtr);
    return NULL;
  }
  funcPtr->func = func;

  return funcPtr;
}

//table is an array of function pointer entries
funcPtrTable createFuncTable(int count, funcPtrEntry table[]) {
  funcPtrTable funcTable = malloc(sizeof (struct funcPtrTable));
  if (funcTable == NULL) {
    printf("Malloc failed when allocating funcTable");
    return NULL;
  }

  funcTable->count = count;

  for (int i = 0; i < count; i++) {
    funcTable->table[i] = table[i];
  }

  return funcTable;
}

nodeFunc getFuncPtr(char *label, funcPtrTable funTable) {
  for (int i = 0; i < funTable->count; i++) {
    if (strcmp(label, funTable->table[i]->name) == 0) {
      return funTable->table[i]->func;
    }
  }
  printf("funcPtr not found!");
  return NULL;
}


// Node functions


Node createNode(uint32_t memoryAddress, const char *type, int num, const char** args) {
  Node newNode = malloc(sizeof(struct Node));
  if (newNode == NULL) {
    printf("Malloc failed when allocating newNode");
    return NULL;
  }

  newNode->memoryAddress = memoryAddress;

  // Allocate memory for the type string and copy the content
  newNode->type = malloc(strlen(type) + 1);
  if (newNode->type == NULL) {
    printf("Malloc failed when allocating for type");
    free(newNode);
    return NULL;
  }
  strcpy(newNode->type, type);

  // Allocate memory for the args array
  newNode->args = malloc(sizeof(char *) * num);
  if (newNode->args == NULL) {
    printf("Malloc failed when allocating args list");
    free(newNode->type);
    free(newNode);
    return NULL;
  }

  // Allocate memory for each args element and copy string
  for (int i = 0; i < num; i++) {
    newNode->args[i] = malloc(sizeof (char) * (strlen(args[i]) + 1));

    if (newNode->args[i] == NULL) {
      printf("Malloc failed when allocating arg[%d]", i);
      // To free all the args before
      for (int j = 0; j < i; j++) {
        free(newNode->args[j]);
      }
      free(newNode->args);
      free(newNode->type);
      free(newNode);
      return NULL;
    }

    strcpy(newNode->args[i], args[i]);
  }


  newNode->num = num;
  newNode->next = NULL;

  return newNode;
}


Node addNextNode(Node currNode, Node addNode, List list) {
  currNode->next = addNode;
  list->count ++;
  list->last = addNode;
  return currNode;
}

Node createAlisCopyNode(Node node, int n, char *arg) {
  //Creating the array with extra element at index n
  char *arrayOfArgs[node->num + 1];
  int j = 0;
  for (int i = 0; i < node->num + 1; i++) {
    if (i == n) {
      arrayOfArgs[i] = malloc(sizeof(strlen(arg) + 1));
      strcpy(arrayOfArgs[i], arg);
    }
    arrayOfArgs[i] = malloc(sizeof(strlen(node->args[j]) + 1));
    strcpy(arrayOfArgs[i], node->args[j]);
    j++;
  }
  
  //Creating copyNode with new string inserted
  Node copyNode = createNode(node->memoryAddress,
  node->type,node->num + 1, arrayOfArgs);
  copyNode->next = node->next;

  
  return copyNode;
}


void printNode(Node node) {
  char builder[100];
  strcpy(builder, node->type);
  for (int i = 0; i < node->num; i++) {
    strcat(builder, " ");
    strcat(builder, (node->args)[i]);
  }
  printf("%s\n", builder);
}


//List functions


List createList(Node startNode, Node endNode, int count) {
  List list = malloc(sizeof(struct List));
  if (list == NULL) {
    printf("Malloc failed when allocating list");
    return NULL;
  }

  list->first = startNode;
  list->last = endNode;
  assert(count >= 0 );
  list->count = count;

  return list;
}

// Similar to createList 
List createListWithStart(Node startNode) {
  return createList(startNode, NULL, 1);
}

List createListWithBoth(Node startNode, Node endNode) {
  return createList(startNode, endNode, -1);
}

void printList(List list) {
  Node node = list->first;
  for (int i = 0; i < list->count; i++) {
    if (node == NULL) {
      printf("NULL NODE REACHED!");
      return;
    }
    printNode(node);
    node = node->next;
  }
}


// Free Functions Below

void freeFuncPtrEntry(funcPtrEntry entry) {
  free(entry->name);
  free(entry);
}

//Frees the memory allocated for a funcPtr table and its entries.
 
void freeFuncPtrTable(funcPtrTable funTable) {
  // Frees each of the entries
  for(int i = 0; i < funTable->count; i++) {
    freeFuncPtrEntry(funTable->table[i]);
  }
  free(funTable);
}




void freeSymbolEntry(symbolEntry symEntry) {
  free(symEntry);
}

//Frees the memory allocated for a symbol table and its entries.

void freeSymbolTable(symbolTable symTable) {
  for (int i = 0; i < symTable->count; i++) { // Free each of the symbolEntries
    freeSymbolEntry(symTable->table[i]);
  }
  free(symTable);
}


void freeNode(Node node) {
  free(node->type);
  // Free each of the strings stored in args
  for (int i = 0; i < node->num; i++){
    free((node->args)[i]);
  }
  free(node->args);
  free(node);
}


//Frees the memory allocated for a linked list and its nodes.

void freeList(List list) {
  assert (list->count != 0);
  Node node = list->first;
  Node nextNode = list->first;
  for (int i = 0; i < (list->count); i++) {
    nextNode = node->next;
    freeNode(node);
    node=nextNode;
  }
  free(list);
}












