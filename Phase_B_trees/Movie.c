/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"


/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

/*additions*/
void initialize_hashtable(){
	
	int max = max_users;
	int i=0;
	while (primes_g[i]<= max) {
		i++;
	}
	hashtable_size = primes_g[i];

	i=0;
	while (primes_g[i]<= max_id) {
		i++;
	}

	p=primes_g[i];
	
 }


 void print_only_users(int userID, int pos) {
	
	
	printf("Chain <%d> of Users:\n", pos);

	user_t *q = user_hashtable_p[pos];
	printf("\t");
	while (q!=NULL) {
		printf("<%d> ", q->userID);
		q=q->next;
	}

	

 }

 void delete_tree(userMovie_t * R) { /*leaf-oriented binary search tree*/
	if (R==NULL) return;
	delete_tree(R->lc);
	delete_tree(R->rc);
	free(R);
 } 

 void print_new_movies(new_movie_t * R) { /*in -order*/
	if (R==NULL) return;
	print_new_movies(R->lc);
	printf("<%d> ", R->movieID);
	print_new_movies(R->rc);
 }

 void print_only_movies(movie_t * R, movie_t * s) { /*in -order*/
	if (R==s) return; /*change to guard node plis*/
	print_only_movies(R->lc, s);
	printf("<%d> ", R->movieID);
	print_only_movies(R->rc, s);
}
 

void count(int * arr, new_movie_t * R ) {
	if (R==NULL) return;
	count(arr, R->lc);
	count(arr, R->rc);
	arr[R->category]++;
}

int TreeToArray( new_movie_t ** arr[] , int category, new_movie_t * R, int index) {
	if (R==NULL) return index;
	index = TreeToArray(arr, category, R->lc, index);
	if (R->category ==  category) {
		arr[category][index]=R;
		index = index + 1;
	}
	index = TreeToArray(arr,category,  R->rc, index);
	return index;
}

movie_t * ArrayToTree(new_movie_t ** array, movie_t * sentinel, int left, int right) {
	if (left > right) {
		return NULL;
	}
	

	int middle = (left + right) /2;
	movie_t * new = (movie_t * )malloc(sizeof(movie_t));
	
	new->movieID = array[middle]->movieID;
	new->year = array[middle]->year;
	new->sumScore = 0;
	new->watchedCounter = 0;
	

	new->lc = ArrayToTree(array, sentinel, left, middle -1);
	if (new->lc == NULL) {
		new->lc = sentinel;
	}
	
	new->rc = ArrayToTree(array, sentinel, middle+1, right);
	
	if (new->rc == NULL) {
		new->rc = sentinel;
	}
	return new;
}


user_t * find_user(int userID) {
	user_t * q;
	for (int i=0; i<hashtable_size; i++) {
		q=user_hashtable_p[i];
		while (q!=NULL && q->userID != userID) {
			q=q->next;
		}
		if (q!=NULL) {
			return q;
		}
	}
	return NULL;
}

void insert_history_tree(user_t * user, userMovie_t * new, int score) {
	userMovie_t * q = user->history;
	if (q==NULL) {
		new->parent = NULL;
		new->lc = NULL;
		new->rc = NULL;
		user->history = new;
		return;
	}
	while (q->lc != NULL ) {
		if (q->movieID > new->movieID) {
			q=q->lc;
		} else {   
			q=q->rc;
		}
		
	}

	if (q->movieID == new->movieID) {
		q->score = score;
		return;
	} 

	userMovie_t * n = (userMovie_t *)malloc(sizeof(userMovie_t));

	
	n->parent = q; /*copying q node to n*/
	new->parent = q;

	n->movieID = q->movieID;
	n->category = q->category;
	n->score = q->score;

	n->lc = NULL; n->rc = NULL;

	if (q->movieID < new->movieID) {
		q->lc = n;
		q->rc = new;

		q->movieID = new->movieID; /*changing values of node q*/
		q->category = new->category;
		q->score = new->score;
	} else if (q->movieID > new->movieID) {
		q->lc = new;
		q->rc = n;
	}

	return;

}


