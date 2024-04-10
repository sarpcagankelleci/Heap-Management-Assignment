
// Write your full name: Sarp Çağan Kelleci, write your KU ID: 79482
#include <stdio.h>
#include <stdlib.h>

#define EMPTY '-'
#define HUMAN 'H'
#define ZOMBIE 'Z'

// Function Prototypes
char** read_map_from_txt(const char* filename, int *width, int *height);
void print_grid(char** grid, int width, int height);
void simulate(char** grid, int width, int height, int *humans_survived, int *humans_infected, int *zombies_dead);
char** create_temp_grid(int width, int height);
int count_neighbors(char** grid, int width, int height, int row, int col, char entity);
int check_humans_left(char** grid, int width, int height);
void move_humans(char** grid, char** temp_grid, int width, int height, int simulation_step);
void move_zombies(char** grid, char** temp_grid, int width, int height, int simulation_step);
void apply_rules(char** grid, char** temp_grid, int width, int height, int *humans_survived, int *humans_infected, int *zombies_dead, int simulation_step);
void cleanup_grid(char** grid, int height);

int main(int argc, char **argv) {
    int width, height;
    char *filename = argv[1];

    char **grid = read_map_from_txt(filename, &width, &height);

    printf("Initial Grid:\n");
    print_grid(grid, width, height);

    int humans_infected = 0;
    int humans_survived = 0;
    int zombies_dead = 0;

    simulate(grid, width, height, &humans_survived, &humans_infected, &zombies_dead);

    printf("Humans survived: %d\nHumans infected: %d\nZombies died: %d\n", humans_survived, humans_infected, zombies_dead);
    printf("Final Grid:\n");
    print_grid(grid, width, height);

    cleanup_grid(grid, height);

    return 0;
}

char** read_map_from_txt(const char* filename, int *width, int *height) {
    // This function should read width and height from the given filename and assign these values to the pointers given.
    // It also should allocate the memory for the grid and return a pointer for grid.
    // Write your code below

    FILE* file = fopen(filename, "r");  //  checking whether file is opened
    if (file == NULL) {
        fprintf(stderr, "Error in opening file\n");
        return NULL;
    }

    fscanf(file, "%d", width);  // width is located in the first line
    fscanf(file, "%d", height); // height is located in the second line

    char** grid = (char**)malloc(*height * sizeof(char*));  // allocation of memory for height in grid
    for (int i = 0; i < *height; i++) {  // Iteration over each row
        grid[i] = (char*)malloc(*width * sizeof(char));  // allocation of memory for width in grid
        for (int j = 0; j < *width; j++) {  // Iteration over each column
            fscanf(file, " %c", &grid[i][j]);
        }
    }

    fclose(file);  // closing the file

    return grid;  // returning the grid value
}
void print_grid(char** grid, int width, int height) {
    // This function should print the grid as in the pdf file for the assignment.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

char** create_temp_grid(int width, int height) {
    // Function to create a template grid to perform on
    char** temp_grid = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        temp_grid[i] = (char*)malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            temp_grid[i][j] = EMPTY;
        }
    }
    return temp_grid;
}

void move_humans(char** grid, char** temp_grid, int width, int height, int simulation_step) {
    int dx = (simulation_step % 2 == 0) ? 1 : 0;
    int dy = (simulation_step % 2 == 0) ? 0 : 1;

    for (int i = height - 1; i >= 0; i--) {
        for (int j = width - 1; j >= 0; j--) {
            if (grid[i][j] == HUMAN) {
                int new_i = i + dy;
                int new_j = j + dx;
                if (new_i >= 0 && new_i < height && new_j >= 0 && new_j < width &&
                    temp_grid[new_i][new_j] == EMPTY && grid[new_i][new_j] == EMPTY) {
                    temp_grid[new_i][new_j] = HUMAN;
                    temp_grid[i][j] = EMPTY;
                } else {
                    temp_grid[i][j] = HUMAN;
                }
            } else if (i == height - 1 && j == width - 1 && grid[i][j] == HUMAN) {
                temp_grid[i][j] = EMPTY;
            }

            if (grid[i][j] == ZOMBIE && temp_grid[i][j] == EMPTY) {
                temp_grid[i][j] = ZOMBIE; // fixing the position of zombies
            }
        }
    }
}
void move_zombies(char** grid, char** temp_grid, int width, int height, int simulation_step) {
    int dx, dy;

    switch (simulation_step % 4) {
        case 0: // Right
            dx = 1; dy = 0;
            break;
        case 1: // Down
            dx = 0; dy = 1;
            break;
        case 2: // Left
            dx = -1; dy = 0;
            break;
        case 3: // Up
            dx = 0; dy = -1;
            break;
    }
    for (int i = 0; i < height; i++) {
    	for (int j = 0; j < width; j++) {
       		if (grid[i][j] == ZOMBIE) {
            		if (i + dy >= 0 && i + dy < height && j + dx >= 0 && j + dx < width &&
               			 grid[i + dy][j + dx] == EMPTY) {
                		temp_grid[i + dy][j + dx] = ZOMBIE;
               			 temp_grid[i][j] = EMPTY;
            }
        }
    }
}
}

