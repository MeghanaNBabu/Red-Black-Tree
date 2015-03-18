#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct RB
{
	int key;
	struct RB *right, *left, *p;
	char color[5];
};

struct sentinel
{
	struct RB *root, *nil;
};

struct sentinel *T;
int num,flag = 0;
static int gName = 0;

void left_rotate(struct RB *x)
{
	struct RB *y;
	y = x->right;
	x->right = y->left;
	if(y->left!=T->nil)
		y->left->p = x;
	y->p = x->p;
	if(x->p==T->nil)
		T->root = y;
	else if(x==x->p->left)
			x->p->left = y;
	else x->p->right = y;
	y->left = x;
	x->p = y;
}
void right_rotate(struct RB *x)
{
	struct RB *y;
	y = x->left;
	x->left = y->right;
	if(y->right!=T->nil)
		y->right->p = x;
	y->p = x->p;
	if(x->p==T->nil)
		T->root = y;
	else if(x==x->p->right)
			x->p->right = y;
	else x->p->left = y;
	y->right = x;
	x->p = y;
}



void insert_fixup(struct RB *z)
{
	struct RB *y;
	while(!strcmp(z->p->color,"RED"))
	{
		if(z->p==z->p->p->left)
		{
			y = z->p->p->right;
			if(!strcmp(y->color,"RED"))
			{
				strcpy(z->p->color,"BLACK");
				strcpy(y->color,"BLACK");
				strcpy(z->p->p->color,"RED");
				z = z->p->p;
			}
			else {
					if(z==z->p->right)
					{
						z = z->p;
						left_rotate(z);
					}
					strcpy(z->p->color,"BLACK");
					strcpy(z->p->p->color,"RED");
					right_rotate(z->p->p);
				}
		}
		else 
		{
			y = z->p->p->left;
			if(!strcmp(y->color,"RED"))
			{
				strcpy(z->p->color,"BLACK");
				strcpy(y->color,"BLACK");
				strcpy(z->p->p->color,"RED");
				z = z->p->p;
			}
			else {
					if(z==z->p->left)
					{
						z = z->p;
						right_rotate(z);
					}
					strcpy(z->p->color,"BLACK");
					strcpy(z->p->p->color,"RED");
					left_rotate(z->p->p);
			}
		}
	}	
	strcpy(T->root->color,"BLACK");
}
				