void print_history_tree(userMovie_t * R) {
	if (R==NULL) return;
	print_history_tree(R->lc);
	print_history_tree(R->rc);

	if (R->lc == NULL) {
		printf("\t<%d, %d>\n", R->movieID, R->score);
	}
	return;
}
void print_in_order (userMovie_t * R) {
	if (R==NULL) return;
	print_in_order(R->lc);
	printf("\t<%d, %d>\n", R->movieID, R->score);
	print_in_order(R->rc);

}



int find_numMovies(movie_t * R, int numMovies, int score) {
	if (R==NULL) return 0;
	int l = find_numMovies(R->lc, numMovies, score);
	int r = find_numMovies(R->rc, numMovies,score);
	
	int num=0;
	if (R->watchedCounter==0) l+r;
	if ((float)R->sumScore / (float)R-> watchedCounter > score) {
		return l+r+1;
	} 
	return l+r;
	
}

int add_to_array(movie_t * R, movie_t ** arr, int score, int index) {
	if (R==NULL) return index;
	index = add_to_array(R->lc, arr, score, index);
	index = add_to_array(R->rc, arr, score, index);

	if ((float)R->sumScore / (float)R-> watchedCounter > score) {
		arr[index++] = R;
	}

	return index;
}

/*void print_arr(movie_t ** arr, int length) {
	printf("ARRAY: ");
	for (int i =0; i<length;i++) {
		printf("%.2f ", (float) arr[i]-> sumScore/ (float) arr[i]->watchedCounter);
	}
	printf("\n");
}*/

void create_max_heap(movie_t ** arr, int i, int length) { /*doesn't work need to fix*/

		int leftChild = 2*i + 1;
		int rightChild = 2*i + 2;
		int key = i;
		float key_avg = (float) arr[key]-> sumScore/ (float) arr[key]->watchedCounter; /*average score*/

		
		if (rightChild<length) {
			float right_avg = (float)arr[rightChild]->sumScore/(float)arr[rightChild]->watchedCounter; 
			if (key_avg < right_avg) {
				key = rightChild;
				key_avg = right_avg;
			}
		}

		if (leftChild<length) {
			float left_avg = (float)arr[leftChild]->sumScore/(float) arr[leftChild]->watchedCounter;
			if (key_avg < left_avg) {
				key = leftChild;
				key_avg = left_avg;
			}
		}

		if (key != i) {
			movie_t * tmp = arr[i]; 	/*swapping elements*/
			arr[i]=arr[key];
			arr[key]= tmp;
			create_max_heap(arr, key, length);
		}
		
		
		
}



void heapsort(movie_t ** arr, int length) {
	for (int i = length/2 -1 ; i>-1; i--) {
		create_max_heap(arr, i, length);
		
	}
	for (int i = length -1 ; i >0 ; i--) {
		
		movie_t * tmp = arr[0]; /*swapping elemets*/
		arr[0]=arr[i];
		arr[i]=tmp;
		
		create_max_heap(arr, 0, i);
		
	}
}

userMovie_t * FindNextLeaf(userMovie_t * currLeaf, userMovie_t * root) { /*Needs O(h) complexity*/
	if (currLeaf == NULL) return NULL;
	userMovie_t * q;
	
	if (currLeaf == currLeaf->parent->lc) {
		q = currLeaf->parent->rc;
		while (q-> lc!=NULL) {
			q=q->lc;
		}
		return q;
	} 
	
	q=currLeaf;
	while(q == q->parent->rc) {
		q=q->parent;
		if (q==root) return NULL;
	}
 	userMovie_t * p = FindNextLeaf(q, root);	
	return p;
}


/**/