// Function to check if there are any humans left in the grid
int check_humans_left(char** grid, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == HUMAN) {
                return 1;  // There are still humans
            }
        }
    }
    return 0;  // There is zero human
}

// Function to count the number of neighbors for humans and zombies
int count_neighbors(char** grid, int width, int height, int row, int col, char entity) {
    int count = 0;

    //checking right neighbor
    if (col + 1 < width && grid[row][col + 1] == entity) {
        count++;
    }
    //checking down neighbor
    if (row + 1 < height && grid[row + 1][col] == entity) {
        count++;
    }

    //checking left neighbor
    if (col - 1 >= 0 && grid[row][col - 1] == entity) {
        count++;
    }

    //checking up neighbor
    if (row - 1 >= 0 && grid[row - 1][col] == entity) {
        count++;
    }

    return count;
}

void apply_rules(char** grid, char** temp_grid, int width, int height, int *humans_survived, int *humans_infected, int *zombies_dead, int simulation_step) {
    // Implement rule logic here
    // Rule 2.1: If a zombie is surrounded by at least two people, it dies. Mark that cell as empty.
    // Rule 2.2: If a human is surrounded by at least one zombie, s/he turns into a zombie.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == ZOMBIE) {
                int count_people = count_neighbors(grid, width, height, i, j, HUMAN);
                if (count_people >= 2) {
                    temp_grid[i][j] = EMPTY;  // Zombie dies
                    grid[i][j] = EMPTY;

                 *zombies_dead += 1;
                }
        }
    }
}
    for (int i = 0; i < height; i++) {
    	for (int j = 0; j < width; j++) {
            if (grid[i][j] == HUMAN) {
            	int count_zombies = count_neighbors(grid, width, height, i, j, ZOMBIE);
		

            	if (count_zombies >= 1) {
                	temp_grid[i][j] = ZOMBIE;
			grid[i][j] = ZOMBIE;
                	*humans_infected += 1;
            }
        }
    }
}
int dx, dy;

    switch (simulation_step % 4) {
        case 0: // Right
            dx = 1; dy = 0;
            break;
        case 1: // Down
            dx = 0; dy = 1;
            break;
        case 2: // Left
            dx = -1; dy = 0;
            break;
        case 3: // Up
            dx = 0; dy = -1;
            break;
    }

  for (int i = 0; i < height; i++) {
    	for (int j = 0; j < width; j++) {
       		if (grid[i][j] == ZOMBIE) {
            		if (i + dy >= 0 && i + dy < height && j + dx >= 0 && j + dx < width &&
               			 grid[i + dy][j + dx] == EMPTY) {
				
				 if (i+dy-1 >= 0 && i+dy-1 < height && j+dx >= 0 && j+dx < width && temp_grid[i+dy-1][j+dx] == HUMAN){
				    temp_grid[i+dy-1][j+dx] = ZOMBIE;
				     }

				 if (i+dy >= 0 && i+dy < height && j+dx-1 >= 0 && j+dx-1 < width && temp_grid[i+dy][j+dx-1] == HUMAN){
				    temp_grid[i+dy][j+dx-1] = ZOMBIE;
				     }

				 if (i+dy+1 >= 0 && i+dy+1 < height && j+dx >= 0 && j+dx < width && temp_grid[i+dy+1][j+dx] == HUMAN){
				    temp_grid[i+dy+1][j+dx] = ZOMBIE;
				     }

				 if (i+dy >= 0 && i+dy < height && j+dx+1 >= 0 && j+dx+1 < width && temp_grid[i+dy][j+dx+1] == HUMAN){
				    temp_grid[i+dy][j+dx+1] = ZOMBIE;
				     }
                		            }
        }
    }
}
    
