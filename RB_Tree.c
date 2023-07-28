#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ND -1
#define RED 1
#define BLACK 0


typedef struct node_s{
	int val;
	int color;
	struct node_s *left, *right, *p;
}node_t;


node_t * T_nil = NULL;

void InOrder_Tree_Walk(node_t *);

node_t * Tree_Search(node_t *, int);

node_t * Tree_Minimum(node_t *);

node_t * Tree_Maximum(node_t *);

node_t * Tree_Successor(node_t *);

node_t * Left_Rotate(node_t *,node_t *);

node_t * Right_Rotate(node_t *,node_t *);

node_t * RB_Insert(node_t *,node_t *);

node_t * RB_Insert_Fixup(node_t *, node_t *);

node_t * RB_Transplant(node_t *, node_t *, node_t *);

node_t * RB_Delete(node_t *, node_t *);

node_t * RB_Delete_Fixup(node_t *, node_t *);



int main(int argc, char const *argv[])
{
	node_t *root = NULL, *tmp = NULL;
	int i, x, y, z;

	T_nil = malloc(sizeof(node_t));
	if(T_nil){
		T_nil->val = ND;
		T_nil->color = BLACK;
		T_nil->left = T_nil;
		T_nil->right = T_nil;
		T_nil->p = T_nil;
		root = T_nil;
	}else{
		printf("Error in memory allocation!\n");
	}
	x = 0;
	y = 4;
	z = 7;
	for (i = 0; i < 15; i++)
	{
		tmp = malloc(sizeof(node_t));
		if(tmp)
		{
			tmp->val = i;
			root = RB_Insert(root, tmp);
		}
		else{
			printf("Error in memory allocation!\n");
		}
	}

	InOrder_Tree_Walk(root);
	tmp = Tree_Search(root, x);
	if(tmp != T_nil){
		root = RB_Delete(root, tmp);
	}
	InOrder_Tree_Walk(root);
	tmp = Tree_Search(root, y);
	if(tmp != T_nil){
		root = RB_Delete(root, tmp);
	}
	InOrder_Tree_Walk(root);	
	tmp = Tree_Search(root, z);
	if(tmp != T_nil){
		root = RB_Delete(root, tmp);
	}
	InOrder_Tree_Walk(root);

	return 0;
}



void InOrder_Tree_Walk(node_t *x){
	if(x != T_nil){
		InOrder_Tree_Walk(x->left);
		printf("%d\n", x->val);
		InOrder_Tree_Walk(x->right);
	}
}


node_t * Tree_Search(node_t *x, int k){
	node_t * curr;

	curr = x;
	if(curr == T_nil || k == curr->val){
		return curr;
	}
	if(k < curr->val){
		return Tree_Search(curr->left, k);
	}else{
		return Tree_Search(curr->right, k);
	}
}


node_t * Tree_Minimum(node_t *x){
	node_t *curr;

	curr = x;
	while(curr->left != T_nil){
		curr = curr->left;
	}
	return curr;
}


node_t * Tree_Maximum(node_t *x){
	node_t * curr;

	curr = x;
	while(curr->right != T_nil){
		curr = curr->right;
	}
	return curr;
}


node_t * Tree_Successor(node_t *x){
	node_t *y, *z;

	if(x->right != T_nil){
		return Tree_Minimum(x->right);
	}
	else{
		z = x;
		y = z->p;
		while(y != T_nil && z == y->right){
			z = y;
			y = y->p;
		}
		return y;
	}
}


node_t * Tree_Predecessor(node_t *x){
	node_t *y, *z;

	if(x->left != T_nil){
		return Tree_Maximum(x->left);
	}
	else{
		z = x;
		y = x->p;
		while(y != T_nil && z == y->left){
			z = y;
			y = y->p;
		}
		return y;
	}
}


