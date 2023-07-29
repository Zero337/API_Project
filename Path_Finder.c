#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define ND -1
#define RED 1
#define BLACK 0


typedef struct car_s{
	int range;
	bool color;
	struct car_s *left, *right, *p;
}car_t;


typedef struct station_s{
	int distance;
	bool color;
	int max_range, id;
	struct station_s *left, *right, *p;
	struct station_s *successor, *predecessor;
	struct car_s *cars;
}station_t;


typedef struct stack_s{
	int distance;
	struct stack_s *next;
}stack_t;


typedef struct queue_s{
	struct station_s *station_ptr;
	struct queue_s *next;
	struct queue_s *previous;
}queue_t;


station_t * T_nil_station = NULL;

car_t * T_nil_car = NULL;

void InOrder_Tree_Walk(station_t *);

station_t * Tree_Search_station(station_t *, int);

car_t * Tree_Search_car(car_t *, int);

station_t * Tree_Minimum_station(station_t *);

car_t * Tree_Minimum_car(car_t *);

car_t * Tree_Maximum_car(car_t *);

station_t * Tree_Successor(station_t *);

station_t * Tree_Predecessor(station_t *);

station_t * Left_Rotate_station(station_t *, station_t *);

car_t * Left_Rotate_car(car_t *, car_t *);

station_t * Right_Rotate_station(station_t *, station_t *);

car_t * Right_Rotate_car(car_t *, car_t *);

car_t * RB_Insert_car(car_t*, car_t*);

station_t * RB_Insert_station(station_t *, station_t *);

station_t * RB_Insert_Fixup_station(station_t *, station_t *);

car_t * RB_Insert_Fixup_car(car_t *, car_t *);

station_t * RB_Transplant_station(station_t *, station_t *, station_t *);

car_t * RB_Transplant_car(car_t *, car_t *, car_t *);

station_t * RB_Delete_station(station_t *, station_t *);

car_t * RB_Delete_car(car_t *, car_t *);

station_t * RB_Delete_Fixup_station(station_t *, station_t *);

car_t * RB_Delete_Fixup_car(car_t *, car_t *);

stack_t * push(stack_t *, stack_t *);

stack_t * pop(stack_t *);

queue_t * enqueue(queue_t *, queue_t *);

queue_t * dequeue(queue_t *);

void path_finder(station_t *, station_t *, int);



