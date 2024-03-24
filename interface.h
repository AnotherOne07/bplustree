/*
  Funções e algoritmos baseados na implementação disponível em: http://www.amittai.com/prose/bplustree.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A ordem D padrão da árvore B+ é 4
#define DEFAULT_ORDER 5


/* TIPOS */
// Tipo que representa um registro
typedef struct record {
  int value;
}record;

// Nó de uma árvore B+
// Representa uma generalização de um 
// nó folha e um nó interno
typedef struct node {
  void **pointers;
  int * keys;
  struct node *parent;
  bool is_leaf;
  int num_keys;
  struct node *next;
}node;

/* Globais */
// A ordem determina o número máximo e mínimo de chaves e ponteiros em um nó 
int order = DEFAULT_ORDER;

node * queue = NULL;

/* Assinatura de funções */

/* Funções de utilidade */
// Funções de impressão
void enqueue(node * new_node);
node * dequeue();
int path_to_root(node * root, node * child);
void print_leaves(node *root);
void print_tree(node *root);

// Funções de busca
node * find_leaf(node * root, int key);
record * find(node * root, int key);
int cut_node(int length);


/* Funções de inserção */
// Cria um registro em memória
record * create_record(int value);

// Cria uma página interna em memória
node * create_node();

// Cria uma página folha em memória
node * create_leaf();

// Retorna o índice imediatamente a esquerda em um nó
int get_left_index(node * parent, node * left);

// Insere um novo par: chave-registro em uma página folha
node * insert_into_leaf( node * leaf, int key, record * pointer );

// Insere um novo par: chave-registro em uma página, depois de realizar o 
// algoritmo de particionamento
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key,
record * pointer);

// Insere um novo par: chave-ponteiro em uma página interna
node * insert_into_node(node * root, node * parent, 
int left_index, int key, node * right);

// Insere um novo par: chave-ponteiro em uma página interna, depois de realizar o algoritmo de particionamento
node * insert_into_node_after_splitting(node * root, node * parent, int left_index, int key, node * right);

// Atualiza um nó pai ou insere um novo nó interno (ou raíz) quando ocorre em um dos seus filhos
// o algoritmo de particionamento
node * insert_into_parent(node * root, node * left, int key, node * right);

// Cria uma nova raíz para duas subárvores e insere a chave apropriada na nova raíz
node * insert_into_new_root(node * left, int key, node * right);

// Inicia uma nova árvore B+
node * start_new_tree(int key, record * pointer);

// Insert MASTER
node * insert(node * root, int key, int value);

/* Funções de exclusão */

// Retorna o índice da chave mais próxima
int get_neighbor_index(node * neighbor);

// Remove um índice e ajusta os índices remanescentes
node * remove_entry_from_node(node * n, int key, node * pointer);

// Ajusta a raíz da árvore B+ após a remoção de uma chave e seu fiel escudeiro
node * adjust_root(node * root);

// Concatena um nó que fere a regra de chaves mínimas por nó de uma árvore b+ com um nó vizinho
node * concatenate_nodes(node * root, node * n, node * neighbor, int neighbor_inde, int k_prime);

// Redistribui as chaves e ponteiros entre dois nós para garantir que nenhum nó fique ou muito pequeno ou muito grande
node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime_index, int k_prime);

node * delete_entry(node * root, node * n, int key, void * pointer);

node * delete_action(node * root, int key);
