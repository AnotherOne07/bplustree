#include "interface.h"

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// Funções auxiliares para imprimir árvore

// Imprimir todas as chaves nas folhas da árvore
void print_leaves(node *root){
  node * current = root;
  if(root == NULL){
    printf("Empty tree.\n");
    return;
  }
  // Enquanto não for uma folha, desce na árvore
  while(!current->is_leaf)
    current = current->pointers[0];

  // Itera sobre cada chave na folha
  while(true){
    for(int i = 0;i < current->num_keys;i++){
      printf("%d ", current->keys[i]);
    }
    // Verifica se há um ponteiro para a próxima folha
    if(current->pointers[order - 1] != NULL){
      printf("| ");
      current = current->pointers[order - 1];
    }else break;
  }
  printf("\n");
}

// Imprime a árvore B+ em ordem de nível(ou ainda, rank), exibindo as chaves em cada nó
void print_tree(node *root){
  node *cNode = NULL;
  int i = 0;
  int rank = 0;
  int new_rank = 0;

  // Validação de que a árvore não está vazia
  if(root == NULL){
    printf("Empty tree.\n");
    return;
  }
  // Inicializa a fila 'queue' e insere o nó raíz na fila
  queue = NULL;
  enqueue(root);

  while(queue != NULL){
    cNode = dequeue();
    if(cNode->parent != NULL && cNode == cNode->parent->pointers[0]){
      new_rank = path_to_root(root, cNode);
      if(new_rank != rank){
        rank = new_rank;
        printf("\n");
      }
    }
    for(i = 0; i < cNode->num_keys;i++){
      printf("%d ", cNode->keys[i]);
    }
    if(!cNode->is_leaf){
      for(i = 0;i <= cNode->num_keys;i++)
        enqueue(cNode->pointers[i]);
    }
    printf("| ");
  }
  printf("\n");
  
}

// Função auxiliar para imprimir árvore
void enqueue(node * new_node){
  node * current;
  if(queue == NULL){
    queue = new_node;
    queue->next = NULL;
  }else{
    current = queue;
    while(current->next != NULL){
      current = current->next;
    }
    current->next = new_node;
    new_node->next = NULL;
  }
}

// Função auxiliar para imprimir árvore
node * dequeue (){
  node * cNode = queue;
  queue = queue->next;
  cNode->next = NULL;
  return cNode;
}

// Função auxiliar que calcula o comprimento do caminho de um nó até a raiz
int path_to_root(node * root, node * child){
  int length = 0;
  node * current = child;
  // Enquanto o nó atual não for igual a raíz, incrementa o comprimento e sobe na árvore através do ponteiro pai
  while(current != root){
    current = current->parent;
    length++;
  }
  return length;
}

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

node * find_leaf(node * root, int key){
  node * current = root;
  int i;
  if(current == NULL){
    return current;
  }
  // Enquanto não for uma folha, desce na árvore
  while(!current->is_leaf){
    i = 0;
    // Itera o nó até encontrar uma chave maior do (key) 
    while (i < current->num_keys){
      if(key >= current->keys[i]){
        i++;
      }else
        break;
    }
    // Utiliza o índice i para acessar o ponteiro do nó filho
    current = (node *)current->pointers[i];
  }
  // Ao final da iteração, current representa a folha onde a chave deve ser inserida
  return current;
}

// Percorre a árvore até as folhas e retorna um ponteiro para um registro
// caso encontre a sua chave na folha
record * find(node * root, int key){
  int i = 0;
  node * current = find_leaf(root, key);
  if (current == NULL) 
    return NULL;
  // Em uma folha, se encontrar a chave, PARE
  for(i = 0;i < current->num_keys;i++){  
    if(current->keys[i] == key)
      break;
  }
  // Acabaram os filhos e nao encontrou a chave
  if(i == current->num_keys)
    return NULL;
  return (record *)current->pointers[i];
}

// Encontra o "ponto de divisão" das chaves de um nó
int cut_node(int length){
  if(length % 2 == 0)
    return length/2;
  return length/2 + 1;
}

/* Funções de inserção */