int main(int argc, char const *argv[])
{
	char command[20];
	int distance, number_of_cars, range, i, starting_station, arrival_station, count;
	station_t *station_root = NULL, *tmp_station = NULL, *start = NULL, *finish = NULL, *tmp_successor = NULL, *tmp_predecessor = NULL;
	car_t *car_root = NULL, *tmp_car = NULL, *car_ptr = NULL;


	count = 0;
	T_nil_car = malloc(sizeof(car_t));
	if(T_nil_car){
		T_nil_car->range = ND;
		T_nil_car->color = BLACK;
		T_nil_car->left = T_nil_car;
		T_nil_car->right = T_nil_car;
		T_nil_car->p = T_nil_car;
		car_root = T_nil_car;
	}else{
		printf("Error in memory allocation!\n");
	}
	T_nil_station = malloc(sizeof(station_t));
	if(T_nil_station){
		T_nil_station->distance = ND;
		T_nil_station->color = BLACK;
		T_nil_station->left = T_nil_station;
		T_nil_station->right = T_nil_station;
		T_nil_station->p = T_nil_station;
		T_nil_station->cars = T_nil_car;
		station_root = T_nil_station;
	}else{
		printf("Error in memory allocation!\n");
	}
	while(scanf("%s", command) != EOF)
	{

	if(strcmp(command, "aggiungi-stazione") == 0)
	{	
		if(scanf("%d %d", &distance, &number_of_cars) != EOF)
		{
			if(Tree_Search_station(station_root, distance) == T_nil_station){		//If the station is not already in the tree
				tmp_station = malloc(sizeof(station_t));	//Allocate the size of 1 station node
				if(tmp_station)
				{
					tmp_station->distance = distance;
					tmp_station->max_range = ND;
					tmp_station->id = count;
					count ++;
					station_root = RB_Insert_station(station_root, tmp_station);
					tmp_station->successor = Tree_Successor(tmp_station);
					if(tmp_station->successor != T_nil_station){
						tmp_station->successor->predecessor = tmp_station;
					}
					tmp_station->predecessor = Tree_Predecessor(tmp_station);
					if(tmp_station->predecessor != T_nil_station){
						tmp_station->predecessor->successor = tmp_station;
					}
					car_root = T_nil_car;
					for(i = 0; i < number_of_cars; i++)		//Loop for creating the car tree
					{
						if(scanf("%d", &range) != EOF)
						{
							tmp_car = malloc(sizeof(car_t));	//Allocate the size of 1 car node
							if(tmp_car)
							{
								tmp_car->range = range;
								car_root = RB_Insert_car(car_root, tmp_car);	//Insert the car node into the car tree
							}else{
							printf("Error in memory allocation!");
							}
						}
					}
					tmp_station->cars = car_root;
					car_ptr = Tree_Maximum_car(car_root);
					tmp_station->max_range = car_ptr->range;
					printf("aggiunta\n");
					}else
					{
						printf("Error in memory allocation!");
					}
				}else{
					printf("non aggiunta\n");
				}
			}
		}

		else if(strcmp(command, "demolisci-stazione") == 0)
		{
			if(scanf("%d", &distance) != EOF)
			{
				tmp_station = Tree_Search_station(station_root, distance);		//First we search for the station
				if(tmp_station != T_nil_station)
				{
					tmp_successor = Tree_Successor(tmp_station);
					tmp_predecessor = Tree_Predecessor(tmp_station);
					station_root = RB_Delete_station(station_root, tmp_station);	//Delete station if found
					tmp_successor->predecessor = Tree_Predecessor(tmp_successor);
					tmp_predecessor->successor = Tree_Successor(tmp_predecessor);
					printf("demolita\n");
				}else{
				printf("non demolita\n");
				}
			}
		}

		else if (strcmp(command, "aggiungi-auto") == 0)
		{	
			if(scanf("%d %d", &distance, &range) != EOF)
			{
				tmp_station = Tree_Search_station(station_root, distance);		//First we search for the station
				if (tmp_station != T_nil_station)
				{
					tmp_car = malloc(sizeof(car_t));	//Allocate the size of 1 car node
					if (tmp_car)
					{
						tmp_car->range = range;
						tmp_station->cars = RB_Insert_car(tmp_station->cars, tmp_car);		//Insert the car node into the car tree of the designated station
						if(range > tmp_station->max_range){
							tmp_station->max_range = range;
						}
						printf("aggiunta\n");
					}else{
						printf("Error in memory allocation!");
					}
				}else{
					printf("non aggiunta\n");
				}
			}
		}

		else if (strcmp(command, "rottama-auto") == 0)
		{
			if (scanf("%d %d", &distance, &range))
			{
				tmp_station = Tree_Search_station(station_root, distance);		//First we search for the station
				if (tmp_station != T_nil_station)
				{
					tmp_car = Tree_Search_car(tmp_station->cars, range);	//If the station is found we search for the car within the station's cars
					if (tmp_car != T_nil_car)
					{
						tmp_station->cars = RB_Delete_car(tmp_station->cars, tmp_car);		//If the car is found we proceed to delete it
						car_ptr = Tree_Maximum_car(tmp_station->cars);
						tmp_station->max_range = car_ptr->range;
						printf("rottamata\n");
					}else{
						printf("non rottamata\n");
					}
				}else{
					printf("non rottamata\n");
				}
			}
		}

		else if (strcmp(command, "pianifica-percorso") == 0)
		{
			if (scanf("%d %d", &starting_station, &arrival_station) != EOF)
			{
				start = Tree_Search_station(station_root, starting_station);
				finish = Tree_Search_station(station_root, arrival_station);
				if(start != T_nil_station && finish != T_nil_station){
					path_finder(start, finish, count);
				}else{
					printf("nessun percorso\n");
				}
			}
		}
	}
	return 0;
}


