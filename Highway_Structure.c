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
	int val;
	struct stack_s *next;
}stack_t;



void InOrder_Tree_Walk(station_t *);

station_t * Tree_Search(station_t *, int);

car_t * Tree_Search_car(car_t *, int);

station_t * Tree_Minimum(station_t *);

car_t * Tree_Minimum_car(car_t *);

station_t * Tree_Maximum(station_t *);

car_t * Tree_Maximum_car(car_t *);

station_t * Tree_Successor(station_t *);

void Left_Rotate_station(station_t *, station_t *);

void Left_Rotate_car(car_t *, car_t *);

void Right_Rotate_station(station_t *, station_t *);

void Right_Rotate_car(car_t *, car_t *);

void RB_Insert_car(car_t*, car_t*);

int RB_Insert_station(station_t *, station_t *);

void RB_Insert_Fixup_station(station_t *, station_t *);

void RB_Insert_Fixup_car(car_t *, car_t *);

void RB_Transplant_station(station_t *, station_t *, station_t *);

void RB_Transplant_car(car_t *, car_t *, car_t *);

void RB_Delete_station(station_t *, station_t *);

void RB_Delete_car(car_t *, car_t *);

void RB_Delete_Fixup_station(station_t *, station_t *);

void RB_Delete_Fixup_car(car_t *, car_t *);

void push(stack_t *, stack_t *);

int pop(stack_t *);

int empty(stack_t *);


