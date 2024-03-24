#include "library.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  node *root;
  root = NULL;

  // Case 1: Insert on empty root;
  printf("Insert on empty root: \n");
  root = insert(root, 1, 1);
  root = insert(root, 2, 2);
  root = insert(root, 3, 3);
  print_tree(root);

  printf("\n");
  root = NULL;
  
  // Case 2: Insert on leaf after splitting
  printf("Insert on leaf after splitting: \n");
  root = insert(root, 1, 1);
  root = insert(root, 2, 2);
  root = insert(root, 3, 3);
  root = insert(root, 4, 4);
  print_tree(root);

  printf("\n");
  root = NULL;

  // Case 3: Insert on leaf after splitting
  root = insert(root, 1, 1);
  root = insert(root, 2, 2);
  root = insert(root, 3, 3);
  root = insert(root, 4, 4);
  root = insert(root, 5, 5);
  root = insert(root, 6, 6);
  root = insert(root, 7, 7);
  root = insert(root, 8, 8);
  root = insert(root, 9, 9);
  print_tree(root);
  printf("\n");

  root = NULL;
	root = insert(root, 1, 1);
	root = insert(root, 2, 2);
	root = insert(root, 3, 3);
	root = insert(root, 4, 4);
	root = insert(root, 5, 5);
	root = insert(root, 6, 6);
	root = insert(root, 7, 7);
	root = insert(root, 8, 8);
	root = insert(root, 9, 9);
	root = insert(root, 10, 10);
	root = insert(root, 11, 11);
	root = insert(root, 12, 12);
	root = insert(root, 13, 13);
	root = insert(root, 14, 14);
	root = insert(root, 15, 15);
	root = insert(root, 16, 16);
	root = insert(root, 17, 17);
	root = insert(root, 18, 18);
	root = insert(root, 19, 19);
	root = insert(root, 20, 20);
	root = insert(root, 21, 21);
	root = insert(root, 22, 22);
	root = insert(root, 23, 23);
	root = insert(root, 24, 24);
	root = insert(root, 25, 25);
	root = insert(root, 26, 26);
	root = insert(root, 27, 27);
	root = insert(root, 28, 28);
	root = insert(root, 29, 29);
	root = insert(root, 30, 30);
	root = insert(root, 31, 31);
	root = insert(root, 32, 32);
	root = insert(root, 33, 33);
	root = insert(root, 34, 34);
	root = insert(root, 35, 35);
	root = insert(root, 36, 36);
	print_tree(root);

  return 0;
}