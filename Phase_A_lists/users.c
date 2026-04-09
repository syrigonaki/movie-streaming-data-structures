

#include "streaming_service.h"


/*
*   Definitions for :  
*   register_user
*   unregister_user
*   print_users
*   ---------------------
*   print_just_user
*/


int register_user(int uid) { /*add node to unsortedlist*/
 
    if (userList->uid != -1) {
        struct user * q = userList;      
        while (q->uid != uid && q->uid != -1) { q=q->next; } /*checking if this user already exists*/
        if (q->uid != -1) return -1; /*user already exists*/
    }

    struct user * newnode = (struct user *)malloc(sizeof(struct user)); /*creating new node with given uid*/

    if (newnode == NULL) return -1;         

    newnode->uid=uid;               /*insertion at head : O(1) complexity*/
    newnode->suggestedHead=NULL;
	newnode->suggestedTail=NULL;
	newnode->watchHistory=NULL;

    newnode->next=userList;
    userList = newnode;

    printf("R <%d>\n", uid);
    print_just_user();

    return 0;
}


void unregister_user(int uid) { /*clear suggested movies and watch history*/ 
    struct user *q=userList;
    struct user * pq = userList;
    while (q->uid != -1 && q->uid != uid) { 
        pq = q;
        q = q->next; 
    }

    if (q->uid == -1) return; /*user doesn't exist*/

    struct movie * tmpstack;

    while (q->watchHistory != NULL) {  /*free watchHistory stack*/
        tmpstack = q->watchHistory;
        q->watchHistory = q->watchHistory->next;
        free(tmpstack);
    }

    q->watchHistory=NULL;

    struct suggested_movie * tmplist;         /*free suggestedMoviesList*/
    while (q->suggestedHead != NULL) {
        tmplist = q->suggestedHead;
        q->suggestedHead = q->suggestedHead->next;
        free(tmplist);
    }

    q->suggestedHead = NULL;
    q->suggestedTail = NULL;

    if (userList == q) {
        userList=q->next;;
    } else {
        pq->next = q->next;
    }
    free(q);

    printf("U <%d>\n", uid);
    print_just_user();

}


void print_users() {

    struct user * q = userList;
    int count;
    printf("P\nUsers\n");

    while (q->uid != -1) {
       
        printf("\t< %d >:\n", q->uid);
        printf("\t\tSuggested: ");

        struct suggested_movie * s = q->suggestedHead;
        count=1;
        while (s!=NULL) {
            printf("<%d, %d> ", s->info.mid, count );
            count++;
            s=s->next;
        }

        printf("\n");

        printf("\t\tWatchHistory: "); 

         
        struct movie * m = q->watchHistory;
        count=1;
	    while (m!=NULL) {
		    printf("<%d, %d> ", m->info.mid, count);
            count++;
		    m=m->next;
	    }

        printf("\n");

        q=q->next;
            
    }
     printf("DONE\n");
        
}

   

   



void print_just_user() {
    struct user * q = userList;
    printf("\tUsers: ");
    while (q->uid!=-1) {
        printf("<%d> ",q->uid);
        q=q->next;
    }
    printf("\nDONE\n");
}