int main(int argc, char const *argv[])
{
	FILE* fd;
	char command[20];
	int distance, number_of_cars, range, i, starting_station, arrival_station;
	station_t *station_root = NULL, *tmp_station = NULL;
	car_t *car_root = NULL, *tmp_car = NULL;


	fd = fopen("archivio_test_aperti\\open_1.txt", "r");
	if(fd)
	{
		while(fscanf(fd, "%s", command) != EOF)
		{

		if(strcmp(command, "aggiungi-stazione") == 0)
		{
			car_root = NULL;
			if(fscanf(fd, "%d %d", &distance, &number_of_cars) != EOF)
			{
				/*Allocate the size of 1 station node*/
			 	tmp_station = malloc(sizeof(station_t));
				if(tmp_station)
				{
					tmp_station->distance = distance;
					/*Insert the station node into the station tree*/
					if(RB_Insert_station(station_root, tmp_station) == -1)
					{
						fprintf(fd, "non aggiunta\n");
						free(tmp_station);
					}
					else
					{
						/*Loop for creating the car tree*/
						for(i = 0; i < number_of_cars; i++)
						{
							if(fscanf(fd, "%d", &range) != EOF)
							{
								/*Allocate the size of 1 car node */
								tmp_car = malloc(sizeof(car_t));
								if(tmp_car)
								{
									tmp_car->range = range;
									/*Insert the car node into the car tree*/
									RB_Insert_car(car_root, tmp_car);
								}else{
								printf("Error in memory allocation!");
								}
							}	 
						}
						tmp_station->cars = car_root;
						fprintf(fd, "aggiunta\n");
					}
				}else
				{
					printf("Error in memory allocation!");
				}
			}
		}

		else if(strcmp(command, "demolisci-stazione") == 0)
		{
			if(fscanf(fd, "%d", &distance) != EOF)
			{
				/*First we search for the station*/
				tmp_station = Tree_Search(station_root, distance);
				if(tmp_station != NULL)
				{
					/*Delete station if found*/
					RB_Delete_station(station_root, tmp_station);
					free(tmp_station);
					fprintf(fd, "demolita\n");
				}else{
					fprintf(fd, "non demolita\n");
				}
			}
		}

		else if (strcmp(command, "aggiungi-auto") == 0)
		{
			if(fscanf(fd, "%d %d", &distance, &range) != EOF)
			{
				/*First we search for the station*/
				tmp_station = Tree_Search(station_root, distance);
				if (tmp_station != NULL)
				{
					/*Allocate the size of 1 car node*/
					tmp_car = malloc(sizeof(car_t));
					if (tmp_car)
					{
						tmp_car->range = range;
						/*Insert the car node into the car tree of the designated station*/
						RB_Insert_car(tmp_station->cars, tmp_car);
						fprintf(fd, "aggiunta\n");
					}else{
						printf("Error in memory allocation!");
					}
				}else{
					fprintf(fd, "non aggiunta\n");
				}
			}
		}

		else if (strcmp(command, "rottama-auto") == 0)
		{
			if (fscanf(fd, "%d %d", &distance, &range))
			{
				/*First we search for the station*/
				tmp_station = Tree_Search(station_root, distance);
				if (tmp_station != NULL)
				{
					/*If the station is found we then search for the car within the station's cars*/
					tmp_car = Tree_Search_car(tmp_station->cars, range);
					if (tmp_car != NULL)
					{
						/*If the car is found we proceed to delete it*/
						RB_Delete_car(tmp_station->cars, tmp_car);
						free(tmp_car);
						fprintf(fd, "rottamata\n");
					}else{
						fprintf(fd, "non rottamata\n");
					}
				}else{
					fprintf(fd, "non rottamata\n");
				}
			}
		}

		else if (strcmp(command, "pianifica-percorso") == 0)
		{
			if (fscanf(fd, "%d %d", &starting_station, &arrival_station) != EOF)
			{
				
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


station_t * Tree_Search(station_t *x, int k){
	if(x==NULL || k == x->distance){
		return x;
	}
	else if(k < x->distance){
		return Tree_Search(x->left, k);
	}
	else{
		return Tree_Search(x->right, k);
	}
}


car_t * Tree_Search_car(car_t *x, int k){
	if(x==NULL || k == x->range){
		return x;
	}
	else if(k < x->range){
		return Tree_Search_car(x->left, k);
	}
	else{
		return Tree_Search_car(x->right, k);
	}
}


station_t * Tree_Minimum(station_t *x){
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

station_t * Tree_Maximum(station_t *x){
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
	station_t *y;
	if(x->right != NULL){
		return Tree_Minimum(x->right);
	}
	y = x->p;
	while(y != NULL && x == y->right){
		x = y;
		y = y->p;
	}
	return y;
}


void Left_Rotate_station(station_t *t, station_t *x){
	station_t *y;
	y = x->right;
	x->right = y->left;
	if(y->left != NULL){
		y->left->p = x;
	}
	y->p =  x->p;
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
}


void Left_Rotate_car(car_t *t, car_t *x){
	car_t *y;
	y = x->right;
	x->right = y->left;
	if(y->left != NULL){
		y->left->p = x;
	}
	y->p =  x->p;
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
}


void Right_Rotate_station(station_t *t, station_t *y){
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
}

void Right_Rotate_car(car_t *t, car_t *y){
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
}


void RB_Insert_car(car_t *t, car_t *z){
	car_t *x, *y;
	y = NULL;
	x = t;
	while(x != NULL){
		y = x;
		if(z->range < x->range){
			x = x->left;
		}
		else{
			x = x->right;
		}
	}
	z->p = y;
	if(y == NULL){
		t = z;
	}
	else if (z->range < y->range)
	{
		y->left = z;
	}
	else{
		y->right = z;
	}
	z->left = NULL;
	z->right = NULL;
	z->color = 1;  /*red*/
	RB_Insert_Fixup_car(t, z);
}

int RB_Insert_station(station_t *t, station_t *z){
	station_t *x, *y;
	y = NULL;
	x = t;
	while(x != NULL){
		y = x;
		if(z->distance < x->distance){
			x = x->left;
		}
		else if(z->distance > x->distance){
			x = x->right;
		}
		else{
			return -1;
		}
	}
	z->p = y;
	if(y == NULL){
		t = z;
	}
	else if (z->distance < y->distance)
	{
		y->left = z;
	}
	else{
		y->right = z;
	}
	z->left = NULL;
	z->right = NULL;
	z->color = 1;  /*red*/
	RB_Insert_Fixup_station(t, z);
	return 0;
}


void RB_Insert_Fixup_station(station_t *t, station_t *z){
	station_t *y;
	while(z->p->color == 1){  /*red*/
		if(z->p == z->p->p->left){
			y = z->p->p->right;
			if(y->color == 1){  /*red*/
				z->p->color = 0;  /*black*/
				y->color = 0;  /*black*/
				z->p->p->color = 1;  /*red*/
				z = z->p->p;
			}else if (z == z->p->right)
			{
				z = z->p;
				Left_Rotate_station(t, z);
			}
			z->p->color = 0;  /*black*/
			z->p->p->color = 1;  /*red*/
			Right_Rotate_station(t, z->p->p);
		}
		else{
			y = z->p->p->left;
			if(y->color == 1){  /*red*/
				z->p->color = 0;  /*black*/
				y->color = 0;  /*black*/
				z->p->p->color = 1;  /*red*/
				z = z->p->p;
			}
			else if (z == z->p->left)
			{
				z = z->p;
				Right_Rotate_station(t, z);
			}
			z->p->color = 0;  /*black*/
			z->p->p->color = 1;  /*red*/
			Left_Rotate_station(t, z->p->p);
			}
		}
	t->color = 0;  /*black*/	
}


void RB_Insert_Fixup_car(car_t *t, car_t *z){
	car_t *y;
	while(z->p->color == 1){  /*red*/
		if(z->p == z->p->p->left){
			y = z->p->p->right;
			if(y->color == 1){  /*red*/
				z->p->color = 0;  /*black*/
				y->color = 0;  /*black*/
				z->p->p->color = 1;  /*red*/
				z = z->p->p;
			}else if (z == z->p->right)
			{
				z = z->p;
				Left_Rotate_car(t, z);
			}
			z->p->color = 0;  /*black*/
			z->p->p->color = 1;  /*red*/
			Right_Rotate_car(t, z->p->p);
		}
		else{
			y = z->p->p->left;
			if(y->color == 1){  /*red*/
				z->p->color = 0;  /*black*/
				y->color = 0;  /*black*/
				z->p->p->color = 1;  /*red*/
				z = z->p->p;
			}
			else if (z == z->p->left)
			{
				z = z->p;
				Right_Rotate_car(t, z);
			}
			z->p->color = 0;  /*black*/
			z->p->p->color = 1;  /*red*/
			Left_Rotate_car(t, z->p->p);
			}
		}
	t->color = 0;  /*black*/	
}
	

void RB_Transplant_station(station_t *t, station_t *u, station_t *v){
	if (u->p == NULL){
		t = v;
	}
	else if (u == u->p->left)
	{
		u->p->left = v;
	}
	else{
		u->p->right = v;
	}
	v->p = u->p;
}


void RB_Transplant_car(car_t *t, car_t *u, car_t *v){
	if (u->p == NULL){
		t = v;
	}
	else if (u == u->p->left)
	{
		u->p->left = v;
	}
	else{
		u->p->right = v;
	}
	v->p = u->p;
}


void RB_Delete_station(station_t *t, station_t *z){
	int y_original_color;
	station_t *x, *y;
	y = z;
	y_original_color = y->color;
	if(z->left == NULL){
		x = z->right;
		RB_Transplant_station(t, z, z->right);
	}
	else if (z->right == NULL)
	{
		x = z->left;
		RB_Transplant_station(t, z, z->left);
	}
	else{
		y = Tree_Minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if(y->p == z){
			x->p = y;
		}
		else{
			RB_Transplant_station(t, y, t->right);
			y->right = z->right;
			y->right->p = y;
		}
		RB_Transplant_station(t, z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if(y_original_color == 0){  /*black*/
		RB_Delete_Fixup_station(t, x);
	}
}


void RB_Delete_car(car_t *t, car_t *z){
	int y_original_color;
	car_t *x, *y;
	y = z;
	y_original_color = y->color;
	if(z->left == NULL){
		x = z->right;
		RB_Transplant_car(t, z, z->right);
	}
	else if (z->right == NULL)
	{
		x = z->left;
		RB_Transplant_car(t, z, z->left);
	}
	else{
		y = Tree_Minimum_car(z->right);
		y_original_color = y->color;
		x = y->right;
		if(y->p == z){
			x->p = y;
		}
		else{
			RB_Transplant_car(t, y, t->right);
			y->right = z->right;
			y->right->p = y;
		}
		RB_Transplant_car(t, z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if(y_original_color == 0){  /*black*/
		RB_Delete_Fixup_car(t, x);
	}
}



void RB_Delete_Fixup_station(station_t *t, station_t *x){
	station_t *w;
	while(x != t && x->color == 0){  /*black*/
		if(x == x->p->left){
			w = x->p->right;
			if(w->color == 1){  /*red*/
				w->color = 0;  /*black*/
				x->p->color = 1;  /*red*/
				Left_Rotate_station(t, x->p);
				w = x->p->right;
			}
			if(w->left->color == 0 && w->right->color == 0){
				w->color = 1;
				x = x->p;
			}
			else{
				if(w->right->color == 0){
					w->left->color = 0;
					w->color = 1;
					Right_Rotate_station(t, w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = 0;
				w->right->color = 0;
				Left_Rotate_station(t, x->p);
				x = t;
			}
		}
		else{
			w = x->p->left;
			if(w->color == 1){
				w->color = 0;
				x->p->color = 1;
				Right_Rotate_station(t, x->p);
				w = x->p->left;
			}
			if(w->right->color == 0 && w->left->color == 0){
				w->color = 0;
				x = x->p;
			}
			else{
				if(w->left->color == 0){
					w->right->color = 0;
					w->color = 1;
					Left_Rotate_station(t, w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = 0;
				w->left->color = 0;
				Right_Rotate_station(t, x->p);
				x = t;
			}
		}
	}
	x->color = 0;
}


void RB_Delete_Fixup_car(car_t *t, car_t *x){
	car_t *w;
	while(x != t && x->color == 0){  /*black*/
		if(x == x->p->left){
			w = x->p->right;
			if(w->color == 1){  /*red*/
				w->color = 0;  /*black*/
				x->p->color = 1;  /*red*/
				Left_Rotate_car(t, x->p);
				w = x->p->right;
			}
			if(w->left->color == 0 && w->right->color == 0){
				w->color = 1;
				x = x->p;
			}
			else{
				if(w->right->color == 0){
					w->left->color = 0;
					w->color = 1;
					Right_Rotate_car(t, w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = 0;
				w->right->color = 0;
				Left_Rotate_car(t, x->p);
				x = t;
			}
		}
		else{
			w = x->p->left;
			if(w->color == 1){
				w->color = 0;
				x->p->color = 1;
				Right_Rotate_car(t, x->p);
				w = x->p->left;
			}
			if(w->right->color == 0 && w->left->color == 0){
				w->color = 0;
				x = x->p;
			}
			else{
				if(w->left->color == 0){
					w->right->color = 0;
					w->color = 1;
					Left_Rotate_car(t, w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = 0;
				w->left->color = 0;
				Right_Rotate_car(t, x->p);
				x = t;
			}
		}
	}
	x->color = 0;
}



void push(stack_t *s, stack_t *z){
	if(s == NULL){
		s = z;
	}
	else{
		z->next = s;
		s = z;
	}
}


int pop(stack_t *s){
	int tmp;
	stack_t *head;
	
	tmp = s->val;
	head = s;
	s = s->next;
	free(head);
	return tmp;
}


int empty(stack_t *s){
	int res;

	if(s == NULL){
		res = 1;
	}else{
		res = 0;
	}

	return res;
}