void insert(struct RB *z)
{
	struct RB *y, *x;
	y = T->nil;
	x = T->root;
	while(x!=T->nil)
	{
		y = x;
		if(z->key==y->key)
		{
			free(z);
			flag = 1;
			return;
		}
		if(z->key<x->key)
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if(y==T->nil)
		T->root = z;
	else if(z->key<y->key)
			y->left = z;
	else y->right = z;
	z->left = T->nil;
	z->right = T->nil;
	strcpy(z->color,"RED");
	insert_fixup(z);
}

void transplant(struct RB *u, struct RB *v)
{
	if(u->p==T->nil)
		T->root = v;
	else if(u==u->p->left)
			u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

struct RB * minimum(struct RB *x)
{
	while(x->left!=T->nil)
		x = x->left;
	return x;
}

void delete_fixup(struct RB *x)
{
	struct RB *w;
	while(x!=T->root && !strcmp(x->color,"BLACK"))
	{
		if(x==x->p->left)
		{
			w = x->p->right;
			if(!strcmp(w->color,"RED"))
			{
				strcpy(w->color,"BLACK");
				strcpy(x->p->color,"RED");
				left_rotate(x->p);
				w = x->p->right;
			}
			if(!strcmp(w->left->color,"BLACK") && !strcmp(w->right->color,"BLACK"))
			{
				strcpy(w->color,"RED");
				x = x->p;
			}
			else {
					if(!strcmp(w->right->color,"BLACK"))
					{
						strcpy(w->left->color,"BLACK");
						strcpy(w->color,"RED");
						right_rotate(w);
						w = x->p->right;
					}
					strcpy(w->color,x->p->color);
					strcpy(x->p->color,"BLACK");
					strcpy(w->right->color,"BLACK");
					left_rotate(x->p);
					x = T->root;
				}
		}
		else
		{
			w = x->p->left;
			if(!strcmp(w->color,"RED"))
			{
				strcpy(w->color,"BLACK");
				strcpy(x->p->color,"RED");
				right_rotate(x->p);
				w = x->p->left;
			}
			if(!strcmp(w->right->color,"BLACK") && !strcmp(w->left->color,"BLACK"))
			{
				strcpy(w->color,"RED");
				x = x->p;
			}
			else {
					if(!strcmp(w->left->color,"BLACK"))
					{
						strcpy(w->right->color,"BLACK");
						strcpy(w->color,"RED");
						left_rotate(w);
						w = x->p->left;
					}
					strcpy(w->color,x->p->color);
					strcpy(x->p->color,"BLACK");
					strcpy(w->left->color,"BLACK");
					right_rotate(x->p);
					x = T->root;
				}
		}
	}
	strcpy(x->color,"BLACK");
}

void delete(struct RB *z)
{
	struct RB *y, *x;
	char orcolor[5];
	y = z;
	strcpy(orcolor,y->color);
	if(z->left==T->nil)
	{
		x = z->right;
		transplant(z,z->right);
	}
	else if(z->right == T->nil)
		{
			x = z->left;
			transplant(z,z->left);
		}
	else
	{
		y = minimum(z->right);
		strcpy(orcolor,y->color);
		x = y->right;
		if(y->p==z)
			x->p = y;
		else
		{
			transplant(y,y->right);
			y->right = z->right;
			y->right->p = y;
		}
		transplant(z,y);
		y->left = z->left;
		y->left->p = y;
		strcpy(y->color,z->color);
	}
	if(!strcmp(orcolor,"BLACK"))
		delete_fixup(x);
}


struct RB * search(int val)
{
	struct RB *u;
	u = T->root;
	while(u!=T->nil)
	{
		if(val==u->key) break;
		if(val<u->key) u = u->left;
		else u = u->right;
	}
	return u;
}

/*void display(struct RB *z)
{
	if(z!=T->nil)
	{
		display(z->left);
		printf("\nValue: %d\tColor:  %s",z->key,z->color);
		display(z->right);
	}
}*/
void preorderDotDump (struct RB *root,FILE* outputFile)
{
	if (root != T->nil) 
	{
		if(!strcmp(root->color,"BLACK"))
			fprintf (outputFile, "%d [label=%d,color=black];\n",root->key, root->key);
		else
			fprintf (outputFile, "%d [label=%d,color=red];\n",root->key, root->key);
		if (root->left != T->nil) 
		         fprintf (outputFile, "%d -> %d ;\n", root->key, (root->left)->key);
		else if(root->left==T->nil)
          	fprintf (outputFile, "%d -> %s ;\n", root->key, "nil");
        
        	if (root->right != T->nil)
            		fprintf (outputFile, "%d -> %d;\n", root->key, root->right->key);
         
         else if(root->right==T->nil)
          	fprintf (outputFile, "%d -> %s ;\n", root->key, "nil");

        	preorderDotDump (root->left, outputFile);
        	preorderDotDump (root->right, outputFile);
    	}
}

void dotDump(struct RB *root,FILE *outFile)
{
	gName++;
	fprintf (outFile, "digraph BST {\n",gName);
	preorderDotDump (root,outFile);
    	fprintf (outFile, "}\n");
}
FILE *outputFile;

main(int argc, char** argv)
{
	struct RB *z = NULL, *res;
	FILE *pipe;
	int i;
	if (argc != 2)
		{
			printf(" need one argument  \n");
			exit(0);
		}
	T = (struct sentinel *)malloc(sizeof(struct sentinel));
	T->nil = (struct RB *)malloc(sizeof(struct RB));
	T->nil->right = NULL;
	T->nil->left = NULL;
	T->nil->p = NULL;
	strcpy(T->nil->color,"BLACK");
	T->root = T->nil;
	int ch,val;
	num = atoi(argv[1]);
	outputFile = fopen ("bst.dot", "w");
    		fclose (outputFile);
    for(i=0;i<num;i++)
    {
    cont: val = rand()%100;
    z = (struct RB *)malloc(sizeof(struct RB));
				z->key = val;
	insert(z);
	if(flag)
	{
		flag = 0;
		goto cont;
		
	}
	outputFile = fopen ("bst.dot", "a");
			if (outputFile != NULL) 
			{
					        	
				dotDump (T->root,outputFile);
			}
			fclose (outputFile);
	}
	while(1)
	{
		printf("\n1.Delete\n2.Display\nEnter choice:  ");
		scanf("%d",&ch);
		switch(ch)
		{
/*		case 1: printf("Enter value:  ");
				scanf("%d",&val);
				z = (struct RB *)malloc(sizeof(struct RB));
				z->key = val;
				insert(z);
				break;*/
		case 1: printf("Enter the key to be deleted:  ");
				scanf("%d",&val);
				res = search(val);
				if(res!=T->nil)
				{
					delete(res);
					outputFile = fopen ("bst.dot", "w");
			if (outputFile != NULL) 
			{
					        	
				dotDump (T->root,outputFile);
			}
			fclose (outputFile);
				}
				else
					printf("Key not found");
				free(res);
				break;
		case 2: pipe=popen("dot -Tps bst.dot -o bst.ps","w");
					pclose(pipe);	
					pipe=popen("evince bst.ps","r"); 
					pclose(pipe);
				break;
		default: exit(0);
		}
	}
}
