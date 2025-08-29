#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct car_s{
	int range;
	int color;
	struct car_s *left, *right, *p;
}car_t;


typedef struct station_s{
	int distance;
	int color;
	struct station_s *left, *right, *p;
	struct car_s *cars;
}station_t;


typedef struct stack_s{
	struct station_s *station_ptr;
	struct stack_s *next;
}stack_t;


typedef struct queue_s{
	struct station_s *station_ptr;
	struct queue_s *next;
	struct queue_s *previous;
}queue_t;



void InOrder_Tree_Walk(station_t *);

station_t * Tree_Search_station(station_t *, int);

car_t * Tree_Search_car(car_t *, int);

station_t * Tree_Minimum_station(station_t *);

car_t * Tree_Minimum_car(car_t *);

car_t * Tree_Maximum_car(car_t *);

station_t * Tree_Successor(station_t *);

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

station_t * RB_Delete_Fixup_station(station_t *, station_t *, station_t *);

car_t * RB_Delete_Fixup_car(car_t *, car_t *, car_t *);

stack_t * push(stack_t *, stack_t *);

stack_t * pop(stack_t *);

queue_t * enqueue(queue_t *, queue_t *);

queue_t * dequeue(queue_t *);

void path_finder(station_t *, station_t *);

stack_t * shorter_path_stack(station_t *, station_t *, station_t *, int);

queue_t * shorter_path_queue(station_t *, station_t *, station_t *, int);



