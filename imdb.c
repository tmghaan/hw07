/* imdb.c

   Main program for interactive IMDB viewer.

   Name:
   Resources used (websites / peers / etc):
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "map.h"
#include "llist.h"
#include "types.h"

#include "imdb_functions.h"

// the IMDB files contain 241 header lines
#define HEADER_LINES 241

// Reads in a file containing a list of cast members
// preconditions: all_movies exists and maps all movies encountered so far
// postconditions: If the file can be read successfully, returns an array
//                 of all cast members in the file, and updates all_movies
//                 to map all movies encountered. If there is an error reading
//                 the file, returns NULL.
// Note: You are responsible for freeing the returned array (if it's not NULL).
array read_cast_member_file(char* filename, map all_movies)
{
  FILE* file = fopen(filename, "r");
  if(!file)
  {
    printf("Cannot open file.");
    return NULL;
  }

  // skip header
  for(int i = 0; i < HEADER_LINES; i++)
  {
    skip_line(file);
  }

  array cast = array_new();

  while(!feof(file))
  {
    cast_member* member = malloc(sizeof(cast_member));
    read_result result = read_cast_member(file, member, all_movies);

    switch(result)
    {
    case SUCCESS:
      array_add(cast, member);

      // This is helpful for seeing progress as you're loading a file.
      if(array_size(cast) % 100000 == 0)
	     printf("Added cast member %s\n", member->name);

      break;
    case FAILURE:
      //free(member);
      skip_line(file); // this makes sure we're always moving forward
      break;

    case END_OF_LIST:
      //free(member);
      return cast;
    }
  }

  return cast; // shouldn't get here unless file is truncated
}

/*void HELP(char* line) {
  char *temp;
  temp = strchr(line, '\n');
  *temp = '\0';
} */

// Our main program loop.
// See King, p. 302, about argc and argv.
int main(int argc, char** argv)
{
  // if(argc < 2)
  // {
  //   printf("Usage: %s [cast_member lists]\n", argv[0]);
  //   return 1;
  // }

  map all_movies = map_new();
  array all_cast = array_new();

  // start i at one to skip program name
//  for(int i = 0; i < argc; i++)
  {
    //array male_cast = read_cast_member_file("actors.list", all_movies);
    //array female_cast = read_cast_member_file("actresses.list", all_movies);

    array male_cast = read_cast_member_file("actors.list", all_movies);
    array female_cast = read_cast_member_file("actresses.list", all_movies);

    /*if(!male_cast)
    {
       // file reading failed, but read_cast_member_file alerted the user already
      continue;
    }*/


    // WRITE CODE HERE
    // At this point, some_cast has the cast members most recently read in,
    // and all_cast has all previous cast members.
    // You need to merge (with a call to merge_arrays) these two arrays, producing
    // a new all_cast that contains both.

    //all_cast = male_cast;
    all_cast = merge_arrays(male_cast, female_cast);

    //printf("Added cast member %s\n", *all_cast->mem[i]);
  }



  for(;;)
  {
    char name[200];
    printf("Enter name: ");
    fgets(name,200,stdin);
    //HELP(name);
    size_t size = strlen(name)-1;

    if(name[size] == '\n') {
      name[size] = '\0';
    }

    if(find_cast_member(all_cast, name) != NULL) {
      printf("Found!\n");

    }

    else{
      printf("Not found.\n");
    }

    //return movie list

    // WRITE CODE HERE
    // This is the main interactive loop, which you must write, as described
    // in the assignment.
    // When the user is done, this should `break`.
  }

  // WRITE CODE HERE
  // Free all used memory before exiting.

  return 0;
}
