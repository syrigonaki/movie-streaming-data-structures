#include "streaming_service.h"



/*
*   Definitions for :  
*   add_new_movie
*   distribute_new_movies
*   print_movies
*   --------------------
*   print_new_movies
*   take_off_movie
*/


int add_new_movie(unsigned mid, movieCategory_t category, unsigned year) {

    struct new_movie * newnode = (struct new_movie *)malloc(sizeof(struct new_movie));
    if (newnode==NULL) return -1;

    if (new_releasesList == NULL) {
        new_releasesList = newnode;
        newnode -> next = NULL;

    } 
    else if (new_releasesList->info.mid >mid ) {
        newnode->next = new_releasesList;
        new_releasesList = newnode;
        
    } else {            /*insert in sorted list*/
        
        struct new_movie * curr=new_releasesList;
        while (curr->next!=NULL && curr->next->info.mid < mid) {
            curr=curr->next;
        }


        newnode->next = curr->next;
        curr->next= newnode;
    }

    newnode->info.mid = mid;
    newnode->info.year = year;
    newnode->category = category;
    print_new_movies(mid, category, year);
    return 0;

}


void distribute_new_movies(void) {
    struct new_movie * q=new_releasesList;

    struct movie ** last = (struct movie **)malloc(6*(sizeof(struct movie *)));
    
    int i;

    for (i=0; i<6; i++) {
        last[i]=catTable[i];
    }

    while (q!=NULL) {
        struct movie * newnode = (struct movie *)malloc(sizeof(struct movie));

        if (catTable[q->category] == NULL) {
            catTable[q->category]=newnode;
            last[q->category] = newnode;
        } else {
            last[q->category]->next = newnode;
            last[q->category]=newnode;
        }

        newnode->next = NULL;
        newnode->info = q->info;

        q=q->next;
    }

    

    free(last);

    printf("D");

    struct movie *m;
    printf("\nCategorized Movies:\n");
    int count=1;
    printf("\tHorror: ");

    m=catTable[0];
    while (m!=NULL) {
        printf("<%d, %d> ", m->info.mid, count);
        count++;
        m=m->next;
    }
    printf("\n");

    count=1;

    printf("\tSci-fi: ");
    
    m=catTable[1];
    while (m!=NULL) {
        printf("<%d, %d> ", m->info.mid, count);
        count++;
        m=m->next;
    }
    printf("\n");

    count=1;

    printf("\tDrama: ");
    
    m=catTable[2];
    while (m!=NULL) {
        printf("<%d, %d> ", m->info.mid, count);
        count++;
        m=m->next;
    }
    printf("\n");

    count=1;

    printf("\tRomance: ");
    
    m=catTable[3];
    while (m!=NULL) {
        printf("<%d, %d> ", m->info.mid, count);
        count++;
        m=m->next;
    }
    printf("\n");

    count=1;

    printf("\tDocumentary: ");
    
    m=catTable[4];
    while (m!=NULL) {
        printf("<%d, %d> ", m->info.mid, count);
        count++;
        m=m->next;
    }
    printf("\n");

    printf("\tComedy: ");
    count=1;

    m=catTable[5];
    while (m!=NULL) {
        printf("<%d, %d> ", m->info.mid, count);
        count++;
        m=m->next;
    }
    printf("\n");

    printf("DONE\n");
   

}






void print_new_movies(unsigned mid, movieCategory_t category, unsigned year) {
    printf("A <%d> <%d> <%d>\n", mid, category, year);
    printf("\tNew Movies: ");
    
    struct new_movie * q = new_releasesList;
    while (q!=NULL) {
        printf("<%d, %d, %d>, ", q->info.mid, q->category, q->info.year);
        q=q->next;
    }

    printf("\nDONE\n");



}


void print_movies() {

    struct movie *q;
    printf("M\nCategorized Movies:\n");
    int count=1;
    printf("\tHorror: ");
    
    q=catTable[0];
    while (q!=NULL) {
        printf("<%d, %d> ", q->info.mid, count);
        count++;
        q=q->next;
    }
    printf("\n");

    printf("\tSci-fi: ");
    count=1;
    q=catTable[1];
    while (q!=NULL) {
        printf("<%d, %d> ", q->info.mid, count);
        count++;
        q=q->next;
    }
    printf("\n");

    printf("\tDrama: ");
    count=1;
    q=catTable[2];
    while (q!=NULL) {
        printf("<%d, %d> ", q->info.mid, count);
        count++;
        q=q->next;
    }
    printf("\n");
    
    printf("\tRomance: ");
    count=1;
    q=catTable[3];
    while (q!=NULL) {
        printf("<%d, %d> ", q->info.mid, count);
        count++;
        q=q->next;
    }
    printf("\n");

    printf("\tDocumentary: ");
    count=1;
    q=catTable[4];
    while (q!=NULL) {
        printf("<%d, %d> ", q->info.mid, count);
        count++;
        q=q->next;
    }
    printf("\n");

    printf("\tComedy: ");
    count=1;
    q=catTable[5];
    while (q!=NULL) {
        printf("<%d, %d> ", q->info.mid, count);
        count++;
        q=q->next;
    }
    printf("\n");

    printf("DONE\n");
   
}


void take_off_movie(unsigned mid) {
    

    printf("T <%d>\n", mid);

    struct user * currUser = userList;
    

    while (currUser->uid != -1) {
       
        struct suggested_movie * s =  currUser->suggestedHead;
        struct suggested_movie * tmp ;

        while (s!=NULL) {

            
            if (s->info.mid == mid) {

                printf("\t<%d> removed from <%d> suggested list.\n", mid, currUser->uid);

               
                
                if (s->prev == NULL && s->next!=NULL) { /*this is ugly sorry*/
                    s->next->prev = NULL;
                    currUser -> suggestedHead = s->next;
                   
                }  else if (s->prev!=NULL && s->next == NULL) {
                    s->prev ->next = NULL;
                    currUser->suggestedTail = s->prev;
                } else if (s->prev == NULL && s->next == NULL) {
                    currUser->suggestedHead=NULL;
                    currUser->suggestedTail = NULL;
                } else {
                    s->prev->next = s->next;
                    s->next->prev = s-> prev;
                }
                

                tmp  = s;
                s=s->next;
                
                free(tmp);
                
            } else {
                s=s->next;
            }

        }
        currUser=currUser->next;
    }

    int cat = 0;
    int stop = 0;
   
    while (cat<6 && stop == 0 ) {
        struct movie * node = catTable[cat];
        struct movie * pq = NULL;
        
        while (node!=NULL) {

            if (node->info.mid == mid) {

                if (node==catTable[cat]) {
                    catTable[cat] = node->next;
                    free(node);
                    stop=1;
                    break;

                } else {
                    pq->next = node->next;
                    free(node);
                    stop=1;
                    break;
                }
               
            }
            pq=node;
            node=node->next;
        }
        
        cat++;
    }

    printf("\tCategory list: ");

    struct movie * m = catTable[cat-1];
    while (m!=NULL) {
        printf("<%d> ", m->info.mid);
        m=m->next;
    }
    printf("\nDONE\n");
}