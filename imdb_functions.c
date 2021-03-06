/* imdb_functions.c
   Name: Cecilia Han Truong
   Resources used (websites / peers / etc):
   Youtube Video about Binary Search : https://www.youtube.com/watch?v=JQhciTuD3E8
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "imdb_functions.h"
#include "types.h"
#include "array.h"

#define STRING_SIZE  200
#define LEN "199"

// Discards all characters until the end of a line in the given file
 void skip_line(FILE* file)
 {
   while(!feof(file) && getc(file) != '\n')
     ;
 }

// copy the given string into a newly-malloc'd buffer
char* malloc_string(char* str)
{
  char* new = malloc(sizeof(char) * (strlen(str)+1));
  strcpy(new, str);
  return new;
}

// like strcmp, but ignores differences in case (why does this work?)
 int stricmp(char* s1, char* s2)
 {
   while(true)
   {
     char c1 = *s1++;
     char c2 = *s2++;
     int d = toupper(c1) - toupper(c2);

     if(d != 0 || !c1)
     {
       return d;
     }
   }
 }


// is the given string composed entirely of dashes?
bool all_dashes(char* str)
{
  while(*str)
  {
    if(*str++ != '-')
    {
      return false;
    }
  }
  return true;
}

// are there any non-ascii characters in the string?
 bool any_bad_chars(char* str)
 {
   while(*str)
   {
     char c = *str++;
     if(c < 0)
     {
       return true;
     }
   }
   return false;
 }

// Reads in a cast member from the given file
// preconditions: the file is open and at a line where a cast member begins
//                member points to a valid cast_member struct
//                all_movies is a map of all known movies
// postconditions: if a cast_member can successfully be read from the file,
//                 *member is filled in. Any new movies found while reading
//                 the cast member are added to all_movies, and SUCCESS is returned
//                 Otherwise, if the end of the list of members is reached,
//                 END_OF_LIST is returned. Otherwise, FAILURE is returned.

read_result read_cast_member(FILE* file, cast_member* member, map all_movies)
{
  char buf[STRING_SIZE];

  if(fscanf(file, "%" LEN "[^\t\n]", buf) != 1) return FAILURE; //Why is there no letters behind %? Does that mean fscanf will read in any type of data?

  if(all_dashes(buf)) return END_OF_LIST;

  // non-ascii chars are alphabetized differently, so we can't handle them here
  if(any_bad_chars(buf)) return FAILURE;

  // At this point, `buf` contains the name of the cast member, and you can fill in the two data fields of *member.
  	member->name = malloc(sizeof(buf)); // allocate the memory for member->name
  	strcpy(member->name, buf);	// copy all the stuff from buf to member->name
  	member->movies = llist_new(); // make a new llist for member->movie since movie is defined as a list in cast_member struct

  while(fscanf(file, "%*[\t]%" LEN "[^\n]", buf) == 1)
  {
    skip_line(file); // eat rest of line

    // cut it off at the first instance of 2 spaces
    char* spaces = strstr(buf, "  ");
    if(spaces)
    {
      *spaces = '\0'; // cut off the string here
    }

    // At this point, `buf` contains the name of the movie. You should
    //   1. Look up the movie in all_movies to see if it already exists.
    //   2. If not, create (malloc) a new movie object and add it to the map.
    //   3. Update the movie to include the cast member.
    //   4. Update the cast member to include the movie.

    //What's key: key is a pointer to an array of char of movie name
    //What's value: value is pointer to a movie struct that has name of movie and cast member
     //1.
    if (!map_contains(all_movies, buf)){
      movie* new_movie = malloc(sizeof(movie)); //1.
    	new_movie->name = malloc_string(buf);
      char* key = malloc_string(buf);
      map_put(all_movies, key, new_movie); //2.
    	new_movie->cast = array_new();
    }
    movie* new_movie = map_get(all_movies, buf);
    array_add(new_movie->cast, member); //3. ??????? cast or name
    llist_add(member->movies, new_movie); //4.
  }
  return SUCCESS;
}

// Merges two sorted arrays. Runs in time linear to the length of the two input arrays.
// preconditions: src1 and src2 are two valid arrays.
//                src1 and src2 are both in sorted order
// postconditions: returns a fresh array containing all the elements of src1 and src2
//                 in sorted order
// Note: You are responsible for freeing the returned array.
array merge_arrays(array src1, array src2)
{
  // WRITE CODE HERE
  int size1 = array_size(src1);
  int size2 = array_size(src2);
  array merged_array = array_new();
  int i=0, j=0;

  while ( i < size1 && j < size2 ){

    if (stricmp(array_get(src1,i)->name , array_get(src2,j)->name) > 0){
      array_add(merged_array, array_get(src2,j));
      //free((array_get(src2,j)));
      //add src2[i] to the array
      j++;
    }

    else {
      array_add(merged_array, array_get(src1,i));
      i++;
    }

    /*
    else if (stricmp(array_get(src1,i)->name, array_get(src2,j)->name) < 0){
      array_add(merged_array, array_get(src1,i));
      //free((array_get(src1,i)));
      //add src1[i] to the array
      i++;
    }

    else if (stricmp(array_get(src1,i)->name, array_get(src2,j)->name) == 0){
      //add src1[i] to the array
      //add src2[j] to the array
      array_add(merged_array, (array_get(src1,i)));
      //free(array_get(src1,i));
      i++;
      array_add(merged_array, (array_get(src2,j)));
      //free(array_get(src2,j));
      j++;
    }
    */
  }

    //if size of src1 is bigger
    if (size1 > size2) {
      for (int k = size2; k < size1; k++) {
array_add(merged_array, array_get(src1,k));
      }
    }

    //if size of src2 is bigger
    if (size2 > size1) {
      for (int t = size1; t < size2; t++) {
array_add(merged_array, array_get(src2,t));
      }
    }

  return merged_array;
}

// Finds a cast member in a sorted array of cast members
// preconditions: cast is a valid, sorted array of cast members
//                name is a valid string
// postconditions: If the cast member can be found, returns a pointer to the member
//                 Otherwise, returns NULL.
cast_member* find_cast_member(array cast, char* name){

  int lo = 1;
  int hi = (array_size(cast))-1;
  int mid = 0;

  while (lo <= hi){
    mid = lo + (hi-lo)/2;

    //if input name matches with
    if ( stricmp(array_get(cast,mid)->name,name) == 0){
      return array_get(cast,mid);
    }
    else if ( stricmp(array_get(cast,mid)->name, name) < 0){
      lo = mid + 1; // d returns the difference between two letters, how is that significant to lo, hi and mid?
    }
    else{
       //
       hi = mid - 1;
    }
  }
  return NULL;
}