/*    for (int i = height - 1; i >= 0; i--) {
        for (int j = width - 1; j >= 0; j--) {
            if (grid[i][j] == HUMAN) {
		int dx = (simulation_step % 2 == 0) ? 1 : 0;
    		int dy = (simulation_step % 2 == 0) ? 0 : 1;
                int new_i = i + dy;
                int new_j = j + dx;
                if (new_i >= 0 && new_i < height && new_j >= 0 && new_j < width &&
                    temp_grid[new_i][new_j] == EMPTY && grid[new_i][new_j] == EMPTY) {
		if (new_i-1 >= 0 && new_i-1 < height && new_j >= 0 && new_j < width && temp_grid[new_i-1][new_j] == ZOMBIE){
		    temp_grid[new_i][new_j] = ZOMBIE;
		    
			(*humans_infected)++;
	        }

	        if (new_i >= 0 && new_i < height && new_j-1 >= 0 && new_j-1 < width && temp_grid[new_i][new_j-1] == ZOMBIE){
		    temp_grid[new_i][new_j] = ZOMBIE;
		    
			(*humans_infected)++;
		 }

	        if (new_i+1 >= 0 && new_i+1 < height && new_j >= 0 && new_j < width && temp_grid[new_i+1][new_j] == ZOMBIE){
		    temp_grid[new_i][new_j] = ZOMBIE;
	 	    
			(*humans_infected)++;
		 }

		if (new_i >= 0 && new_i < height && new_j+1 >= 0 && new_j+1 < width && temp_grid[new_i][new_j+1] == ZOMBIE){
		    temp_grid[new_i][new_j] = ZOMBIE;
		    
			(*humans_infected)++;
		}

                 }
    }
	}
    }


*/


    for (int i = 0; i < height; i++) {
       for (int j = 0; j < width; j++) {
            if (grid[i][j] == HUMAN) {
                if (i == height - 1 && j == width - 2 && (simulation_step % 2) != 1) {
                    (*humans_survived)++;
                    temp_grid[i][j] = EMPTY;
                    grid[i][j] = EMPTY;
                    temp_grid[i][j+1] = EMPTY;
                    grid[i][j+1] = EMPTY;
                }
   if (i == height - 2 && j == width - 1 && (simulation_step % 4) != 0) {
                    (*humans_survived)++;
                    temp_grid[i][j] = EMPTY;
                    grid[i][j] = EMPTY;
                    temp_grid[i+1][j] = EMPTY;
                    grid[i+1][j] = EMPTY;
                }
        }
     }
  }
}

void simulate(char** grid, int width, int height, int *humans_survived, int *humans_infected, int *zombies_dead) {
    int simulation_step = 0;
    char** temp_grid = create_temp_grid(width, height);

    while (check_humans_left(grid, width, height)) {
        /*for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                temp_grid[i][j] = EMPTY;
            }
        }*/

        // Application of functions
        apply_rules(grid, temp_grid, width, height, humans_survived, humans_infected, zombies_dead, simulation_step);

        move_humans(grid, temp_grid, width, height, simulation_step);
	
	apply_rules(grid, temp_grid, width, height, humans_survived, humans_infected, zombies_dead, simulation_step);
        move_zombies(grid, temp_grid, width, height, simulation_step);
	apply_rules(grid, temp_grid, width, height, humans_survived, humans_infected, zombies_dead, simulation_step);

	// Print the current state of the grid
        printf("Simulation Step: %d\n", simulation_step);
        print_grid(temp_grid, width, height);

        // Move temp_grid to the original grid
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                grid[i][j] = temp_grid[i][j];
            }
        }

        simulation_step++;
    }

    // Cleanup temporary grid
    cleanup_grid(temp_grid, height);
}


//Function to clean the grid
void cleanup_grid(char** grid, int height) {
    for (int i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);
}

                       