void InOrder_Tree_Walk(station_t *x){
	if(x != T_nil_station){
		InOrder_Tree_Walk(x->left);
		printf("%d\n", x->distance);
		InOrder_Tree_Walk(x->right);
	}
}


station_t * Tree_Search_station(station_t *x, int k){
	station_t * curr;

	curr = x;
	if (curr == T_nil_station || k == curr->distance)
	{
		return curr;
	}
	if (k < curr->distance)
	{
		return Tree_Search_station(curr->left, k);
	}else{
		return Tree_Search_station(curr->right, k);
	}
}


car_t * Tree_Search_car(car_t *x, int k){
	car_t * curr;

	curr = x;
	if (curr == T_nil_car || k == curr->range)
	{
		return curr;
	}
	if (k < curr->range)
	{
		return Tree_Search_car(curr->left, k);
	}else{
		return Tree_Search_car(curr->right, k);
	}
}


station_t * Tree_Minimum_station(station_t *x){
	station_t *curr;

	curr = x;
	while(curr->left != T_nil_station){
		curr = curr->left;
	}
	return curr;
}


car_t * Tree_Minimum_car(car_t *x){
	car_t *curr;

	curr = x;
	while(curr->left != T_nil_car){
		curr = curr->left;
	}
	return curr;
}


station_t * Tree_Maximum_station(station_t *x){
	station_t * curr;

	curr = x;
	while(curr->right != T_nil_station){
		curr = curr->right; 
	}
	return curr;
}


car_t * Tree_Maximum_car(car_t *x){
	car_t * curr;

	curr = x;
	while(curr->right != T_nil_car){
		curr = curr->right;
	}
	return curr;
}


station_t * Tree_Successor(station_t *x){
	station_t *y, *z;

	if(x->right != T_nil_station){
		return Tree_Minimum_station(x->right);
	}
	else{
		z = x;
		y = z->p;
		while(y != T_nil_station && z == y->right){
			z = y;
			y = y->p;
		}
		return y;
	}
}


station_t * Tree_Predecessor(station_t *x){
	station_t *y, *z;

	if(x->left != T_nil_station){
		return Tree_Maximum_station(x->left);
	}
	else{
		z = x;
		y = x->p;
		while(y != T_nil_station && z == y->left){
			z = y;
			y = y->p;
		}
		return y;
	}
}


