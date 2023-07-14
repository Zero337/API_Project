#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node_s{
	int val;
	int color;
	struct node_s *left, *right, *p;
}node_t;


void InOrder_Tree_Walk(node_t *);

node_t * Tree_Search(node_t *, int);

node_t * Tree_Minimum(node_t *);

node_t * Tree_Maximum(node_t *);

node_t * Tree_Successor(node_t *);

node_t * Left_Rotate(node_t *,node_t *);

node_t * Right_Rotate(node_t *,node_t*);

node_t * RB_Insert(node_t *,node_t *);

node_t * RB_Insert_Fixup(node_t *, node_t *);

node_t * RB_Transplant(node_t *, node_t *, node_t *);

node_t * RB_Delete(node_t *, node_t *);

node_t * RB_Delete_Fixup(node_t *, node_t *, node_t *);



int main(int argc, char const *argv[])
{
	node_t *root = NULL, *tmp = NULL;
	int i, x, y, z;

	x = 3;
	y = 4;
	z = 10;
	for (i = 0; i < 15; i++)
	{
		tmp = malloc(sizeof(node_t));
		if (tmp)
		{
			tmp->val = i;
			root = RB_Insert(root, tmp);
		}
		else{
			printf("Error in allocating memory!\n");
		}
	}

	InOrder_Tree_Walk(root);
	tmp = Tree_Search(root, x);
	if(tmp != NULL){
		root = RB_Delete(root, tmp);
	}
	InOrder_Tree_Walk(root);
	tmp = Tree_Search(root, y);
	if(tmp != NULL){
		root = RB_Delete(root, tmp);
	}
	InOrder_Tree_Walk(root);	
	tmp = Tree_Search(root, z);
	if(tmp != NULL){
		root = RB_Delete(root, tmp);
	}
	InOrder_Tree_Walk(root);

	return 0;
}



void InOrder_Tree_Walk(node_t *x){
	if(x != NULL){
		InOrder_Tree_Walk(x->left);
		printf("%d\n", x->val);
		InOrder_Tree_Walk(x->right);
	}
}


node_t * Tree_Search(node_t *x, int k){
	node_t * curr;

	curr = x;
	if (curr == NULL || k == curr->val)
	{
		return curr;
	}
	if (k < curr->val)
	{
		return Tree_Search(curr->left, k);
	}else{
		return Tree_Search(curr->right, k);
	}
}


node_t * Tree_Minimum(node_t *x){
	node_t *curr;

	curr = x;
	while(curr->left != NULL){
		curr = curr->left;
	}
	return curr;
}


node_t * Tree_Maximum(node_t *x){
	node_t * curr;

	curr = x;
	while(curr->right != NULL){
		curr = curr->right;
	}
	return curr;
}