int register_user(int userID) {


	if (user_hashtable_p==NULL) {
		initialize_hashtable();
		user_hashtable_p = (user_t **)malloc(hashtable_size*sizeof(user_t *));
	}
		
	if (userID > max_id) return 0;



	
	int pos = (userID)%hashtable_size;
	

	user_t * new = (user_t *)malloc(sizeof(user_t));
	if (new==NULL) return 0; /*malloc failure*/

	new->userID = userID;
	new->history = NULL;
	
	if (user_hashtable_p[pos]==NULL) {
		new->next = NULL;
	} else {
		new->next = user_hashtable_p[pos];
	}

	user_hashtable_p[pos]=new;
	

	//printf("R <%d>\n", userID);
	print_only_users(userID, pos);
	printf("\nDONE\n");
	return 1;
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID) {
	if (userID>max_id) return 0;

	int pos = userID%hashtable_size;

	user_t * q = user_hashtable_p[pos];
	user_t * pq = NULL;

	while (q!=NULL && q->userID!=userID) {
		pq=q;
		q=q->next;
	}

	if (q==NULL) return 0;

	/*delete watch history tree*/

	if (q->history != NULL) delete_tree(q->history);
	

	/*delete from list*/
	if (pq==NULL) {
		user_hashtable_p[pos]= q->next;
	} else {
		pq->next = q->next;
	}

	free(q);
	//printf("U <%d>\n", userID);
	print_only_users(userID, pos);

	return 1;
	
 }
 
 /**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

 int add_new_movie(int movieID, int category, int year){ 

	/*insert in new_releases - type is new_movie_t*/
	/*sorted binary tree*/
	new_movie_t * new = (new_movie_t *)malloc(sizeof(new_movie_t));
	
	if (new==NULL) return 0; /*malloc failure*/

	new->year = year;
	new->movieID = movieID;
	new->category = category;
	new->lc = NULL;
	new->rc = NULL;

	if (new_releases == NULL) {
		new_releases = new; 
		printf("New releases Tree:\n\t<new_releases>: ");
		print_new_movies(new_releases);
		printf("\nDONE\n");
		return 1;
	}
	new_movie_t * q = new_releases;
	new_movie_t * pq = NULL;
	while (q!=NULL) {
		if (q->movieID>movieID) { 
			pq=q;
			q=q->lc;
		} else if (q->movieID<movieID) {
			pq=q;
			q=q->rc;
		} else { /*movie already exists*/
			free(new);
			return 0;
		} 
	}

	if (pq->movieID > movieID) {
		pq->lc = new;
	} else {
		pq->rc = new;
	}
	
	//printf("A <%d> <%d> <%d>\n",movieID, category, year );
	printf("New releases Tree:\n\t<new_releases>: ");
	print_new_movies(new_releases);
	printf("\nDONE\n");
	return 1;
 }
 
 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

 int distribute_movies(void){
	new_movie_t **arr[6]; 
	int catCount[6];
	for (int i=0; i<6; i++) catCount[i]=0;
	count(catCount, new_releases); /*counting movies in each category*/
	
	for (int i = 0; i<6; i++) {
		if (catCount[i]!=0) {
			arr[i] = (new_movie_t **)malloc(catCount[i]*sizeof(new_movie_t *)); /*allocating memory for array*/
			if (arr[i]==NULL) return 0; /*malloc failure*/
		} else {
			arr[i]=NULL;
		}
		TreeToArray(arr, i ,new_releases, 0); /*adds each movie from the new_releases tree to the corresponding array*/
		
	}
	
	
	for (int i=0; i<6; i++) {
		categoryArray[i]=(movieCategory_t *)malloc(sizeof(movieCategory_t)); 
		if (categoryArray[i]==NULL) return 0;

		categoryArray[i]->sentinel=(movie_t *)malloc(sizeof(movie_t));
		if (categoryArray[i]->sentinel==NULL) return 0;

		categoryArray[i]->sentinel->movieID=-1;
		categoryArray[i]->sentinel->watchedCounter=0;
		categoryArray[i]->sentinel->sumScore=0;
		
		if (catCount[i]==0) {
			categoryArray[i]->movie = categoryArray[i]->sentinel;
			continue;
		}
		categoryArray[i]->movie = ArrayToTree(arr[i], categoryArray[i]->sentinel, 0, catCount[i]-1); /*turns category arrays into trees with a sentinel node*/
		
	}
	
	//printf("D\nMovie Category Array:\n");
	for (int i=0; i<6; i++) {
		printf("\t<Category %d>: ", i);
		print_only_movies(categoryArray[i]->movie, categoryArray[i]->sentinel);
		printf("\n");
	}
	printf("DONE\n");
	return 1;
 }
 
 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

 int watch_movie(int userID,int category, int movieID, int score){

	if (category > 5) return 0;

	user_t * u = find_user(userID);
	if (u==NULL) return 0; /*user does not exist*/

	categoryArray[category]->sentinel->movieID=movieID; /*search*/
	movie_t * q = categoryArray[category]->movie;

	while (q->movieID != movieID) {
		if (q->movieID > movieID) q=q->lc;
		else if (q->movieID < movieID) q=q->rc;
	}

	if (q!=categoryArray[category]->sentinel) {
		q->sumScore += score;
		q->watchedCounter++;
	} else {
		return 0; /*movie not found*/
	}

	userMovie_t * new = (userMovie_t *)malloc(sizeof(userMovie_t));
	if (new==NULL) return 0; /*malloc failure*/

	new->movieID = movieID;
	new->category = category;
	new->score = score;

	insert_history_tree(u ,new, score);
	


	//printf("W <%d> <%d> <%d> <%d>\n", userID, category, movieID, score);
	printf("History Tree of user <%d>:\n", userID);
	print_history_tree(u->history); /*I don't know which version of print to use. It says to print in order but that doesn't make sense for a leaf oriented tree :( */
	//print_in_order(u->history);
	printf("DONE\n");
	return 1;
}
 