int main(int argc, char const *argv[])
{
	FILE* fd;
	char command[20];
	int distance, number_of_cars, range, i, starting_station, arrival_station;
	station_t *station_root = NULL, *tmp_station = NULL, *start = NULL, *finish = NULL;
	car_t *car_root = NULL, *tmp_car = NULL;


	fd = fopen("archivio_test_aperti/open_10.txt", "r");
	if(fd)
	{
		while(fscanf(fd, "%s", command) != EOF)
		{

		if(strcmp(command, "aggiungi-stazione") == 0)
		{	
			if(fscanf(fd, "%d %d", &distance, &number_of_cars) != EOF)
			{
				if(Tree_Search_station(station_root, distance) == NULL){		//If the station is not already in the tree
					tmp_station = malloc(sizeof(station_t));	//Allocate the size of 1 station node
					if(tmp_station)
					{
						tmp_station->distance = distance;
						tmp_station->cars = NULL;
						station_root = RB_Insert_station(station_root, tmp_station);
						car_root = NULL;
						for(i = 0; i < number_of_cars; i++)		//Loop for creating the car tree
						{
							if(fscanf(fd, "%d", &range) != EOF)
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
				if(fscanf(fd, "%d", &distance) != EOF)
				{
					tmp_station = Tree_Search_station(station_root, distance);		//First we search for the station
					if(tmp_station != NULL)
					{
						station_root = RB_Delete_station(station_root, tmp_station);	//Delete station if found
						printf("demolita\n");
					}else{
					printf("non demolita\n");
					}
				}
			}

			else if (strcmp(command, "aggiungi-auto") == 0)
			{	
				if(fscanf(fd, "%d %d", &distance, &range) != EOF)
				{
					tmp_station = Tree_Search_station(station_root, distance);		//First we search for the station
					if (tmp_station != NULL)
					{
						tmp_car = malloc(sizeof(car_t));	//Allocate the size of 1 car node
						if (tmp_car)
						{
							tmp_car->range = range;
							tmp_station->cars = RB_Insert_car(tmp_station->cars, tmp_car);		//Insert the car node into the car tree of the designated station
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
				if (fscanf(fd, "%d %d", &distance, &range))
				{
					tmp_station = Tree_Search_station(station_root, distance);		//First we search for the station
					if (tmp_station != NULL)
					{
						tmp_car = Tree_Search_car(tmp_station->cars, range);	//If the station is found we search for the car within the station's cars
						if (tmp_car != NULL)
						{
							tmp_station->cars = RB_Delete_car(tmp_station->cars, tmp_car);		//If the car is found we proceed to delete it
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
				if (fscanf(fd, "%d %d", &starting_station, &arrival_station) != EOF)
				{
					start = Tree_Search_station(station_root, starting_station);
					finish = Tree_Search_station(station_root, arrival_station);
					if(start != NULL && finish != NULL){
						path_finder(start, finish);
					}else{
						printf("nessun percorso\n");
					}
				}
			}
		}
		fclose(fd);
	}else{
		printf("Error in opening the file!");
	}
	return 0;
}


void InOrder_Tree_Walk(station_t *x){
	if(x != NULL){
		InOrder_Tree_Walk(x->left);
		printf("%d\n", x->distance);
		InOrder_Tree_Walk(x->right);
	}
}


station_t * Tree_Search_station(station_t *x, int k){
	station_t * curr;

	curr = x;
	if (curr == NULL || k == curr->distance)
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
	if (curr == NULL || k == curr->range)
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
	while(curr->left != NULL){
		curr = curr->left;
	}
	return curr;
}


car_t * Tree_Minimum_car(car_t *x){
	car_t *curr;

	curr = x;
	while(curr->left != NULL){
		curr = curr->left;
	}
	return curr;
}


station_t * Tree_Maximum_station(station_t *x){
	station_t * curr;

	curr = x;
	while(curr->right != NULL){
		curr = curr->right; 
	}
	return curr;
}


car_t * Tree_Maximum_car(car_t *x){
	car_t * curr;

	curr = x;
	while(curr->right != NULL){
		curr = curr->right;
	}
	return curr;
}


station_t * Tree_Successor(station_t *x){
	station_t *y, *z;

	if(x->right != NULL){
		return Tree_Minimum_station(x->right);
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


station_t * Tree_Predecessor(station_t *x){
	station_t *y, *z;

	if(x->left != NULL){
		return Tree_Maximum_station(x->left);
	}
	else{
		z = x;
		y = x->p;
		while(y != NULL && z == y->left){
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


car_t * Left_Rotate_car(car_t *t, car_t *x){
	car_t *y;

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


station_t * Right_Rotate_station(station_t *t, station_t *y){
	station_t *x;

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


car_t * Right_Rotate_car(car_t *t, car_t *y){
	car_t *x;

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


station_t * RB_Insert_station(station_t *t, station_t *z){
	station_t *x, *y;

	x = t;
	y = NULL;
	while(x != NULL){
		y = x;
		if(z->distance < x->distance){
			x = x->left;
		}
		else{
			x = x->right;
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
	else if (z->distance < y->distance)
	{
		y->left = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		t = RB_Insert_Fixup_station(t, z);
		return t;
	}
	else{
		y->right = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		t = RB_Insert_Fixup_station(t, z);
		return t;
	}
}


car_t * RB_Insert_car(car_t *t, car_t *z){
	car_t *x, *y;

	x = t;
	y = NULL;
	while(x != NULL){
		y = x;
		if(z->range < x->range){
			x = x->left;
		}
		else if(z->range > x->range){
			x = x->right;
		}
		else{
			if(x->right == NULL){
				x = x->right;
			}
			else if(x->left == NULL){
				x = x->left;
			}
			else{
				y = Tree_Minimum_car(x->right);
				x = y->left;
			}
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
	else if(z->range < y->range)
	{
		y->left = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		t = RB_Insert_Fixup_car(t, z);
		return t;
	}
	else if(z->range > y->range)
	{
		y->right = z;
		z->left = NULL;
		z->right = NULL;
		z->color = 1;  //Red
		t = RB_Insert_Fixup_car(t, z);
		return t;
	}
	else
	{
		if(y->right == NULL){
			y->right = z;
			z->left = NULL;
			z->right = NULL;
			z->color = 1;  //Red
			t = RB_Insert_Fixup_car(t, z);
			return t;
		}
		else{
			y->left = z;
			z->left = NULL;
			z->right = NULL;
			z->color = 1;  //Red
			t = RB_Insert_Fixup_car(t, z);
			return t;
		}
	}
}


station_t * RB_Insert_Fixup_station(station_t *t, station_t *z){
	station_t *y;

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
					t = Left_Rotate_station(t, z);
				}
				if(z->p != NULL){
					z->p->color = 0;  //Black
				}
				if(z->p->p != NULL){
					z->p->p->color = 1;  //Red
					t = Right_Rotate_station(t, z->p->p);
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
					t = Right_Rotate_station(t, z);
				}
				if(z->p != NULL){
					z->p->color = 0;  //Black
				}
				if(z->p->p != NULL){
					z->p->p->color = 1;  //Red
					t = Left_Rotate_station(t, z->p->p);
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


car_t * RB_Insert_Fixup_car(car_t *t, car_t *z){
	car_t *y;

	while(z->p != NULL && z->p->p != NULL && z->p->color == 1){  /*while the added node has a red parent*/
		if(z->p == z->p->p->left){  //Is z's parent a left child?
			y = z->p->p->right;	
			if(y != NULL && y->color == 1){  //Are z's parent and uncle both red?
				z->p->color = 0;  /*black*/
				y->color = 0;  /*black*/
				z->p->p->color = 1;  /*red*/
				z = z->p->p;
			}else{
				if (z == z->p->right){
					z = z->p;
					t = Left_Rotate_car(t, z);
				}
				if(z->p != NULL){
					z->p->color = 0;  /*black*/
				}
				if(z->p->p != NULL){
					z->p->p->color = 1;  /*red*/
					t = Right_Rotate_car(t, z->p->p);
				}
			}
		}

		else{
			y = z->p->p->left;
			if(y != NULL && y->color == 1){  //Are z's parent and uncle both red?
				z->p->color = 0;  /*black*/
				y->color = 0;  /*black*/
				z->p->p->color = 1;  /*red*/
				z = z->p->p;
			}else{
				if (z == z->p->left){
					z = z->p;
					t = Right_Rotate_car(t, z);
				}
				if(z->p != NULL){
					z->p->color = 0;  /*black*/
				}
				if(z->p->p != NULL){
					z->p->p->color = 1;  /*red*/
					t = Left_Rotate_car(t, z->p->p);
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
	

station_t * RB_Transplant_station(station_t *t, station_t *u, station_t *v){

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


car_t * RB_Transplant_car(car_t *t, car_t *u, car_t *v){

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


station_t * RB_Delete_station(station_t *t, station_t *z){
	int y_original_color;
	station_t *x = NULL, *y = NULL, *parent = NULL;
	
	y = z;
	y_original_color = y->color;
	if(z->left == NULL){
		x = z->right;
		parent = z->p;
		t = RB_Transplant_station(t, z, z->right);	//Replace z by its right child
	}
	else if(z->right == NULL)
	{
		x = z->left;
		parent = z->p;
		t = RB_Transplant_station(t, z, z->left);	//Replace z by its left child
	}
	else
	{
		y = Tree_Minimum_station(z->right);		//y is z's successor
		y_original_color = y->color;
		x = y->right;
		if(y != z->right)
		{
			parent = y->p;
			t = RB_Transplant_station(t, y, y->right);	//Replace y by its right child
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
			t = RB_Transplant_station(t, z, y);
			y->left = z->left;
			y->left->p = y;
			y->color = z->color;
			y->p = z->p; 
		}
	}
	free(z);
	if(y_original_color == 0){  				//If any RB violations occurred
		t = RB_Delete_Fixup_station(t, x, parent);		//Correct them
	}
	return t;
}


car_t * RB_Delete_car(car_t *t, car_t *z){
	int y_original_color;
	car_t *x = NULL, *y = NULL, *parent = NULL;
	
	y = z;
	y_original_color = y->color;
	if(z->left == NULL){
		x = z->right;
		parent = z->p;
		t = RB_Transplant_car(t, z, z->right);	//Replace z by its right child
	}
	else if(z->right == NULL)
	{
		x = z->left;
		parent = z->p;
		t = RB_Transplant_car(t, z, z->left);	//Replace z by its left child
	}
	else
	{
		y = Tree_Minimum_car(z->right);		//y is z's successor
		y_original_color = y->color;
		x = y->right;
		if(y != z->right)
		{
			parent = y->p;
			t = RB_Transplant_car(t, y, y->right);	//Replace y by its right child
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
			t = RB_Transplant_car(t, z, y);
			y->left = z->left;
			y->left->p = y;
			y->color = z->color;
			y->p = z->p; 
		}
	}
	free(z);
	if(y_original_color == 0){  				//If any RB violations occurred
		t = RB_Delete_Fixup_car(t, x, parent);		//Correct them
	}
	return t;
}


station_t * RB_Delete_Fixup_station(station_t *t, station_t *x, station_t *parent){
	station_t *w;
	
	while((x != t && x == NULL) || (x != t && x->color == 0)){		//While x is black or NULL
		if(x == parent->left){		//Is x a left child?
			w = parent->right;		//w is x's sibling
			if(w->color == 1){  	//Case 1
				w->color = 0;  
				parent->color = 1;
				t = Left_Rotate_station(t, parent);
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
						t = Right_Rotate_station(t, w);
						w = parent->right;
					}
					w->color = parent->color;		//Case 4
					parent->color = 0;
					w->right->color = 0;
					t = Left_Rotate_station(t, parent);
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
						t = Right_Rotate_station(t, w);
						w = parent->right;
					}
					w->color = parent->color;		//Case 4
					parent->color = 0;
					w->right->color = 0;
					t = Left_Rotate_station(t, parent);
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
					t = Right_Rotate_station(t, w);
					w = parent->right;
				}
				w->color = parent->color;		//Case 4
				parent->color = 0;
				w->right->color = 0;
				t = Left_Rotate_station(t, parent);
				x = t;
			}
		}
		else{
			w = parent->left;
			if(w->color == 1){
				w->color = 0;
				parent->color = 1;
				t = Right_Rotate_station(t, parent);
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
						t = Left_Rotate_station(t, w);
						w = parent->left;
					}
					w->color = parent->color;
					parent->color = 0;
					w->left->color = 0;
					t = Right_Rotate_station(t, parent);
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
						t = Left_Rotate_station(t, w);
						w = parent->left;
					}
					w->color = parent->color;
					parent->color = 0;
					w->left->color = 0;
					t = Right_Rotate_station(t, parent);
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
					t = Left_Rotate_station(t, w);
					w = parent->left;
				}
				w->color = parent->color;
				parent->color = 0;
				w->left->color = 0;
				t = Right_Rotate_station(t, parent);
				x = t;
			}
		}
	}
	if(x != NULL){
		x->color = 0;
	}
	return t;
}



car_t * RB_Delete_Fixup_car(car_t *t, car_t *x, car_t *parent){
	car_t *w;
	
	while((x != t && x == NULL) || (x != t && x->color == 0)){		//While x is black or NULL
		if(x == parent->left){		//Is x a left child?
			w = parent->right;		//w is x's sibling
			if(w->color == 1){  	//Case 1
				w->color = 0;  
				parent->color = 1;
				t = Left_Rotate_car(t, parent);
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
						t = Right_Rotate_car(t, w);
						w = parent->right;
					}
					w->color = parent->color;		//Case 4
					parent->color = 0;
					w->right->color = 0;
					t = Left_Rotate_car(t, parent);
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
						t = Right_Rotate_car(t, w);
						w = parent->right;
					}
					w->color = parent->color;		//Case 4
					parent->color = 0;
					w->right->color = 0;
					t = Left_Rotate_car(t, parent);
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
					t = Right_Rotate_car(t, w);
					w = parent->right;
				}
				w->color = parent->color;		//Case 4
				parent->color = 0;
				w->right->color = 0;
				t = Left_Rotate_car(t, parent);
				x = t;
			}
		}
		else{
			w = parent->left;
			if(w->color == 1){
				w->color = 0;
				parent->color = 1;
				t = Right_Rotate_car(t, parent);
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
						t = Left_Rotate_car(t, w);
						w = parent->left;
					}
					w->color = parent->color;
					parent->color = 0;
					w->left->color = 0;
					t = Right_Rotate_car(t, parent);
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
						t = Left_Rotate_car(t, w);
						w = parent->left;
					}
					w->color = parent->color;
					parent->color = 0;
					w->left->color = 0;
					t = Right_Rotate_car(t, parent);
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
					t = Left_Rotate_car(t, w);
					w = parent->left;
				}
				w->color = parent->color;
				parent->color = 0;
				w->left->color = 0;
				t = Right_Rotate_car(t, parent);
				x = t;
			}
		}
	}
	if(x != NULL){
		x->color = 0;
	}
	return t;
}


stack_t * push(stack_t *s, stack_t *z){
	if(s == NULL){
		s = z;
	}
	else{
		z->next = s;
		s = z;
	}
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
	}
	else{
		z->previous = t;
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


void path_finder(station_t *start, station_t *finish){
	station_t *tmp_destination = NULL, *tmp_stop = NULL, *farthest_stop = NULL, *tmp_limit = NULL, *tmp = NULL;
	station_t *tmp_start = NULL; 
	car_t *tmp_max_range = NULL;
	stack_t *stops = NULL, *tmp_elem = NULL, *black_list = NULL, *stack_ptr = NULL;
	queue_t *head = NULL, *tail = NULL, *tmp_tail = NULL;
	int res, len;


	if(start->distance == finish->distance){		//Arrival station is the same as the starting one.
		printf("%d\n", start->distance);
	}
	else if(start->distance < finish->distance)		//Arrival station is closer to the start of the highway (first direction)
	{
		tmp_destination = finish;
		tmp_stop = Tree_Predecessor(finish);	//Find the predecessor of the arrival station
		tmp_elem = malloc(sizeof(stack_t));		//Push the arrival station into the stops stack
		if(tmp_elem){
			tmp_elem->station_ptr = finish;
			tmp_elem->next = NULL;
			stops = push(stops, tmp_elem);
			len = 1;
		}else{
			printf("Error in memory allocation!\n");
			return;
		}
		while(farthest_stop != start)
		{
			farthest_stop = NULL;
			while((tmp_stop != NULL) && (tmp_stop->distance >= start->distance))	//Loop for finding the farthest station that can reach tmp_destination
			{
				stack_ptr = black_list;
				res = 0;							
				while(stack_ptr	!= NULL && res == 0){						//Loop for checking if the candidate tmp_stop is on the blacklist
					if(tmp_stop == stack_ptr->station_ptr){
						res = 1;
					}
					stack_ptr = stack_ptr->next;
				}
				if(res == 0){								//If the candidate tmp_stop is not on the blacklist
					tmp_max_range = Tree_Maximum_car(tmp_stop->cars);
					if(tmp_max_range->range >= (tmp_destination->distance - tmp_stop->distance)){
						farthest_stop = tmp_stop;
					}
				}
				tmp_stop = Tree_Predecessor(tmp_stop);
			}
			if(farthest_stop != NULL){					//If we find the farthest station
				tmp_elem = malloc(sizeof(stack_t));		//Push it into the stops stack
				if(tmp_elem){
					tmp_elem->station_ptr = farthest_stop;
					tmp_elem->next = NULL;
					stops = push(stops, tmp_elem);
					len ++;
				}else{
					printf("Error in memory allocation!\n");
					return;
				}
			}else if(tmp_destination == finish)			//If we can't reach the final station from any of the stations before it
			{
				printf("nessun percorso\n");
				return;
			}else 										//If the farthest stop cannot be reached by any ancestor
			{	
				while((farthest_stop == NULL) && (stops->next != NULL))		//Use a stop that's closer to the arrival station and try to search for the farthest station that can reach it
				{
					tmp_limit = stops->station_ptr;
					stops = pop(stops);
					len --;
					tmp_elem = malloc(sizeof(stack_t));						//Push the unreachable station into the blacklist stack
					if(tmp_elem){
						tmp_elem->station_ptr = tmp_limit;
						tmp_elem->next = NULL;
						black_list = push(black_list, tmp_elem);
					}else{
						printf("Error in memory allocation!\n");
						return;
					}
					tmp_destination = stops->station_ptr;
					tmp_stop = Tree_Predecessor(tmp_destination);
					while((tmp_stop != NULL) && (tmp_stop->distance > tmp_limit->distance))		//Loop for finding the farthest station that can reach tmp_destination
					{
						tmp_max_range = Tree_Maximum_car(tmp_stop->cars);
						if(tmp_max_range->range >= (tmp_destination->distance - tmp_stop->distance)){
							farthest_stop = tmp_stop;
						}
						tmp_stop = Tree_Predecessor(tmp_stop);
					}
				}
				if(farthest_stop == NULL){
					printf("nessun percorso\n");
					return;
				}
				else  							//If we find the farthest station
				{  								//Push it into the stops stack
					tmp_elem = malloc(sizeof(stack_t));		
					if(tmp_elem){
						tmp_elem->station_ptr = farthest_stop;
						tmp_elem->next = NULL;
						stops = push(stops, tmp_elem);
						len ++;
					}else{
						printf("Error in memory allocation!\n");
						return;
					}
				}
			}
			tmp_destination = farthest_stop;
			tmp_stop = Tree_Predecessor(tmp_destination);
		}
		stack_ptr = stops->next;
		tmp = stack_ptr->station_ptr;
		tmp_elem = NULL;
		while(tmp != finish){		//Loop for checking if the chosen path is indeed the one with the least stops
			tmp_elem = shorter_path_stack(start, finish, tmp, len);
			if(tmp_elem != NULL){
				stops = tmp_elem;
				stack_ptr = stops->next;
				tmp = stack_ptr->station_ptr;
				len = 0;
				tmp_elem = stops;
				while(tmp_elem != NULL){
					len ++;
					tmp_elem = tmp_elem->next;
				}
			}
			else{
				stack_ptr = stack_ptr->next;
				tmp = stack_ptr->station_ptr;
			}
		}	
		while(stops->next != NULL){
			tmp = stops->station_ptr;
			printf("%d ", tmp->distance);
			stops = pop(stops);
		}
		tmp = stops->station_ptr;
		printf("%d\n", tmp->distance);
		stops = pop(stops);
	}else  								//Final destination ;) is closer to the start of the highway (second direction)
		{
		tmp_start = start;
		tmp_stop = Tree_Predecessor(start);		//Find the predecessor of the starting station
		tmp_tail = malloc(sizeof(queue_t));		//Enqueue the station into the queue 
		if(tmp_tail){
			tmp_tail->station_ptr = start;
			tmp_tail->next = NULL;
			tmp_tail->previous = NULL;
			tail = enqueue(tail, tmp_tail);
			head = tail;
			len = 1;
		}else{
			printf("Error in memory allocation!\n");
			return;
		}
		while(farthest_stop != finish)
		{
			farthest_stop = NULL;
			while((tmp_stop != NULL) && (tmp_stop->distance >= finish->distance))	//Loop for finding the farthest station that can be reached by tmp_start
			{		
				stack_ptr = black_list;
				res = 0;
				while(stack_ptr != NULL && res == 0){				//Loop for checking if the candidate tmp_stop is on the blacklist
					if(tmp_stop == stack_ptr->station_ptr){
						res = 1;
					}
					stack_ptr = stack_ptr->next;
				}
				if(res == 0){									//If the candidate tmp_stop is not on the blacklist
					tmp_max_range = Tree_Maximum_car(tmp_start->cars);
					if(tmp_max_range->range >= (tmp_start->distance - tmp_stop->distance)){
						farthest_stop = tmp_stop;
					}
				}
				tmp_stop = Tree_Predecessor(tmp_stop);
			}
			if(farthest_stop != NULL){					//If we find the farthest station
				tmp_tail = malloc(sizeof(queue_t));		//Enqueue it into the stations queue
				if(tmp_tail){
					tmp_tail->station_ptr = farthest_stop;
					tmp_tail->next = NULL;
					tmp_tail->previous = NULL;
					tail = enqueue(tail, tmp_tail);
					len ++;
				}else{
					printf("Error in memory allocation!\n");
					return;
				}
			}else if(tmp_start == start)		//If we can't reach any of the stations after the starting one
			{
				printf("nessun percorso\n");
				return;
			}else 								//If the farthest stop cannot reach any of its successors
			{
				while((farthest_stop == NULL) && (tail->previous != NULL))		//Use a stop that's closer to the starting station and try to search for the farthest station that it can reach
				{
					tmp_limit = tail->station_ptr;
					tmp_tail = tail;
					tail = tail->previous;
					free(tmp_tail);
					len --;
					tmp_elem = malloc(sizeof(stack_t));
					if(tmp_elem){												//Push the unreachable station into the blacklist stack
						tmp_elem->station_ptr = tmp_limit;
						tmp_elem->next = NULL;
						black_list = push(black_list, tmp_elem);
					}else{
						printf("Error in memory allocation!\n");
						return;
					}
					tmp_start = tail->station_ptr;
					tmp_stop = Tree_Predecessor(tmp_start);
					while((tmp_stop != NULL) && (tmp_stop->distance > tmp_limit->distance))		//Loop for finding the farthest station that can be reached by tmp_start
					{	
						tmp_max_range = Tree_Maximum_car(tmp_start->cars);
						if(tmp_max_range->range >= (tmp_start->distance - tmp_stop->distance)){
							farthest_stop = tmp_stop;
						}
						tmp_stop = Tree_Predecessor(tmp_stop);
					}
				}
				if(farthest_stop == NULL){
					printf("nessun percorso\n");
					return;
				}
				else  									//If we find the farthest station
				{										//Enqueue it into the stations queue
					tmp_tail = malloc(sizeof(queue_t));
					if(tmp_tail){
						tmp_tail->station_ptr = farthest_stop;
						tmp_tail->next = NULL;
						tmp_tail->previous = NULL;
						tail = enqueue(tail, tmp_tail);
						len ++;
					}
					else{
						printf("Error in memory allocation!\n");
						return;
					}
				}
			}
			tmp_start = farthest_stop;
			tmp_stop = Tree_Predecessor(tmp_start);
		}
		while(head->next != NULL){
			tmp = head->station_ptr;
			printf("%d ", tmp->distance);
			head = dequeue(head);
		}
		tmp = head->station_ptr;
		printf("%d\n", tmp->distance);
		head =  dequeue(head);
	}
	return;
}	


stack_t * shorter_path_stack(station_t *start, station_t *finish, station_t *candidate, int max_len){
	station_t *tmp_destination = NULL, *tmp_stop = NULL, *farthest_stop = NULL, *tmp_limit = NULL;
	stack_t *stops = NULL, *tmp_elem = NULL, *black_list = NULL, *stack_ptr = NULL;
	int res, len;

	tmp_elem = malloc(sizeof(stack_t));		//Push the candidate into the stations blacklist
	if(tmp_elem){
		tmp_elem->station_ptr = candidate;
		tmp_elem->next = NULL;
		black_list = push(black_list, tmp_elem);
	}
	tmp_destination = finish;
	tmp_stop = Tree_Predecessor(finish);	//Find the predecessor of the arrival station
	tmp_elem = malloc(sizeof(stack_t));		//Push the arrival station into the stops stack
	if(tmp_elem){
		tmp_elem->station_ptr = finish;
		tmp_elem->next = NULL;
		stops = push(stops, tmp_elem);
		len = 1;
	}else{
		printf("Error in memory allocation!\n");
		return;
	}
	while(farthest_stop != start && len < max_len)
	{
		farthest_stop = NULL;
		while((tmp_stop != NULL) && (tmp_stop->distance >= start->distance))	//Loop for finding the farthest station that can reach tmp_destination
		{
			stack_ptr = black_list;
			res = 0;							
			while(stack_ptr	!= NULL && res == 0){						//Loop for checking if the candidate tmp_stop is on the blacklist
				if(tmp_stop == stack_ptr->station_ptr){
					res = 1;
				}
				stack_ptr = stack_ptr->next;
			}
			if(res == 0){								//If the candidate tmp_stop is not on the blacklist
				tmp_max_range = Tree_Maximum_car(tmp_stop->cars);
				if(tmp_max_range->range >= (tmp_destination->distance - tmp_stop->distance)){
					farthest_stop = tmp_stop;
				}
			}
			tmp_stop = Tree_Predecessor(tmp_stop);
		}
		if(farthest_stop != NULL){					//If we find the farthest station
			tmp_elem = malloc(sizeof(stack_t));		//Push it into the stops stack
			if(tmp_elem){
				tmp_elem->station_ptr = farthest_stop;
				tmp_elem->next = NULL;
				stops = push(stops, tmp_elem);
				len ++;
			}else{
				printf("Error in memory allocation!\n");
				return;
			}
		}else if(tmp_destination == finish)			//If we can't reach the final station from any of the stations before it
		{
			return NULL;
		}else 										//If the farthest stop cannot be reached by any ancestor
		{	
			while((farthest_stop == NULL) && (stops->next != NULL))		//Use a stop that's closer to the arrival station and try to search for the farthest station that can reach it
			{
				tmp_limit = stops->station_ptr;
				stops = pop(stops);
				len --;
				tmp_elem = malloc(sizeof(stack_t));						//Push the unreachable station into the blacklist stack
				if(tmp_elem){
					tmp_elem->station_ptr = tmp_limit;
					tmp_elem->next = NULL;
					black_list = push(black_list, tmp_elem);
				}else{
					printf("Error in memory allocation!\n");
					return;
				}
				tmp_destination = stops->station_ptr;
				tmp_stop = Tree_Predecessor(tmp_destination);
				while((tmp_stop != NULL) && (tmp_stop->distance > tmp_limit->distance))		//Loop for finding the farthest station that can reach tmp_destination
				{
					tmp_max_range = Tree_Maximum_car(tmp_stop->cars);
					if(tmp_max_range->range >= (tmp_destination->distance - tmp_stop->distance)){
						farthest_stop = tmp_stop;
					}
					tmp_stop = Tree_Predecessor(tmp_stop);
				}
			}
			if(farthest_stop == NULL){
				return NULL;
			}
			else  							//If we find the farthest station
			{  								//Push it into the stops stack
				tmp_elem = malloc(sizeof(stack_t));		
				if(tmp_elem){
					tmp_elem->station_ptr = farthest_stop;
					tmp_elem->next = NULL;
					stops = push(stops, tmp_elem);
					len ++;
				}else{
					printf("Error in memory allocation!\n");
					return;
				}
			}
		}
		tmp_destination = farthest_stop;
		tmp_stop = Tree_Predecessor(tmp_destination);
	}
	if(len >= max_len){
		return NULL;
	}
	else{
		return stops;
	}
}


stack_t * shorter_path_queue(station_t *start, station_t *finish, station_t *candidate, int max_len){
	
}