station_t * Left_Rotate_station(station_t *t, station_t *x){
	station_t *y;

	y = x->right;
	x->right = y->left;
	if(y->left != T_nil_station){
		y->left->p = x;
	}
	y->p = x->p;
	if(x->p == T_nil_station){
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


car_t * Left_Rotate_car(car_t *t, car_t *x){
	car_t *y;

	y = x->right;
	x->right = y->left;
	if(y->left != T_nil_car){
		y->left->p = x;
	}
	y->p = x->p;
	if(x->p == T_nil_car){
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


station_t * Right_Rotate_station(station_t *t, station_t *y){
	station_t *x;

	x = y->left;
	y->left = x->right;
	if(x->right != T_nil_station){
		x->right->p = y;
	}
	x->p =  y->p;
	if(y->p == T_nil_station){
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


car_t * Right_Rotate_car(car_t *t, car_t *y){
	car_t *x;

	x = y->left;
	y->left = x->right;
	if(x->right != T_nil_car){
		x->right->p = y;
	}
	x->p =  y->p;
	if(y->p == T_nil_car){
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


station_t * RB_Insert_station(station_t *t, station_t *z){
	station_t *x, *y;

	x = t;
	y = T_nil_station;
	while(x != T_nil_station){
		y = x;
		if(z->distance < x->distance){
			x = x->left;
		}
		else{
			x = x->right;
		}
	}
	z->p = y;
	if(y == T_nil_station){
		t = z;
	}
	else if(z->distance < y->distance){
		y->left = z;
	}else{
		y->right = z;
	}
	z->left = T_nil_station;
	z->right = T_nil_station;
	z->color = RED;
	t = RB_Insert_Fixup_station(t, z);
	return t;
}


car_t * RB_Insert_car(car_t *t, car_t *z){
	car_t *x, *y;

	x = t;
	y = T_nil_car;
	while(x != T_nil_car){
		y = x;
		if(z->range < x->range){
			x = x->left;
		}
		else if(z->range > x->range){
			x = x->right;
		}
		else{
			if(x->right == T_nil_car){
				x = x->right;
				y->right = z;
			}
			else if(x->left == T_nil_car){
				x = x->left;
				y->left = z;
			}
			else{
				y = Tree_Minimum_car(x->right);
				x = y->left;
				y->left = z;
			}
		}
	}
	z->p = y;
	if(y == T_nil_car){
		t = z;
	}
	else if(z->range < y->range){
		y->left = z;
	}else if(z->range > y->range){
		y->right = z;
	}
	z->left = T_nil_car;
	z->right = T_nil_car;
	z->color = RED;
	t = RB_Insert_Fixup_car(t, z);
	return t;
}


station_t * RB_Insert_Fixup_station(station_t *t, station_t *z){
	station_t *y;

	while(z->p->color == RED){  	//while the added node has a red parent
		if(z->p == z->p->p->left){  //Is z's parent a left child?
			y = z->p->p->right;	
			if(y->color == RED){  //Are z's parent and uncle both red?	Case 1
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}else{
				if(z == z->p->right){			//Case 2
					z = z->p;
					t = Left_Rotate_station(t, z);
				}
				z->p->color = BLACK;			//Case 3
				z->p->p->color = RED;
				t = Right_Rotate_station(t, z->p->p);
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
				if(z == z->p->left){			//Case 2
					z = z->p;
					t = Right_Rotate_station(t, z);
				}
				z->p->color = BLACK;			//Case 3
				z->p->p->color = RED;
				t = Left_Rotate_station(t, z->p->p);
			}
		}
	}
	t->color = BLACK;	//Color the root black
	return t;
}


car_t * RB_Insert_Fixup_car(car_t *t, car_t *z){
	car_t *y;

	while(z->p->color == RED){  	//While the added node has a red parent
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
					t = Left_Rotate_car(t, z);
				}
				z->p->color = BLACK;			//Case 3
				z->p->p->color = RED;
				t = Right_Rotate_car(t, z->p->p);
			}
		}
		else{
			y = z->p->p->left;
			if(y->color == RED){  //Are z's parent and uncle both red?
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}else{
				if (z == z->p->left){			//Case 2
					z = z->p;
					t = Right_Rotate_car(t, z);
				}
				z->p->color = BLACK;			//Case 3
				z->p->p->color = RED;
				t = Left_Rotate_car(t, z->p->p);
			}
		}
	}
	t->color = BLACK;	//Color the root black
	return t;
}
	

station_t * RB_Transplant_station(station_t *t, station_t *u, station_t *v){

	if(u->p == T_nil_station){
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


car_t * RB_Transplant_car(car_t *t, car_t *u, car_t *v){

	if(u->p == T_nil_car){
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


station_t * RB_Delete_station(station_t *t, station_t *z){
	int y_original_color;
	station_t *x = NULL, *y = NULL;
	
	y = z;
	y_original_color = y->color;
	if(z->left == T_nil_station){
		x = z->right;
		t = RB_Transplant_station(t, z, z->right);	//Replace z by its right child
	}
	else if(z->right == T_nil_station)
	{
		x = z->left;
		t = RB_Transplant_station(t, z, z->left);	//Replace z by its left child
	}
	else
	{
		y = Tree_Minimum_station(z->right);		//y is z's successor
		y_original_color = y->color;
		x = y->right;
		if(y != z->right)
		{
			t = RB_Transplant_station(t, y, y->right);	//Replace y by its right child
			y->right = z->right;						//z’s right child becomes y's right child
			y->right->p = y;
		}else{											//In case x is T_nil
			x->p = y;
		}
		t = RB_Transplant_station(t, z, y);				//Replace z by its successor y and give z's left
		y->left = z->left;								//child to y, which had no left child
		y->left->p = y;
		y->color = z->color;
	}
	free(z);
	if(y_original_color == BLACK){					//If any RB violations occurred
		t = RB_Delete_Fixup_station(t, x);			//Correct them
	}
	return t;
}


car_t * RB_Delete_car(car_t *t, car_t *z){
	int y_original_color;
	car_t *x = NULL, *y = NULL;
	
	y = z;
	y_original_color = y->color;
	if(z->left == T_nil_car){
		x = z->right;
		t = RB_Transplant_car(t, z, z->right);	//Replace z by its right child
	}
	else if(z->right == T_nil_car)
	{
		x = z->left;
		t = RB_Transplant_car(t, z, z->left);	//Replace z by its left child
	}
	else
	{
		y = Tree_Minimum_car(z->right);		//y is z's successor
		y_original_color = y->color;
		x = y->right;
		if(y != z->right)
		{
			t = RB_Transplant_car(t, y, y->right);	//Replace y by its right child
			y->right = z->right;					//z’s right child becomes y's right child
			y->right->p = y;
		}else{										//In case x is T_nil
			x->p = y;
		}
		t = RB_Transplant_car(t, z, y);				//Replace z by its successor y and give z's left
		y->left = z->left;							//child to y, which had no left child
		y->left->p = y;
		y->color = z->color;
	}
	free(z);
	if(y_original_color == BLACK){  			//If any RB violations occurred
		t = RB_Delete_Fixup_car(t, x);			//Correct them
	}
	return t;
}


station_t * RB_Delete_Fixup_station(station_t *t, station_t *x){
	station_t *w;
	
	while(x != t && x->color == BLACK){		//While x is black
		if(x == x->p->left){		//Is x a left child?
			w = x->p->right;		//w is x's sibling
			if(w->color == RED){  	//Case 1
				w->color = BLACK;  
				x->p->color = RED;
				t = Left_Rotate_station(t, x->p);
				w = x->p->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK){	//Case 2
				w->color = RED;
				x = x->p;
			}else{
				if(w->right->color == BLACK){		//Case 3
					w->left->color = BLACK;
					w->color = RED;
					t = Right_Rotate_station(t, w);
					w = x->p->right;
				}
				w->color = x->p->color;		//Case 4
				x->p->color = BLACK;
				w->right->color = BLACK;
				t = Left_Rotate_station(t, x->p);
				x = t;
			}
		}
		else{		//If x is a right child
			w = x->p->left;
			if(w->color == RED){		//Case 1
				w->color = BLACK;
				x->p->color = RED;
				t = Right_Rotate_station(t, x->p);
				w = x->p->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK){	//Case 2
				w->color = RED;
				x = x->p;
			}else{
				if(w->left->color == BLACK){		//Case 3
					w->right->color = BLACK;
					w->color = RED;
					t = Left_Rotate_station(t, w);
					w = x->p->left;
				}
				w->color = x->p->color;			//Case 4
				x->p->color = BLACK;
				w->left->color = BLACK;
				t = Right_Rotate_station(t, x->p);
				x = t;
			}
		}
	}
	x->color = BLACK;
	return t;
}


car_t * RB_Delete_Fixup_car(car_t *t, car_t *x){
	car_t *w;
	
	while(x != t && x->color == BLACK){		//While x is black
		if(x == x->p->left){		//Is x a left child?
			w = x->p->right;		//w is x's sibling
			if(w->color == RED){  	//Case 1
				w->color = BLACK;  
				x->p->color = RED;
				t = Left_Rotate_car(t, x->p);
				w = x->p->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK){	//Case 2
				w->color = RED;
				x = x->p;
			}else{
				if(w->right->color == BLACK){		//Case 3
					w->left->color = BLACK;
					w->color = RED;
					t = Right_Rotate_car(t, w);
					w = x->p->right;
				}
				w->color = x->p->color;		//Case 4
				x->p->color = BLACK;
				w->right->color = BLACK;
				t = Left_Rotate_car(t, x->p);
				x = t;
			}
		}
		else{		//If x is a right child
			w = x->p->left;
			if(w->color == RED){		//Case 1
				w->color = BLACK;
				x->p->color = RED;
				t = Right_Rotate_car(t, x->p);
				w = x->p->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK){
				w->color = RED;
				x = x->p;
			}else{
				if(w->left->color == BLACK){		//Case 3
					w->right->color = BLACK;
					w->color = RED;
					t = Left_Rotate_car(t, w);
					w = x->p->left;
				}
				w->color = x->p->color;			//Case 4
				x->p->color = BLACK;
				w->left->color = BLACK;
				t = Right_Rotate_car(t, x->p);
				x = t;
			}
		}
	}
	x->color = BLACK;
	return t;
}


stack_t * push(stack_t *s, stack_t *z){
	z->next = s;
	s = z;
	return s;
}


stack_t * pop(stack_t *s){
	stack_t *head = NULL;

	head = s;
	s = s->next;
	free(head);

	return s;
}


queue_t * enqueue(queue_t *t, queue_t *z){		//We will use the tail as reference and modify it here while the head is modified in the main function when there is need
	if(t == NULL){
		t = z;
		z->next = NULL;
		z->previous = NULL;
	}
	else{
		z->previous = t;
		z->next = NULL;
		t->next = z;
		t = z;
	}
	return t;
}


queue_t * dequeue(queue_t *h){
	queue_t *head = NULL;

	head = h;
	h = h->next;
	if(h != NULL){
		h->previous = NULL;
	}
	free(head);

	return h;
}


void path_finder(station_t *start, station_t *finish, int max_len){
	station_t *tmp_stop = NULL, *farthest_stop = NULL, *parents[max_len], *limit = NULL, *border = NULL, *station_ptr = NULL;
	queue_t *head = NULL, *tail = NULL, *tmp_tail = NULL;
	int level[max_len], lvl, max_car_range, i;
	stack_t *stack_ptr = NULL, *stops = NULL;

	if(start->distance == finish->distance){		//Arrival station is the same as the starting one.
		printf("%d\n", start->distance);
		return;
	}
	else if(start->distance < finish->distance)		//Starting station is closer to the start of the highway (first direction)
	{
		for(i = 0; i < max_len; i++){		//Initialize the arrays with -1 and T_nil_station and lvl at 0
			level[i] = -1;
			parents[i] = T_nil_station;
		}
		lvl = 0;
		tmp_tail = malloc(sizeof(queue_t));		//Enqueue the starting station into the "nodes to be visited" queue
		if(tmp_tail){
			tmp_tail->station_ptr = start;
			level[start->id] = lvl;
			tail = enqueue(tail, tmp_tail);
			head = tail;
		}else{
			printf("Error in memory allocation!\n");
		}
		limit = start;
		while(head != NULL && head->station_ptr != finish){		//While there are still nodes to visit and we haven't found the destination node
			tmp_stop = head->station_ptr;
			lvl = level[tmp_stop->id] + 1;
			head = dequeue(head);
			if(head == NULL){
				tail = NULL;
			}
			farthest_stop = limit->successor;		//Enqueue all the reachable stations from the tmp_stop which haven't already been enqueued
			max_car_range = tmp_stop->max_range;
			while(farthest_stop != T_nil_station){
				if(max_car_range >= (farthest_stop->distance - tmp_stop->distance)){
					tmp_tail = malloc(sizeof(queue_t));
					if(tmp_tail){
						tmp_tail->station_ptr = farthest_stop;
						level[farthest_stop->id] = lvl;
						parents[farthest_stop->id] = tmp_stop;
						tail = enqueue(tail, tmp_tail);
						limit = farthest_stop;
						if(head == NULL){
							head = tail;
						}
					}else{
						printf("Error in memory allocation!\n");
					}
					farthest_stop = farthest_stop->successor;
				}else{
					farthest_stop = T_nil_station;
				}
			}
		}
		if(head != NULL){		//If we find the destination node
			tmp_stop = head->station_ptr;
			while(tmp_stop != T_nil_station){		//Push the stops on the path into the stack
				stack_ptr = malloc(sizeof(stack_t));
				if(stack_ptr){
					stack_ptr->distance = tmp_stop->distance;
					stops = push(stops, stack_ptr);
					tmp_stop = parents[tmp_stop->id];
				}else{
					printf("Error in memory allocation!\n");
				}
			}	
		}
		if(stops != NULL){
			stack_ptr = stops;
			while(stack_ptr->next != NULL){
				printf("%d ", stack_ptr->distance);
				stack_ptr = stack_ptr->next;
			}
			printf("%d\n", stack_ptr->distance);
			return;
		}else{
			printf("nessun percorso\n");
			return;
		}
	}else  				//Arrival station is closer to the start of the highway (second direction)
	{
		for(i = 0; i < max_len; i++){		//Initialize the arrays at -1 and T_nil_station and lvl at 0
			level[i] = -1;
			parents[i] = T_nil_station;
		}
		lvl = 0;
		tmp_tail = malloc(sizeof(queue_t));		//Enqueue the starting station into the "nodes to be visited" queue
		if(tmp_tail){
			tmp_tail->station_ptr = start;
			level[start->id] = lvl;
			tail = enqueue(tail, tmp_tail);
			head = tail;
		}else{
			printf("Error in memory allocation!\n");
		}
		limit = start;
		border = start;
		while(head != NULL && head->station_ptr != finish){		//While there are still nodes to visit and we haven't found the destination node
			tmp_stop = head->station_ptr;
			lvl = level[tmp_stop->id] + 1;
			head = dequeue(head);
			if(head == NULL){
				tail = NULL;
			}
			farthest_stop = limit->predecessor;		//Enqueue all the reachable stations from the tmp_stop which haven't already been enqueued
			max_car_range = tmp_stop->max_range;
			while(farthest_stop != T_nil_station){
				if(max_car_range >= (tmp_stop->distance - farthest_stop->distance)){
					tmp_tail = malloc(sizeof(queue_t));
					if(tmp_tail){
						tmp_tail->station_ptr = farthest_stop;
						level[farthest_stop->id] = lvl;
						parents[farthest_stop->id] = tmp_stop;
						tail = enqueue(tail, tmp_tail);
						limit = farthest_stop;
						if(head == NULL){
							head = tail;
						}
						station_ptr = border;
						while(station_ptr != start){
							if(station_ptr->max_range >= (station_ptr->distance - farthest_stop->distance)){
								parents[farthest_stop->id] = station_ptr;
								station_ptr = start;
							}
							else{
								station_ptr = station_ptr->successor;
							}
						}
					}else{
						printf("Error in memory allocation!\n");
					}
					farthest_stop = farthest_stop->predecessor;
				}else{
					farthest_stop = T_nil_station;
				}
			}
			if(head != NULL && level[tmp_stop->id] != level[head->station_ptr->id]){
				border = tail->station_ptr;
			}
		}
		if(head != NULL){		//If we find the destination node
			tmp_stop = head->station_ptr;
			while(tmp_stop != T_nil_station){		//Push the stops on the path into the stack
				stack_ptr = malloc(sizeof(stack_t));
				if(stack_ptr){
					stack_ptr->distance = tmp_stop->distance;
					stops = push(stops, stack_ptr);
					tmp_stop = parents[tmp_stop->id];
				}else{
					printf("Error in memory allocation!\n");
				}
			}	
		}
		if(stops != NULL){
			stack_ptr = stops;
			while(stack_ptr->next != NULL){
				printf("%d ", stack_ptr->distance);
				stack_ptr = stack_ptr->next;
			}
			printf("%d\n", stack_ptr->distance);
			return;
		}else{
			printf("nessun percorso\n");
			return;
		}
	}
}