/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, int score) {

	/*go to each movie, see if avg score is bigger than score num++, make array, do again, put in array, heap sort D: */

	user_t * u = find_user(userID);
	if (u==NULL) return 0; /*user does not exist*/
	
	
	int numMovies=0;
	
	for (int i = 0; i<6; i++) {
		
		numMovies += find_numMovies(categoryArray[i]->movie, 0, score);
	}
	
	if (numMovies!=0) {
		movie_t ** arr = (movie_t **)malloc(numMovies*sizeof(movie_t *));
		if (arr==NULL) return 0; /*malloc failure*/
		int last_index=0;
		for (int i = 0; i<6; i++) {
			last_index = add_to_array(categoryArray[i]->movie, arr, score, last_index);
		}


	if (last_index == 0) return 0; /*Root was NULL :\ */
	


	heapsort(arr, numMovies);
	
	//printf("F <%d> <%d>\n", userID, score );
	for (int i=0; i<numMovies; i++) {
		printf("<%d><%.2f> ", arr[i]->movieID, (float)arr[i]->sumScore / (float)arr[i]->watchedCounter);
	}
	
	printf("\nDONE\n");
	return 1;

	} else {
		printf("F <%d> <%.2d>\n", userID, score);
		printf("DONE\n");
	}


	return 1;
 }
 
/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the userprintf
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID){

	user_t * u = find_user(userID);
	if (u==NULL) return 0; /*user doesn't exist*/

	
	if (u->history == NULL) {
		printf("Q <%d> <0>\nDONE\n", userID);
		return 1;
	}

		
	int sumScore = 0;
	int counter = 0;
	
	userMovie_t * h = u->history; /*finding most left leaf*/	
	while(h->lc!=NULL) h=h->lc;
	


	while (h!=NULL) {
		sumScore+=h->score;
		counter++;
		h=FindNextLeaf(h, u->history);
	}

	printf("Q <%d> <%.2f>\nDONE\n", userID, (float)sumScore/(float)counter);


	return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category){

	categoryArray[category]->sentinel->movieID=movieID;
	movie_t * q = categoryArray[category]->movie;

	while (q->movieID != movieID) {
		if (q->movieID > movieID) q=q->lc;
		else if (q->movieID < movieID) q=q->rc;
	}

	if (q!=categoryArray[category]->sentinel) {
		printf("I <%d> <%d> <%d>", movieID, category, q->year);
		printf("\nDONE\n");
		return 1;
	}

	return 0;
 }
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
	printf("Movie Category Array:\n");
	for (int i=0; i<6; i++) {
		
		printf("\t<Category %d>: ", i);
		print_only_movies(categoryArray[i]->movie, categoryArray[i]->sentinel);
		printf("\n");
	}
	printf("DONE\n");
	return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){

	printf("P\n");
	user_t * u;
	for (int i =0 ; i<hashtable_size; i++) {
		printf("Chain <%d> of Users:\n", i);
		u=user_hashtable_p[i];
		while (u!=NULL) {
			printf("\t<%d>\n", u->userID);
			printf("\tHistory Tree: \n");
			//print_in_order(u->history);
			print_history_tree(u->history);
			u=u->next;
		}
		printf("\n\n");
	}

	printf("DONE\n");

	return 1;
 }
 