node_t * Tree_Successor(node_t *x){
	node_t *y, *z;

	if(x->right != NULL){
		return Tree_Minimum(x->right);
	}
	else{
		z = x;
		y = z->p;
		while(y != NULL && z == y->right){
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
	if(y->left != NULL){
		y->left->p = x;
	}
	y->p = x->p;
	if(x->p == NULL){
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
	if(x->right != NULL){
		x->right->p = y;
	}
	x->p =  y->p;
	if(y->p == NULL){
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
	y = NULL;
	while(x != NULL){
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
	if(y == NULL){
		t = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		return t;
	}
	else if (z->val < y->val)
	{
		y->left = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		t = RB_Insert_Fixup(t, z);
		return t;
	}
	else{
		y->right = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		t = RB_Insert_Fixup(t, z);
		return t;
	}
}


node_t * RB_Insert_Fixup(node_t *t, node_t *z){
	node_t *y;

	while(z->p != NULL && z->p->p != NULL && z->p->color == 1){  /*while the added node has a red parent*/
		if(z->p == z->p->p->left){  //Is z's parent a left child?
			y = z->p->p->right;	
			if(y != NULL && y->color == 1){  //Are z's parent and uncle both red?
				z->p->color = 0;  //Black
				y->color = 0;  //Black
				z->p->p->color = 1;  //Red
				z = z->p->p;
			}else{
				if (z == z->p->right){
					z = z->p;
					t = Left_Rotate(t, z);
				}
				if(z->p != NULL){
					z->p->color = 0;  //Black
				}
				if(z->p->p != NULL){
					z->p->p->color = 1;  //Red
					t = Right_Rotate(t, z->p->p);
				}
			}
		}

		else{
			y = z->p->p->left;
			if(y != NULL && y->color == 1){  //Are z's parent and uncle both red?
				z->p->color = 0;  //Black
				y->color = 0;  //Black
				z->p->p->color = 1;  //Red
				z = z->p->p;
			}else{
				if (z == z->p->left){
					z = z->p;
					t = Right_Rotate(t, z);
				}
				if(z->p != NULL){
					z->p->color = 0;  //Black
				}
				if(z->p->p != NULL){
					z->p->p->color = 1;  //Red
					t = Left_Rotate(t, z->p->p);
				}
			}
		}
	}
	if (z->p != NULL && z->p->p == NULL && z->p->color == 1)  /*if z's parent is the root and it's red*/
	{
		z->p->color = 0;  /*turn z's parent's (the root's) color to black*/
	}
	return t;
}


node_t * RB_Transplant(node_t *t, node_t *u, node_t *v){

	if(u->p == NULL){
		t = v;
	}
	else if(u == u->p->left)
	{
		u->p->left = v;
	}
	else{
		u->p->right = v;
	}
	if(v != NULL){
		v->p = u->p;
	}
	return t;
}


node_t * RB_Delete(node_t *t, node_t *z){
	int y_original_color;
	node_t *x = NULL, *y = NULL, *parent = NULL;
	
	y = z;
	y_original_color = y->color;
	if(z->left == NULL){
		x = z->right;
		parent = z->p;
		t = RB_Transplant(t, z, z->right);	//Replace z by its right child
	}
	else if(z->right == NULL)
	{
		x = z->left;
		parent = z->p;
		t = RB_Transplant(t, z, z->left);	//Replace z by its left child
	}
	else
	{
		y = Tree_Minimum(z->right);		//y is z's successor
		y_original_color = y->color;
		x = y->right;
		if(y != z->right)
		{
			parent = y->p;
			t = RB_Transplant(t, y, y->right);	//Replace y by its right child
			y->right = z->right;
			y->right->p = y;
			y->left = z->left;
			y->left->p = y;
			if(z->p != NULL){		//Is z different from the root of the tree?
				if(z == z->p->left){	//Is z a left child?
					y->p = z->p;
					y->p->left = y;
				}
				else{
					y->p = z->p;
					y->p->right = y;
				}
			}else
			{
				y->p = z->p;
				t = y;
			}
		}
		else
		{
			parent = y;
			t = RB_Transplant(t, z, y);
			y->left = z->left;
			y->left->p = y;
			y->color = z->color;
			y->p = z->p; 
		}
	}
	free(z);
	if(y_original_color == 0){  				//If any RB violations occurred
		t = RB_Delete_Fixup(t, x, parent);		//Correct them
	}
	return t;
}


node_t * RB_Delete_Fixup(node_t *t, node_t *x, node_t *parent){
	node_t *w;

	while((x != t && x == NULL) || (x != t && x->color == 0)){		//While x is black or NULL
		if(x == parent->left){		//Is x a left child?
			w = parent->right;		//w is x's sibling
			if(w->color == 1){  	//Case 1
				w->color = 0;  
				parent->color = 1;
				t = Left_Rotate(t, parent);
				w = parent->right;
			}
			if(w->left == NULL){
				if(w->right == NULL || w->right->color == 0){
					w->color = 0;
					x = parent;
					parent = x->p;
				}else{
					if((w->right == NULL) || (w->right->color == 0)){		//Case 3
						w->left->color = 0;
						w->color = 1;
						t = Right_Rotate(t, w);
						w = parent->right;
					}
					w->color = parent->color;		//Case 4
					parent->color = 0;
					w->right->color = 0;
					t = Left_Rotate(t, parent);
					x = t;
				}
			}else if(w->right == NULL){
				if(w->left->color == 0){
					w->color = 0;
					x = parent;
					parent = x->p;
				}else{
					if((w->right == NULL) || (w->right->color == 0)){		//Case 3
						w->left->color = 0;
						w->color = 1;
						t = Right_Rotate(t, w);
						w = parent->right;
					}
					w->color = parent->color;		//Case 4
					parent->color = 0;
					w->right->color = 0;
					t = Left_Rotate(t, parent);
					x = t;
				}
			}else if(w->right->color == 0 && w->left->color == 0){
				w->color = 0;
				x = parent;
				parent = x->p;
			}else{
				if((w->right == NULL) || (w->right->color == 0)){		//Case 3
					w->left->color = 0;
					w->color = 1;
					t = Right_Rotate(t, w);
					w = parent->right;
				}
				w->color = parent->color;		//Case 4
				parent->color = 0;
				w->right->color = 0;
				t = Left_Rotate(t, parent);
				x = t;
			}
		}
		else{
			w = parent->left;
			if(w->color == 1){
				w->color = 0;
				parent->color = 1;
				t = Right_Rotate(t, parent);
				w = parent->left;
			}
			if(w->left == NULL){
				if(w->right == NULL || w->right->color == 0){
					w->color = 0;
					x = parent;
					parent = x->p;
				}else{
					if((w->left == NULL) || (w->left->color == 0)){
						w->right->color = 0;
						w->color = 1;
						t = Left_Rotate(t, w);
						w = parent->left;
					}
					w->color = parent->color;
					parent->color = 0;
					w->left->color = 0;
					t = Right_Rotate(t, parent);
					x = t;
				}
			}else if(w->right == NULL){
				if(w->left->color == 0){
					w->color = 0;
					x = parent;
					parent = x->p;
				}else{
					if((w->left == NULL) || (w->left->color == 0)){
						w->right->color = 0;
						w->color = 1;
						t = Left_Rotate(t, w);
						w = parent->left;
					}
					w->color = parent->color;
					parent->color = 0;
					w->left->color = 0;
					t = Right_Rotate(t, parent);
					x = t;
				}
			}else if(w->right->color == 0 && w->left->color == 0){
				w->color = 0;
				x = parent;
				parent = x->p;
			}else{
				if((w->left == NULL) || (w->left->color == 0)){
					w->right->color = 0;
					w->color = 1;
					t = Left_Rotate(t, w);
					w = parent->left;
				}
				w->color = parent->color;
				parent->color = 0;
				w->left->color = 0;
				t = Right_Rotate(t, parent);
				x = t;
			}
		}
	}
	if(x != NULL){
		x->color = 0;
	}
	return t;
}