node_t * Left_Rotate(node_t *t, node_t *x){
	node_t *y;

	y = x->right;
	x->right = y->left;
	if(y->left != T_nil){
		y->left->p = x;
	}
	y->p = x->p;
	if(x->p == T_nil){
		t = y;
	}
	else if(x == x->p->left){
		x->p->left = y;
	}
	else{
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
	return t;
}


node_t * Right_Rotate(node_t *t, node_t *y){
	node_t *x;

	x = y->left;
	y->left = x->right;
	if(x->right != T_nil){
		x->right->p = y;
	}
	x->p =  y->p;
	if(y->p == T_nil){
		t = x;
	}
	else if(y == y->p->right){
		y->p->right = x;
	}
	else{
		y->p->left = x;
	}
	x->right = y;
	y->p = x;
	return t;
}


node_t * RB_Insert(node_t *t, node_t *z){
	node_t *x, *y;

	x = t;
	y = T_nil;
	while(x != T_nil){
		y = x;
		if(z->val < x->val){
			x = x->left;
		}
		else if(z->val > x->val){
			x = x->right;
		}
		else{
			return NULL;
		}
	}
	z->p = y;
	if(y == T_nil){
		t = z;
	}
	else if(z->val < y->val){
		y->left = z;
	}else{
		y->right = z;
	}
	z->left = T_nil;
	z->right = T_nil;
	z->color = RED;
	t = RB_Insert_Fixup(t, z);
	return t;
}


node_t * RB_Insert_Fixup(node_t *t, node_t *z){
	node_t *y;

	while(z->p->color == RED){  	//while the added node has a red parent
		if(z->p == z->p->p->left){  //Is z's parent a left child?
			y = z->p->p->right;	
			if(y->color == RED){  //Are z's parent and uncle both red?	Case 1
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}else{
				if (z == z->p->right){			//Case 2
					z = z->p;
					t = Left_Rotate(t, z);
				}
				z->p->color = BLACK;			//Case 3
				z->p->p->color = RED;
				t = Right_Rotate(t, z->p->p);
			}
		}
		else{
			y = z->p->p->left;
			if(y->color == RED){  //Are z's parent and uncle both red?	Case 1
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}else{
				if (z == z->p->left){			//Case 2
					z = z->p;
					t = Right_Rotate(t, z);
				}
				z->p->color = BLACK;			//Case 3
				z->p->p->color = RED;
				t = Left_Rotate(t, z->p->p);
			}
		}
	}
	t->color = BLACK;
	return t;
}


node_t * RB_Transplant(node_t *t, node_t *u, node_t *v){

	if(u->p == T_nil){
		t = v;
	}
	else if(u == u->p->left)
	{
		u->p->left = v;
	}
	else{
		u->p->right = v;
	}
	v->p = u->p;
	return t;
}


node_t * RB_Delete(node_t *t, node_t *z){
	int y_original_color;
	node_t *x = NULL, *y = NULL;
	
	y = z;
	y_original_color = y->color;
	if(z->left == T_nil){
		x = z->right;
		t = RB_Transplant(t, z, z->right);	//Replace z by its right child
	}
	else if(z->right == T_nil)
	{
		x = z->left;
		t = RB_Transplant(t, z, z->left);	//Replace z by its left child
	}
	else
	{
		y = Tree_Minimum(z->right);		//y is z's successor
		y_original_color = y->color;
		x = y->right;
		if(y != z->right)
		{
			t = RB_Transplant(t, y, y->right);	//Replace y by its right child
			y->right = z->right;				//z’s right child becomes y's right child
			y->right->p = y;				
		}else{									//In case x is T_nil
			x->p = y;
		}
		t = RB_Transplant(t, z, y);				//Replace z by its successor y and give z's left
		y->left = z->left;						//child to y, which had no left child
		y->left->p = y;
		y->color = z->color;
	}
	free(z);
	if(y_original_color == BLACK){  		//If any RB violations occurred
		t = RB_Delete_Fixup(t, x);			//Correct them
	}
	return t;
}


node_t * RB_Delete_Fixup(node_t *t, node_t *x){
	node_t *w;

	while(x != t && x->color == BLACK){		//While x is black
		if(x == x->p->left){		//Is x a left child?
			w = x->p->right;		//w is x's sibling
			if(w->color == RED){  	//Case 1
				w->color = BLACK;  
				x->p->color = RED;
				t = Left_Rotate(t, x->p);
				w = x->p->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK){	//Case 2
				w->color = RED;
				x = x->p;
			}else{
				if(w->right->color == BLACK){		//Case 3
					w->left->color = BLACK;
					w->color = RED;
					t = Right_Rotate(t, w);
					w = x->p->right;
				}
				w->color = x->p->color;		//Case 4
				x->p->color = BLACK;
				w->right->color = BLACK;
				t = Left_Rotate(t, x->p);
				x = t;
			}
		}
		else{		//If x is a right child
			w = x->p->left;
			if(w->color == RED){		//Case 1
				w->color = BLACK;
				x->p->color = RED;
				t = Right_Rotate(t, x->p);
				w = x->p->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK){	//Case 2
				w->color = RED;
				x = x->p;
			}else{
				if(w->left->color == BLACK){		//Case 3
					w->right->color = BLACK;
					w->color = RED;
					t = Left_Rotate(t, w);
					w = x->p->left;
				}
				w->color = x->p->color;			//Case 4
				x->p->color = BLACK;
				w->left->color = BLACK;
				t = Right_Rotate(t, x->p);
				x = t;
			}
		}
	}
	x->color = BLACK;
	return t;
}