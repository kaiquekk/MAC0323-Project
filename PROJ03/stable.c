#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stable.h"
#define M 23 /*Change the M value for the hash function*/

typedef struct node{
    const char *key;
    EntryData *data;
    struct node *next;
} node_t;

typedef struct{
    node_t *head;
    int size;
} linked_list;

struct stable_s{
    linked_list hash[M];
    int n;
};

SymbolTable stable_create(){
    struct stable_s *st = malloc(sizeof(struct stable_s));
    st->n = 0;
    return st;
}
static node_t *createHead(const char* str){
    node_t* head = malloc(sizeof(node_t*));
    head->data = (EntryData*)malloc(sizeof(EntryData));
    head->key = str;
    head->next = NULL;
    return head;
}
static node_t *createNode(const char* str, node_t *head){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->data = (EntryData*)malloc(sizeof(EntryData));
    node->key = str;
    node->next = head;
    return node;
}
void stable_destroy(SymbolTable table){
    memset(table, 0, sizeof(struct stable_s));
    free(table);
}
static int hash(const char *key, int m){//modular hash function
    return strlen(key)%m;
}
InsertionResult stable_insert(SymbolTable table, const char *key){
    InsertionResult ir;
    int h = hash(key, M); //return the hash function of the key
    if(table->hash[h].size == 0){//the linked list is empty on the h hash position
        table->hash[h].head = createHead(key);
        table->hash[h].size = 1;
        ir.new = 1;              
        ir.data = table->hash[h].head->data;
        table->n++;
    }
    else{
        node_t *current = table->hash[h].head;
        int have = 0; //flag to know if the linked list has the key
        while(current != NULL){
            if(strcmp(current->key, key) == 0){//the linked list has the key
                ir.new = 0;
                ir.data = current->data;
                have = 1;
                break;
            }
            current = current->next;
        }
        if(have == 0){ //the linked list doesn't have the key, so it needs to create a new node
            node_t *node = malloc(sizeof(node_t));
            node = createNode(key, table->hash[h].head);
            table->hash[h].head = node;
            table->hash[h].size++;
            ir.new = 1;
            ir.data = table->hash[h].head->data;
            table->n++;
        }
    }
    return ir;
}
EntryData *stable_find(SymbolTable table, const char *key){
    int h = hash(key, M);
    node_t *current = table->hash[h].head;
    int have = 0; //flag to know if the linked_list has the key
    while(current != NULL){
        if(strcmp(current->key, key) == 0){
            have = 1;
            return current->data;
            break;
        }
        current = current->next;
    }
    if(have == 0){
        current = NULL;
    }
    return NULL;
}
static int print(const char *key, EntryData *data){
    if(key != NULL && data != NULL){
        printf("%s\t\t%d\n", key, data->i);
        return 1;
    }
    else{
        return 0;
    }
}
int stable_visit(SymbolTable table, int (*visit)(const char *key, EntryData *data)){
    for (int i = 0; i < M; i++){
        node_t *current = table->hash[i].head;
        while(current != NULL){
            printf("%s\t\t%d\n", current->key, current->data->i);
            current = current->next;
        }
    }
    return 0;
}