// Cria um registro em memória
record * create_record(int value){
  record * new_record = (record *) malloc(sizeof(record));
  if(new_record == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  new_record->value = value;
  return new_record;
}

// Cria uma página interna em memória
node * create_node(){
  node * new_node;
  new_node = malloc(sizeof(node));
  if(new_node == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  // Alocando memória para os ponteiros e chaves 
  new_node->pointers = malloc( order * sizeof(void *) );
  if(new_node->pointers == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  new_node->keys = malloc( (order - 1) * sizeof(int) );
  if(new_node->keys == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  new_node->is_leaf = false;
  new_node->num_keys = 0;
  new_node->parent = NULL;
  new_node->next = NULL;
  return new_node;
}

// Cria uma página folha em memória
node * create_leaf(){
  node * new_leaf = create_node();
  new_leaf->is_leaf = true;
  return new_leaf;
}

// Retorna o índice imediatamente a esquerda em um nó
// Usada para encontrar o índice do ponteiro do pai para o nó à esquerda da chave a ser inserida
int get_left_index(node * parent, node * left){
  int left_index = 0;
  while(left_index <= parent->num_keys && parent->pointers[left_index] != left)
    left_index++;
  return left_index;
}

// Insere um novo par: chave-registro em uma página folha
node * insert_into_leaf( node * leaf, int key, record * pointer ){
  int i;
  int insertion_point = 0;
  
  // Encontra a posição de inserção no nó folha
  while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
    insertion_point++;

  // Desloca as chaves do nó folha para a direita até o ponto de inserção
  for (i = leaf->num_keys; i > insertion_point; i--) {
    leaf->keys[i] = leaf->keys[i - 1];
    leaf->pointers[i] = leaf->pointers[i - 1];
  }
  // Atribui chave e ponteiro para informaçoes do registro
  leaf->keys[insertion_point] = key;
  leaf->pointers[insertion_point] = pointer;
  leaf->num_keys++;
  return leaf;
}

// Insere um novo par: chave-registro em uma página, depois de realizar o 
// algoritmo de particionamento por excesso de chaves em uma folha
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer){
  node * new_leaf;
  int * temp_keys;
  void ** temp_pointers;
  int insertion_index, split, new_key, i, j;

  // Novo nó para armazenar chaves e pointeiros excedentes
  new_leaf = create_leaf();
  
  // Alocação de memória para os arrays de chaves e ponteiros do novo nó temporário
  temp_keys = malloc( order * sizeof(int) );
  if(temp_keys == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  temp_pointers = malloc( order * sizeof(void *) );
  if(temp_pointers == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }

  // Encontra posição correta para inserção
  insertion_index = 0;
  while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
    insertion_index++;

  // Copia chaves e ponteiros para os arrays temporários, garantindo que haja espaço para a nova chave
  // respeitando a ordem de ordenação na página
  for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
    if (j == insertion_index) j++;
    temp_keys[j] = leaf->keys[i];
    temp_pointers[j] = leaf->pointers[i];
  }

  temp_keys[insertion_index] = key;
  temp_pointers[insertion_index] = pointer;

  leaf->num_keys = 0;

  // Calcula o ponto de divisão para as chaves e ponteiros entre a folha atual e a nova folha
  split = cut_node(order - 1);

  // Copia metade das chaves e ponteiros para a folha atual e metade para a nova folha
  for (i = 0; i < split; i++) {
    leaf->pointers[i] = temp_pointers[i];
    leaf->keys[i] = temp_keys[i];
    leaf->num_keys++;
  }
  for (i = split, j = 0; i < order; i++, j++) {
    new_leaf->pointers[j] = temp_pointers[i];
    new_leaf->keys[j] = temp_keys[i];
    new_leaf->num_keys++;
  }

  // Liberar memória alocada para os arrays temporários
  free(temp_pointers);
  free(temp_keys);

  // Atualiza os ponteiros da folha atual e da nova folha para manter a ordem correta na lista encadeada de folhas
  new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
  leaf->pointers[order - 1] = new_leaf;

  // Zera os ponteiros que não possuem alguma chave à esquerda na folha atual e na nova folha
  for (i = leaf->num_keys; i < order - 1; i++)
    leaf->pointers[i] = NULL;
  for (i = new_leaf->num_keys; i < order - 1; i++)
    new_leaf->pointers[i] = NULL;

  // Atualiza o pai da nova folha, para ser mesmo que o da folha atual
  // obtém a nova chave que será inserida no pai (chave d+1)
  new_leaf->parent = leaf->parent;
  new_key = new_leaf->keys[0];

  // Chamada da função que vai atualizar o pai com a chave (d+1)
  return insert_into_parent(root, leaf, new_key, new_leaf);
}

// Insere um novo par: chave-ponteiro em uma página interna
node * insert_into_node(node * root, node * n, int left_index, int key, node * right){
  int i;
  // Deslocando as chaves e ponteiros do nó interno para a direita
  for (i = n->num_keys; i > left_index; i--) {
    n->pointers[i + 1] = n->pointers[i];
    n->keys[i] = n->keys[i - 1];
  }

  n->pointers[left_index + 1] = right;
  n->keys[left_index] = key;
  n->num_keys++;
  return root;
}

// Insere um novo par: chave-ponteiro em uma página interna, depois de realizar o algoritmo de particionamento
node * insert_into_node_after_splitting(node * root, node * old_node, int left_index, int key, node * right){
  int i, j, split, k_prime;
  node * new_node, * child;
  int * temp_keys;
  node ** temp_pointers;

  // Alocação de memória para os arrays temporários de ponteiros e chaves
  temp_pointers = malloc( (order + 1) * sizeof(node *));
  if(temp_pointers == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  
  temp_keys = malloc( order * sizeof(int));
  if(temp_keys == NULL){
    printf("Error! Falha na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }

  // Copia os ponteiros e chaves do nó antigo para os arrays temporários, garantindo que haja espaço para a nova chave
  // respeitando a ordem de ordenação no nó
  for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) {
    if (j == left_index + 1) j++;
    temp_pointers[j] = old_node->pointers[i];
  }
  for (i = 0, j = 0; i < old_node->num_keys; i++, j++) {
    if (j == left_index) j++;
    temp_keys[j] = old_node->keys[i];
  }

  // Insere a nova chave e o ponteiro na matriz temporária na posição indicada por left_index
  temp_pointers[left_index + 1] = right;
  temp_keys[left_index] = key;

  // Calcula o ponto de divisão para as chaves e ponteiros entre o nó antigo e o novo nó
  split = cut_node(order);

  // Cria novo nó e copia metade das chaves e ponteiros para o nó antigo e a outra metade para o novo nó
  new_node = create_node();
  old_node->num_keys = 0;
  for (i = 0; i < split - 1; i++) {
    old_node->pointers[i] = temp_pointers[i];
    old_node->keys[i] = temp_keys[i];
    old_node->num_keys++;
  }
  old_node->pointers[i] = temp_pointers[i];
  
  // Essa chave k_prime é a chave que será inserida no nó pai (representa a chave d+1)
  k_prime = temp_keys[split - 1];

  // ++i é necessário porque na árvore b+, ao realizar o algoritmo de particionamento, a chave d+1 (k_prime)
  // somente é inserida no pai, não permanece uma cópia no novo nó, diferente de como acontece em uma folha
  for (++i, j = 0; i < order; i++, j++) {
    new_node->pointers[j] = temp_pointers[i];
    new_node->keys[j] = temp_keys[i];
    new_node->num_keys++;
  }
  new_node->pointers[j] = temp_pointers[i];
  free(temp_pointers);
  free(temp_keys);
  new_node->parent = old_node->parent;

  // Atualiza os ponteiros dos nós filhos para apontarem corretamente para os seus pais (podem ser nós internos ou folhas)
  for (i = 0; i <= new_node->num_keys; i++) {
    child = new_node->pointers[i];
    child->parent = new_node;
  }

  // Retorna o resultado da chamada da fução que vai atualizar o pai com a chave (d+1)
  return insert_into_parent(root, old_node, k_prime, new_node);
}

// Atualiza um nó pai ou insere um novo nó interno (ou raíz) quando ocorre em um dos seus filhos
// o algoritmo de particionamento
node * insert_into_parent(node * root, node * left, int key, node * right) {
  
  int left_index;
  node * parent;

  parent = left->parent;

  // Caso 1: Nova raíz
  if (parent == NULL)
    return insert_into_new_root(left, key, right);

  // Encontra o índice onde o ponteiro para o nó esquerdo está armazenado no nó pai
  left_index = get_left_index(parent, left);
  
  // Caso 2: O nó interno tem espaço para inserir a nova chave
  if (parent->num_keys < order - 1)
    return insert_into_node(root, parent, left_index, key, right);

  // Caso 3: O nó interno não tem espaço para inserir a nova chave
  return insert_into_node_after_splitting(root, parent, left_index, key, right);
}

node * start_new_tree(int key, record * reg){
  node * root = create_leaf();
  root->keys[0] = key;
  root->pointers[0] = reg;
  root->pointers[order - 1] = NULL;
  root->parent = NULL;
  root->num_keys++;
  return root;
}

// Cria uma nova raíz para duas subárvores e insere a chave apropriada na nova raíz
node * insert_into_new_root(node * left, int key, node * right){
  // Cria nova raíz
  node * new_root = create_node();

  // Atribui a nova chave e os ponteiros paras as subárvores esquerda(antiga raíz) e direita(novo nó)
  new_root->keys[0] = key;
  new_root->pointers[0] = left;
  new_root->pointers[1] = right;
  new_root->num_keys++;
  new_root->parent = NULL;

  // Atualiza o pai das duas subárvores
  left->parent = new_root;
  right->parent = new_root;

  return new_root;
  
}

node * insert(node * root, int key, int value){
  record * reg;
  node * leaf;

  // Verifica se a chave já existe na árvore, se existir retorna a árvore sem alterações
  if(find(root, key) != NULL)
    return root;

  reg = create_record(value);

  // Caso 1: A árvore está vazia
  if(root == NULL)
    return start_new_tree(key, reg);

  leaf = find_leaf(root, key);
  // Caso 2: A árvore existe e a folha onde a chave deve ser inserida tem espaço para inserir a nova chave
  if(leaf->num_keys < order - 1){
    leaf = insert_into_leaf(leaf, key, reg);
    return root;
  }

  // Caso 3: A árvore existe e a folha onde a chave deve ser inserida não tem espaço para inserção da nova chave
  return insert_into_leaf_after_splitting(root, leaf, key, reg);
}

/* Funções de exclusão */

// Retorna o índice do irmão mais próximo de um nó à esquerda, se existir
// se o ponteiro for o fiho mais à esquerda de um nó, retorna -1
int get_neighbor_index( node * n ) {
  int i;
  // Para cada ponteiro, verifica se ele aponta para o nó (neighbor)
  // se encontrar, retorna o índice do ponteiro - 1
  for (i = 0; i <= n->parent->num_keys; i++)
    if (n->parent->pointers[i] == n)
      return i - 1;
}

// Ajusta a raíz da árvore B+ após a remoção de uma chave e seu fiel escudeiro
node * adjust_root(node * root){
  node * new_root;

  // Caso 1: A raíz não está vazia
  if(root->num_keys > 0)
    return root;

  // Caso 2: A raíz está vazia e não é uma folha
  // Se tem apenas um filho, promove ele para ser a nova raíz
  if(!root->is_leaf){
    new_root = root->pointers[0];
    new_root->parent = NULL;
  }else{ 
  // Caso 3: A raíz está vazia e é uma folha
    new_root = NULL;  
  }
  free(root->keys);
  free(root->pointers);
  free(root);

  return new_root;
}

// Remove um índice e ajusta os índices remanescentes
node * remove_entry_from_node(node * n, int key, node * pointer){
  
  int i, num_pointers;

  // Remove a chave a desloca as chaves restantes no nó para a esquerda
  i = 0;
  while (n->keys[i] != key)
    i++;
  for (++i; i < n->num_keys; i++)
    n->keys[i - 1] = n->keys[i];

  // Remove o ponteiro e desloca os ponteiros restantes no nó para a esquerda
  num_pointers = n->is_leaf ? n->num_keys : n->num_keys + 1;
  i = 0;
  while (n->pointers[i] != pointer)
    i++;
  for (++i; i < num_pointers; i++)
    n->pointers[i - 1] = n->pointers[i];

  // Uma chave a menos na folha
  n->num_keys--;

  // Limpeza dos ponteiros restantes, uma vez que o conteúdo foi copiado para uma unidade à esquerda
  if(n->is_leaf){  
    for(i = n->num_keys; i < order - 1; i++)
      n->pointers[i] = NULL;
  }else{
    for(i = n->num_keys + 1; i < order; i++)
      n->pointers[i] = NULL;
  }
  return n;
}

// Concatena um nó que fere a regra de chaves mínimas por nó de uma árvore b+, com um nó vizinho 
node * concatenate_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime){
  
  int i, j, neighbor_insertion_index, n_end;
  node * tmp;

  // Verifica se o nó vizinho está à direita do nó (neighbor_index = -1), se estiver, os nós são trocados
  // pois o nó fornecido está à esquerda e o vizinho está à direita.
  if (neighbor_index == -1) {
    tmp = n;
    n = neighbor;
    neighbor = tmp;
  }

  neighbor_insertion_index = neighbor->num_keys;

  // Caso 1: O nó é um nó interno
  if (!n->is_leaf){
    
    // Copia a chave k_prime e o ponteiro (fiel escudeiro) ao final do vizinho (neighbor)
    neighbor->keys[neighbor_insertion_index] = k_prime;
    neighbor->num_keys++;

    n_end = n->num_keys;

    // Copia todas as chaves do nó n para o vizinho (neighbor) a partir da última posição livre do vizinho
    for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
      neighbor->keys[i] = n->keys[j];
      neighbor->pointers[i] = n->pointers[j];
      neighbor->num_keys++;
      n->num_keys--;
    }
    neighbor->pointers[i] = n->pointers[j];

    // Todos os filhos do vizinho precisam apontar para o mesmo pai
    for(i = 0; i < neighbor->num_keys + 1; i++){
      tmp = (node *)neighbor->pointers[i];
      tmp->parent = neighbor;
    } 
  }else{ // Caso 2: O nó é uma folha
    // Copia todas as chaves e ponteiros do nó fornecido (n) para o vizinho. (JÁ ESTÁ ORDENADO)
    for (i = neighbor_insertion_index, j = 0; j < n->num_keys; i++, j++) {
      neighbor->keys[i] = n->keys[j];
      neighbor->pointers[i] = n->pointers[j];
      neighbor->num_keys++;
    }
    // Para manter a lista encadeada nas folhas
    neighbor->pointers[order - 1] = n->pointers[order - 1];
  }
  // Remove a entrada e apaga o nó n
  root = delete_entry(root, n->parent, k_prime, n);
  free(n->keys);
  free(n->pointers);
  free(n); 
  return root;
}

// Redistribui as chaves e ponteiros entre dois nós para garantir que nenhum nó fique ou muito pequeno ou muito grande
node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime_index, int k_prime){
  int i;
  node * tmp;

  // Caso 1: O nó vizinho está a esquerda
  if(neighbor_index == -1){
    // Caso 1.1: O nó vizinho é um nó interno
      // Move o último ponteiro do nó vizinho para o final do nó n (porque tem ponteiros do que chaves em um nó)
      if (!n->is_leaf)
        n->pointers[n->num_keys + 1] = n->pointers[n->num_keys];
      // Desloca todas as chaves e ponteiros do nó n para a direita
      // Objetivo: liberar espaço para nova chave e ponteiro
      for (i = n->num_keys; i > 0; i--) {
        n->keys[i] = n->keys[i - 1];
        n->pointers[i] = n->pointers[i - 1];
      }
      
      if (!n->is_leaf) {
        n->pointers[0] = neighbor->pointers[neighbor->num_keys];
        tmp = (node *)n->pointers[0];
        tmp->parent = n;
        neighbor->pointers[neighbor->num_keys] = NULL;
        n->keys[0] = k_prime;
        n->parent->keys[k_prime_index] = neighbor->keys[neighbor->num_keys - 1];
      }
      else { // Caso 1.2: O nó vizinho é uma folha
        n->pointers[0] = neighbor->pointers[neighbor->num_keys - 1];
        neighbor->pointers[neighbor->num_keys - 1] = NULL;
        n->keys[0] = neighbor->keys[neighbor->num_keys - 1];
        n->parent->keys[k_prime_index] = n->keys[0];
      }
  }else{ // Caso 2: O nó vizinho está a direita
    // Caso 2.1: O nó vizinho é uma folha
    // Significa que o vizinho não tem filhos e está imediatamente à direita da folha mais à esquerda da árvore (n)
    // Move o primeiro par chave:ponteiro do nó vizinho para o final do nó (n). (GARANTIR QUE ESTEJA EM ORDEM)
      if (n->is_leaf) {
        n->keys[n->num_keys] = neighbor->keys[0];
        n->pointers[n->num_keys] = neighbor->pointers[0];
        n->parent->keys[k_prime_index] = neighbor->keys[1];
      }
    // Caso 2.2: O nó vizinho é um nó interno
      else {
        // Move a chave que subiu para o pai (k_prime) para o final do nó (n)
        n->keys[n->num_keys] = k_prime;
        n->pointers[n->num_keys + 1] = neighbor->pointers[0];
        tmp = (node *)n->pointers[n->num_keys + 1];
        tmp->parent = n;
        n->parent->keys[k_prime_index] = neighbor->keys[0];
      }
    // Desloca cada chave restante no vizinho uma casa para esquerda
    for (i = 0; i < neighbor->num_keys - 1; i++) {
      neighbor->keys[i] = neighbor->keys[i + 1];
      neighbor->pointers[i] = neighbor->pointers[i + 1];
    }
    if (!n->is_leaf)
      neighbor->pointers[i] = neighbor->pointers[i + 1];
  }
  // Incrementa n e decrementa neighbor
  n->num_keys++;
  neighbor->num_keys--;

  return root;
}

// Remove uma entrada da árvore B+
node * delete_entry( node * root, node * n, int key, void * pointer ){
  
  int min_keys;
  node * neighbor;
  int neighbor_index;
  int k_prime_index, k_prime;
  int capacity;

  // Remove par chave:ponteiro da árvore
  n = remove_entry_from_node(n, key, pointer);

  // Verifica se o nó é a raiz, então faz o ajuste necessário
  if (n == root) 
    return adjust_root(root);

  // Verifica se o nó é uma folha para determinar o mínimo de chaves por nó
  min_keys = n->is_leaf ? cut_node(order - 1) : cut_node(order) - 1;

  // Caso 1: Quantidade de chaves é igual ou maior do que a quantidade mínima na árvore
  if (n->num_keys >= min_keys)
    return root;

  // Caso 2: Quantidade de chaves está abaixo da quantidade permitida na árvore
  neighbor_index = get_neighbor_index( n );
  k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
  k_prime = n->parent->keys[k_prime_index];
  // O vizinho será o nó à direita, caso a folha seja a mais à esquerda
  // senão, o vizinho será o nó à esquerda
  neighbor = neighbor_index == -1 ? n->parent->pointers[1] : 
    n->parent->pointers[neighbor_index];
  capacity = n->is_leaf ? order : order - 1;

  // Para decidir qual algoritmo de combinação de nós utilizar, a função verifica se a concatenação dos nós
  // (n) e seu vizinho (n) resultará em um número de chaves ABAIXO da quantidade mínima na árvore
  if (neighbor->num_keys + n->num_keys < capacity) // Algoritmo de Concatenação
    return concatenate_nodes(root, n, neighbor, neighbor_index, k_prime);
  else // Algoritmo de Redistribuição
    return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}

node * delete(node * root, int key){
  node * key_leaf;
  record * key_record;

  key_record = find(root, key);
  key_leaf = find_leaf(root, key);
  if (key_record != NULL && key_leaf != NULL) {
    root = delete_entry(root, key_leaf, key, key_record);
    free(key_record);
  }
  return root;
}