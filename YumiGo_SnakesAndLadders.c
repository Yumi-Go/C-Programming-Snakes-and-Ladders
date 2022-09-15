#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

// board definition by linked list
typedef int position;
struct Square
{
    position data;
    struct Square* link;
};

// current position
struct Square* top;

// trace position
void push(position data)
{
    // create new square position and allocate memory
    struct Square* temp;
    temp = (struct Square*)malloc(sizeof(struct Square));
    // initialise data into temp data
    temp->data = data;
    // put top pointer into temp link
    temp->link = top;
    // make temp as top
    top = temp;
}

// check the stack is empty
int isEmpty()
{
    return top == NULL;
}

// return the latest position from the stack
position positioning()
{
    // check for empty stack
    if (!isEmpty())
        return top->data;
    else
        exit(1);
}

// delete top element when the position is a head of a snake
position delete_top()
{
    struct Square* temp;
    
    // check for stack underflow
    if (isEmpty())
    {
        exit(1);
    }
    else
    {
        position data = top->data;
        // top assign into temp
        temp = top;
        // assign second position to top
        top = top->link;
        // destroy connection between first and second
        temp->link = NULL;
        // release memory of top node
        free(temp);
        return data;
    }
}

int random_dice() {
    int dice;
    dice = 1 + rand()%6;
    return dice;
}

void main() {
    srand((unsigned int)time(NULL));
    int num_snakes, num_ladders;
    int board_size;
    board_size = 32 + rand()%33;
    
    // open the file
    FILE* file = fopen("GameReport.txt", "w");
    if (file == NULL) {
		printf("Fail to open file\n");
	}

    fprintf(file, "%s\n\n", "Snakes and Ladders by Yumi Go");

    // user-defined the number of snakes and ladders
    while(1) {
        printf("Board Size: %d\n", board_size);
        printf("Set the number of snakes: ");
        scanf("%d", &num_snakes);

        printf("Set the number of ladders: ");
        scanf("%d", &num_ladders);
        
        if (2 * (num_snakes + num_ladders) <= board_size) {
            fprintf(file, "Board Size: %d\n", board_size);
            fprintf(file, "Number of Snakes: %d\n\n", num_snakes);
            fprintf(file, "Number of Ladders: %d\n\n", num_ladders);
            break;
        }
        else if (num_snakes < 1) {
            printf("The number of snakes must be more than 1!\n");
        }
        else if (num_ladders < 1) {
            printf("The number of ladders must be more than 1!\n");
        }
        else {
            printf("It's over the limit!\n");
        }
    }

    // create random move by snakes and ladders
    int move_squares = num_snakes + num_ladders;
    int num[move_squares*2];
    int i, j;
    int move;
    
    for (i=0; i<move_squares; i++) {
        num[i] = 2 + rand()%board_size-2;
        for (j=0; j<i-1; j++) {
            if (num[i] == num[j]) {
                i--;
                break;
            }
        }
    }

    // between 1 and 10 squares back
    for (i=0; i<num_snakes; i++) {
        while(1) {
            int dup_check = 0;
            move = 1 + rand()%10;
            for (j=0; j<move_squares+i; j++) {
                if(num[j] == num[i] - move) {
                    break;
                }
                else {
                    dup_check++;
                }
            }
            if (num[i] - move > 0 && dup_check == move_squares + i) {
                break;
            }
        }
        num[move_squares+i] = num[i] - move;
    }

    // between 1 and 10 squares forward
    for (i=num_snakes; i<move_squares; i++) {
        while(1) {
            int dup_check = 0;
            move = 1 + rand()%10;
            for (j=0; j<move_squares+i; j++) {
                if(num[j] == num[i] + move) {
                    break;
                }
                else {
                    dup_check++;
                }
            }
            if (num[i] + move <= board_size && dup_check == move_squares + i) {
                break;
            }
        }
        num[move_squares+i] = num[i] + move;
    }

    // placed snakes and ladders
    for (i=0; i<num_snakes; i++) {
        printf("Snake%d: %d to %d\n", i+1, num[i], num[move_squares+i]);
        fprintf(file, "Snake%d: %d to %d\n", i+1, num[i], num[move_squares+i]);
    }
    for (i=num_snakes; i<move_squares; i++) {
        printf("Ladder%d: %d to %d\n", i+1-num_snakes, num[i], num[move_squares+i]);
        fprintf(file, "Ladder%d: %d to %d\n", i+1-num_snakes, num[i], num[move_squares+i]);
    }

    fprintf(file, "\n");

    // game play
    int single_move;
    int current_position = 0;
    while(1) {
        // random move from dice
        single_move = random_dice();
        printf("Die: %d\n", single_move);
        fprintf(file, "Die: %d\n", single_move);
        current_position += single_move;
        push(current_position);
        printf("Moved Position: %d\n", positioning());
        fprintf(file, "Moved Position: %d\n\n", positioning());
        // check snake head or ladder foot
        for (i=0; i<num_snakes; i++) {
            if (current_position == num[i]) {
                printf("Meet the head of snake%d\n", i+1);
                fprintf(file, "Meet the head of snake%d\n", i+1);
                current_position = num[move_squares+i];
                delete_top();
                push(current_position);
                printf("Moved Position: %d\n", positioning());
                fprintf(file, "Moved Position: %d\n\n", positioning());
            }
        }
        for (i=num_snakes; i<move_squares; i++) {
            if (current_position == num[i]) {
                printf("Meet the foot of ladder%d\n", i+1-num_snakes);
                fprintf(file, "Meet the foot of ladder%d\n", i+1-num_snakes);
                current_position = num[move_squares+i];
                push(current_position);
                printf("Moved Position: %d\n", positioning());
                fprintf(file, "Moved Position: %d\n\n", positioning());
            }
        }
        // check the game is done
        if (positioning() > board_size) {
            printf("The Board Size is %d\n", board_size);
            fprintf(file, "The Board Size is %d\n", board_size);
            printf("The Game is finished!");
            fprintf(file, "The Game is finished!");
            // close the file
            fclose(file);
            break;
        }
    }
}