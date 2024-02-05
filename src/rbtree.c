#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil -> color = RBTREE_BLACK;

  p -> nil = p -> root = nil;

  return p;
}
void search_delete(rbtree *t , node_t *node)
{
  if (node->left != t->nil)
    search_delete(t,node ->left);
  if (node->right != t->nil)
    search_delete(t,node ->right);

  free(node);
  
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *node = t->root;
  if(node != t->nil)
   search_delete(t,node);
  free(t->nil);
  free(t);
}

void exchange_color(node_t *a, node_t *b)
{
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp==RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
}

void right_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_right = node->right;

  if(parent == t->root)
    t->root = node;
  
  else
  {
    if(grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent;
  parent->parent = node;
  node->right = parent;
  node_right -> parent = parent;
  parent->left = node_right;

}

void left_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_left = node->left;

  if(parent == t->root)
    t->root = node;
  
  else
  {
    if(grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent;
  parent->parent = node;
  node->left = parent;
  parent->right = node_left;
  node_left->parent = parent;
}


void rbtree_insert_fixup(rbtree *t, node_t *node)
{
  node_t *parent = node -> parent;
  node_t *grand_parent = parent -> parent;
  node_t *uncle;
  int is_left = node == parent->left;
  int is_parent_is_left;

  if(node == t->root)
  {
    node->color = RBTREE_BLACK;
    return;
  }

  if(parent->color == RBTREE_BLACK)
  return;

  is_parent_is_left = grand_parent->left == parent;
  uncle = (is_parent_is_left) ? grand_parent->right : grand_parent->left;

  if(uncle->color == RBTREE_RED)
  {
    parent->color = RBTREE_BLACK;
    uncle->color = RBTREE_BLACK;
    grand_parent->color = RBTREE_RED;
    rbtree_insert_fixup(t,grand_parent);
    return;
  }
  if(is_parent_is_left)
  {
    if(is_left)
    {
      right_rotate(t,parent);
      exchange_color(parent,parent->right);
      return;
    }

    left_rotate(t,node);
    right_rotate(t,node);
    exchange_color(node,node->right);
    return;
  }
  if(is_left)
  {
    right_rotate(t,node);
    left_rotate(t,node);
    exchange_color(node,node->left);
    return;
  }

  left_rotate(t,parent);
  exchange_color(parent,parent->left);

}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new_node = (node_t *)calloc(1,sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = t -> nil;

  node_t *current = t->root;
  while(current != t->nil)
  {
    if(key < current->key)
    {
      if(current->left == t->nil)
        {
          current->left = new_node;
          break;
        }
    
    current = current->left;
    }
    else
    {
      if(current->right == t->nil)
      {
        current->right = new_node;
        break;
      }
      current = current->right;
    }
  }
new_node->parent = current;
if(current == t->nil)
t->root = new_node;

rbtree_insert_fixup(t,new_node);
  return t->root;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  {
    node_t *current = t->root;
    while (current != t->nil)
    {
      if(key == current->key)
        return current;
      else
        current = (key < current->key) ? current->left : current->right;
    }
    return NULL;
  }
  
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->left != t->nil)
    current = current->left;
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->right != t->nil)
    current = current->right;
  return current;

}
node_t *get_next_node(const rbtree *t , node_t *p)
{
  node_t *current = p->right;
  if(current == t->nil)
  {
    current = p;
    while(1)
    {
      if (current->parent->right == current)
        current = current->parent;
      else
        return current->parent;
    }

  }
  while(current->left != t->nil)
    current = current->left;
  return current;
}

void rbtree_erase_fixup(rbtree *t , node_t*parent,int is_left)
{
  node_t *extra_black = is_left ? parent->left : parent->right;
  if(extra_black->color == RBTREE_RED)
  {
    extra_black->color = RBTREE_BLACK;
    return;
  }

node_t *sibling = is_left ? parent->right : parent->left;
node_t *sibling_left = sibling->left;
node_t *sibling_right = sibling->right;

if(sibling->color == RBTREE_RED)
{
  if(is_left)
    left_rotate(t,sibling);
  else
    right_rotate(t,parent);
  exchange_color(sibling,parent);
  rbtree_erase_fixup(t,parent,is_left);
  return;
}

node_t *near = is_left ? sibling_left : sibling_right;
node_t *distant = is_left ? sibling_right : sibling_left;

if(is_left && near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
{
  right_rotate(t,near);
  exchange_color(sibling,near);
  rbtree_erase_fixup(t,parent,is_left);
  return;
}

if(is_left && distant->color == RBTREE_RED)
{
  left_rotate(t,sibling);
  exchange_color(sibling,parent);
  distant->color = RBTREE_BLACK;
  return;
}

if(near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
{
  left_rotate(t,near);
  exchange_color(sibling,near);
  rbtree_erase_fixup(t,parent,is_left);
  return;
}

if(distant->color == RBTREE_RED)
{
  right_rotate(t,sibling);
  exchange_color(sibling,parent);
  distant->color = RBTREE_BLACK;
  return;
}


sibling->color = RBTREE_RED;

if(parent!=t->root)
rbtree_erase_fixup(t,parent->parent,parent->parent->left == parent);
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *remove;
  node_t *remove_parent , *replace_node;
  int is_remove_black, is_remove_left;

  if (p->left != t->nil && p->right != t->nil)
  {
    remove = get_next_node(t,p);
    replace_node = remove->right;
    p->key = remove->key;
  }

  else
  {
    remove = p;
    replace_node = (remove->right != t->nil) ? remove->right : remove->left;
  }
  remove_parent = remove->parent;
  if(remove == t->root)
  {
    t->root = replace_node;
    t->root->color = RBTREE_BLACK;
    free(remove);
    return 0;
  }

  is_remove_black = remove->color;
  is_remove_left = remove_parent->left == remove;

  if (is_remove_left)
    remove_parent->left = replace_node;
  else
    remove_parent->right = replace_node;

  replace_node->parent = remove_parent;
  free(remove);

  if(is_remove_black)
    rbtree_erase_fixup(t,remove_parent,is_remove_left);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *current = rbtree_min(t);
  arr[0] = current->key;
  for(int i = 1; i<n; i++)
  {
    if(current == t->nil)
      break;
    current = get_next_node(t,current);
    if(current == t->nil)
      break;
    arr[i] = current -> key;
  }
  return 0;
}
