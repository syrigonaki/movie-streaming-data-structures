#include "streaming_service.h"


/*
*   Definitions for :  
*   watch_movie
*   suggest_movies
*   filtered_movie_search
*   ---------------------
*   print_watch_history
*   print_suggested
*   Push
*   Pop
*/

 

struct movie * Pop(struct user * u) {
    
    if (u->watchHistory==NULL) return NULL;

    struct movie * x = (struct movie *)malloc(sizeof(struct movie));

    x->info =  u->watchHistory->info;
    struct movie * tmp = u->watchHistory;
    u->watchHistory = u->watchHistory->next;
    free(tmp);

    return x;

}

 struct movie * Push(struct movie_info i, struct movie * w) {

    struct movie * node = (struct movie *)malloc(sizeof(struct movie));
    if (node == NULL) return NULL;

    node->info.mid = i.mid;
    node->info.year= i.year;
    node->next=w;
    w = node;
    return w;
}


/*watch*/

int watch_movie(int uid, unsigned mid) {
    int cat = 0;
    int found = 0;
    struct movie * currMovie = NULL;
    

   while (cat < 6 && found == 0) {         /*finding movie in category table*/
        
        currMovie = catTable[cat];
        
        while (currMovie != NULL && found ==0) {
            if (currMovie->info.mid == mid) {
                found = 1;
            } else {
                currMovie = currMovie ->next;
            }
            

        }
        
        cat++;

    }

    if (currMovie == NULL) return -1;
    
    

    struct user * currUser = userList;

    while (currUser->uid != -1 && currUser->uid != uid) {
        currUser = currUser->next;
    }

    
    if (currUser->uid == -1) return -1;

    struct movie * newTop = Push(currMovie->info, currUser->watchHistory);

    if (newTop == NULL) return -1;

    currUser->watchHistory = newTop;
    
    printf("W <%d> <%d>\n", uid, mid);
    printf("\tUser <%d> Watch History = ", uid);

    print_watch_history(currUser);

    printf("\n");

    return 0;
}

void print_watch_history(struct user * u) {

      struct movie * m = u->watchHistory;

	    while (m!=NULL) {
		    printf("<%d> ", m->info.mid);
		    m=m->next;
	    }

        printf("\nDONE\n");

}


        
/*watch*/

/*suggest*/

int suggest_movies(int uid) {


    int count = 1;
    int empty = 0; 
    struct user * currUser = userList;
  
    while (currUser->uid != -1 && currUser->uid != uid) {   /*finding pointer to user with given uid*/
        currUser = currUser->next;
    }


    if (currUser->uid==-1) return -1;
    
    struct suggested_movie * left=NULL;
    struct suggested_movie * right=NULL;
    struct user * u = currUser;

    currUser = userList;
    
    struct suggested_movie * oldhead=u->suggestedHead; /*saving list in case it is not empty*/
    struct suggested_movie * oldtail=u->suggestedTail;

    u->suggestedHead=NULL;
    u->suggestedTail = NULL;


    while (currUser->uid != -1) {
            
        if (currUser->uid !=uid ) {
            
           
            struct movie * suggestion = Pop(currUser);
            

            if (suggestion == NULL) { /*no movies in watch hsitory*/
                currUser = currUser ->next;
                continue;
            }

            currUser->watchHistory = Push(suggestion->info, currUser->watchHistory);

            struct suggested_movie * new = (struct suggested_movie *)malloc(sizeof(struct suggested_movie));

            
            if (new==NULL) return -1;   /*malloc failure*/

            new->info = suggestion->info;
            
            if (count % 2 != 0) {
                

                if (u->suggestedHead==NULL )  {
                   
                    new->prev = NULL;
                    new->next=NULL;
                    u->suggestedHead = new;
                    left=u->suggestedHead;
                } else {

                    
                    left->next = new;
                    new->prev = left;
                    new->next = NULL;
                    left = new;
                     
                }

            
            } else {

                

                if (u->suggestedTail==NULL) {
                    new->prev = NULL;
                    new->next=NULL;
                    u->suggestedTail = new;
                    right=u->suggestedTail;
                    
                } else {
                    right->prev=new;
                    new->prev = NULL;
                    new->next = right;
                    right = new;
                    
                }
                
            }
        
            count++;

            
        }
            
        currUser=currUser->next;
        
    }


    if (count > 1 && oldhead==NULL) {
        left->next = right;
        right->prev = left;
    } else if (oldhead!=NULL){
        left->next = oldhead;
        oldhead->prev = left;
        right->prev = oldtail;
        oldtail->next=right;    
    }

    printf("S <%d>\n", uid);
    printf("\tUser <%d> Suggested Movies: ", uid);
    print_suggested(u);

    return 0;


}


void print_suggested(struct user * u) {

    struct suggested_movie * m = u->suggestedHead;

        while (m!=NULL) {
		    printf("<%d> ", m->info.mid);
		    m=m->next;
	    }

        printf("\nDONE\n");

}

/*suggest*/


/*filter*/

int filtered_movie_search(int uid, movieCategory_t category1, movieCategory_t category2, unsigned year) {

    struct user * u = userList;
    
    while (u->uid != -1 && u->uid != uid) {   /*finding pointer to user with given uid*/
        u = u->next;
    }

    
    if (u->uid == -1) return -1;
    

    struct movie * a=catTable[category1];
    struct movie * b=catTable[category2];
    int res=0;
      
    while (a!= NULL && b!=NULL) {
        

        if (a->info.year < year) {
            a=a->next;
            continue;
        }

        if (b->info.year < year) {
            b=b->next;
            continue;
        }   


        if (a->info.mid < b->info.mid) { 

           res=insert(u, a->info);
            if (res!=0) return -1;
            a=a->next;
        } else {

            res=insert(u, b->info);
            if (res!=0) return -1;
            b=b->next;
        }
        
    }
    

    if (a!=NULL && b==NULL) {
        
        while (a!= NULL) {

           if (a->info.year<year) {
                a=a->next;
                continue;
           }
           res=insert(u, a->info);
           if (res!=0) return -1;
           a=a->next;
        }

    } 
    
    if (a==NULL && b!=NULL) {
        while (b!=NULL) {
            if (b->info.year<year) {
                b=b->next;
                continue;
           }
            res=insert(u, b->info);
            if (res!=0) return -1;
            b=b->next;
        }

    }

    

    printf("F <%d> <%d> <%d> <%d>\n", uid, category1, category2, year);
    printf("\tUser <%d> Suggested Movies: ", uid);
    print_suggested(u);
    
    return 0;

}


int insert(struct user * u, struct movie_info info) {

    struct suggested_movie * new = (struct suggested_movie *)malloc(sizeof(struct suggested_movie));
    if (new==NULL) return -1;

    if (u->suggestedTail == NULL) {
        u->suggestedTail = new;
        u->suggestedHead = new;
        new->info = info;
        new->next=NULL;
        new->prev=NULL;
    } else {
        new->info = info;
        u->suggestedTail->next = new;
        new->prev = u->suggestedTail;
        u->suggestedTail = new;
        new->next = NULL;
    }

    return 0;
}   
