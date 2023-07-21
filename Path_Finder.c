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
	int visited;
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
	struct stack_s *parent;
	int level;
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

void path_finder(station_t *, station_t *, int);



int main(int argc, char const *argv[])
{
	FILE* fd;
	char command[20];
	int distance, number_of_cars, range, index = 0, i, starting_station, arrival_station;
	station_t *station_root = NULL, *tmp_station = NULL, *start = NULL, *finish = NULL;
	car_t *car_root = NULL, *tmp_car = NULL;


	fd = fopen("archivio_test_aperti/open_100.txt", "r");
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
						tmp_station->visited = 0;
						index ++;
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
						path_finder(start, finish, index);
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


void path_finder(station_t *start, station_t *finish, int len){
	station_t *tmp_stop = NULL, *farthest_stop = NULL;
	car_t *tmp_max_range = NULL;
	queue_t *head_1 = NULL, *tail_1 = NULL, *tail_2 = NULL, *tmp_tail = NULL, *tmp_head = NULL, *queue_ptr = NULL;
	int lvl, limit;
	stack_t *stack_ptr = NULL, *stops = NULL;

	if(start->distance == finish->distance){		//Arrival station is the same as the starting one.
		printf("%d\n", start->distance);
		return;
	}
	else if(start->distance < finish->distance)		//Starting station is closer to the start of the highway (first direction)
	{
		lvl = 0;
		tmp_tail = malloc(sizeof(queue_t));		//Enqueue the starting station into the first queue (nodes to be visited queue)
		if(tmp_tail){
			tmp_tail->station_ptr = start;
			tmp_tail->next = NULL;
			tmp_tail->previous = NULL;
			tmp_tail->parent = NULL;
			tmp_tail->level = lvl;
			tail_1 = enqueue(tail_1, tmp_tail);
			head_1 = tail_1;
		}else{
			printf("Error in memory allocation!\n");
		}
		while(head_1 != NULL && head_1->station_ptr != finish){		//While there are still nodes to visit
			tmp_stop = head_1->station_ptr;
			if(tmp_stop->visited == 0){			//If the node hasn't been visited yet then enqueue it into the second queue(visited nodes)
				tmp_head = head_1;
				head_1 = head_1->next;
				tail_2 = enqueue(tail_2, tmp_head);
				tail_2->station_ptr->visited = 1;
				lvl = tail_2->level + 1;
				farthest_stop = Tree_Successor(tmp_stop);		//Enqueue all the reachable stations from the tmp_stop into the first queue(nodes to be visited)
				while(farthest_stop != NULL){
					tmp_max_range = Tree_Maximum_car(tmp_stop->cars);
					if(tmp_max_range->range >= (farthest_stop->distance - tmp_stop->distance)){
						tmp_tail = malloc(sizeof(queue_t));
						if(tmp_tail){
							tmp_tail->station_ptr = farthest_stop;
							tmp_tail->next = NULL;
							tmp_tail->previous = NULL;
							tmp_tail->level = lvl;
							stack_ptr = malloc(sizeof(stack_t));
							if(stack_ptr){
								stack_ptr->station_ptr = tmp_stop;
								stack_ptr->next = NULL;
							}else{
								printf("Error in memory allocation!\n");
							}
							tmp_tail->parent = stack_ptr;
							tail_1 = enqueue(tail_1, tmp_tail);
							if(head_1 == NULL){
								head_1 = tail_1;
							}
						}else{
							printf("Error in memory allocation!\n");
						}
						farthest_stop = Tree_Successor(farthest_stop);
					}else{
						farthest_stop = NULL;
					}
				}
			}else{		//If it has already been visited then simply dequeue it from the first queue 
				head_1 = dequeue(head_1);
			}
		}
		if(head_1 != NULL){		//If we find the destination node
			tmp_head = head_1;
			head_1 = head_1->next;
			tail_2 = enqueue(tail_2, tmp_head);		//We enqueue it into the visited nodes queue
			queue_ptr = tail_2;
			while(queue_ptr != NULL){		//While there are stops, push them into the stops stack
				stack_ptr = malloc(sizeof(stack_t));
				if(stack_ptr){
					stack_ptr->station_ptr = queue_ptr->station_ptr;
					stack_ptr->next = NULL;
					stops = push(stops, stack_ptr);
				}else{
					printf("Error in memory allocation!\n");
				}
				if(queue_ptr->parent != NULL){
					tmp_stop = queue_ptr->parent->station_ptr;
					queue_ptr->station_ptr->visited = 0;
					while(queue_ptr->station_ptr != tmp_stop){
						queue_ptr = queue_ptr->previous;
						queue_ptr->station_ptr->visited = 0;
					}
				}else{
					queue_ptr->station_ptr->visited = 0;
					queue_ptr = NULL;
				}
			}
		}
		if(stops != NULL){	
			stack_ptr = stops;
			while(stack_ptr->next != NULL){
				tmp_stop = stack_ptr->station_ptr;
				printf("%d ", tmp_stop->distance);
				stack_ptr = stack_ptr->next;
			}
			tmp_stop = stack_ptr->station_ptr;
			printf("%d\n", tmp_stop->distance);
			return;
		}else{
			queue_ptr = tail_2;
			while(queue_ptr != NULL){
				queue_ptr->station_ptr->visited = 0;
				queue_ptr = queue_ptr->previous;
			}
			printf("nessun percorso\n");
			return;
		}
	}else  				//Arrival station is closer to the start of the highway (second direction)
	{
		lvl = 0;
		tmp_tail = malloc(sizeof(queue_t));		//Enqueue the starting station into the first queue (nodes to be visited queue)
		if(tmp_tail){
			tmp_tail->station_ptr = start;
			tmp_tail->next = NULL;
			tmp_tail->previous = NULL;
			tmp_tail->parent = NULL;
			tmp_tail->level = lvl;
			tail_1 = enqueue(tail_1, tmp_tail);
			head_1 = tail_1;
		}else{
			printf("Error in memory allocation!\n");
		}
		while(head_1 != NULL && head_1->station_ptr != finish){		//While there are still nodes to visit
			tmp_stop = head_1->station_ptr;
			if(tmp_stop->visited == 0){			//If the node hasn't been visited yet then enqueue it into the second queue(visited nodes)
				tmp_head = head_1;
				head_1 = head_1->next;
				tail_2 = enqueue(tail_2, tmp_head);
				tail_2->station_ptr->visited = 1;
				lvl = tail_2->level + 1;
				farthest_stop = Tree_Predecessor(tmp_stop);		//Enqueue all the reachable stations from the tmp_stop into the first queue(nodes to be visited)
				while(farthest_stop != NULL){
					tmp_max_range = Tree_Maximum_car(tmp_stop->cars);
					if(tmp_max_range->range >= (tmp_stop->distance - farthest_stop->distance)){
						tmp_tail = malloc(sizeof(queue_t));
						if(tmp_tail){
							tmp_tail->station_ptr = farthest_stop;
							tmp_tail->next = NULL;
							tmp_tail->previous = NULL;
							tmp_tail->level = lvl;
							stack_ptr = malloc(sizeof(stack_t));
							if(stack_ptr){
								stack_ptr->station_ptr = tmp_stop;
								stack_ptr->next = NULL;
							}else{
								printf("Error in memory allocation!\n");
							}
							tmp_tail->parent = stack_ptr;
							tail_1 = enqueue(tail_1, tmp_tail);
							if(head_1 == NULL){
								head_1 = tail_1;
							}
						}else{
							printf("Error in memory allocation!\n");
						}
						farthest_stop = Tree_Predecessor(farthest_stop);
					}else{
						farthest_stop = NULL;
					}
				}
			}else{		//If it has already been visited then if it also has the same lvl of the original and a parent closer to the start then update the parent
				queue_ptr = tail_2;
				while(queue_ptr->station_ptr != tmp_stop){
					queue_ptr = queue_ptr->previous;
				}
				if((head_1->level == queue_ptr->level) && (head_1->parent->station_ptr->distance < queue_ptr->parent->station_ptr->distance)){
					queue_ptr->parent = head_1->parent;
				}
				head_1 = dequeue(head_1);
			}
		}
		if(head_1 != NULL){		//If we find the destination node
			tmp_head = head_1;
			head_1 = head_1->next;
			tail_2 = enqueue(tail_2, tmp_head);		//We enqueue it into the visited nodes queue
			limit = tail_2->level;
			queue_ptr = head_1;
			while(queue_ptr != NULL && queue_ptr->level == limit){		//If there are stations that can reach the final one but are closer to the start then update the final station's parent
				if(queue_ptr->station_ptr == finish){
					if(queue_ptr->parent->station_ptr->distance < tail_2->parent->station_ptr->distance){
						tail_2->parent = queue_ptr->parent;
					}
				}
				queue_ptr = queue_ptr->next;
			}
			queue_ptr = tail_2;
			while(queue_ptr != NULL){		//While there are stops, push them into the stops stack
				stack_ptr = malloc(sizeof(stack_t));
				if(stack_ptr){
					stack_ptr->station_ptr = queue_ptr->station_ptr;
					stack_ptr->next = NULL;
					stops = push(stops, stack_ptr);
				}else{
					printf("Error in memory allocation!\n");
				}
				if(queue_ptr->parent != NULL){
					tmp_stop = queue_ptr->parent->station_ptr;
					queue_ptr->station_ptr->visited = 0;
					while(queue_ptr->station_ptr != tmp_stop){
						queue_ptr = queue_ptr->previous;
						queue_ptr->station_ptr->visited = 0;
					}
				}else{
					queue_ptr->station_ptr->visited = 0;
					queue_ptr = NULL;
				}
			}		
		}
		if(stops != NULL){
			stack_ptr = stops;
			while(stack_ptr->next != NULL){
				tmp_stop = stack_ptr->station_ptr;
				printf("%d ", tmp_stop->distance);
				stack_ptr = stack_ptr->next;
			}
			tmp_stop = stack_ptr->station_ptr;
			printf("%d\n", tmp_stop->distance);
			return;
		}else{
			queue_ptr = tail_2;
			while(queue_ptr != NULL){
				queue_ptr->station_ptr->visited = 0;
				queue_ptr = queue_ptr->previous;
			}
			printf("nessun percorso\n");
			return;
		}
	}
}