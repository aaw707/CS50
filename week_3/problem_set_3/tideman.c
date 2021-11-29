#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
bool current_cycle;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    //print preferences
    /*
    printf("Printing preferences matrix...\n");
    for (int m = 0; m < candidate_count; m++)
    {
        for (int n = 0; n < candidate_count; n++)
        {
            printf("%i ", preferences[m][n]);
        }
        printf("\n");
    }
    */
    add_pairs();

    // print pairs
    /*
    printf("Printing pairs array...\n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("Winner: %i, Loser: %i\n", pairs[i].winner, pairs[i].loser);
    }
    */
    sort_pairs();

    // print pairs after sorting
    /*
    printf("Printing sorted pairs array...\n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("Winner: %i, Loser: %i\n", pairs[i].winner, pairs[i].loser);
    }
    */

    lock_pairs();

    // print locked

    printf("Printing locked graph...\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // loop through the candidates to find the one match to name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // loop through each pair of candidates in ranks in order
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            // else if equal, the pair will not be added to pairs (according to the specification of the problem set)
            // check50 wants both pairs instead
            /*
            else
            {
                // add both pairs
                pairs[pair_count].winner = i;
                pairs[pair_count].winner = j;

                pairs[pair_count + 1].winner = j;
                pairs[pair_count + 1].winner = i;

                pair_count += 2;
            }
            */
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    pair temp;
    int victory_i;
    int victory_j;
    int max_victory;
    int max_index;

    // loop through each pair
    for (int i = 0; i < pair_count - 1; i++)
    {

        victory_i = preferences[pairs[i].winner][pairs[i].loser];
        max_victory = victory_i;
        max_index = i;

        for (int j = i + 1; j < pair_count; j++)
        {
            victory_j = preferences[pairs[j].winner][pairs[j].loser];
            if (victory_j > victory_i)
            {
                max_victory = victory_j;
                max_index = j;
            }
        }

        if (max_index != i)
        {
            temp = pairs[max_index];
            pairs[max_index] = pairs[i];
            pairs[i] = temp;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // loop through the pairs to see whether to add it to the locked graph
    for (int i = 0; i < pair_count; i++)
    {
        if (!check_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // loop through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // all candidates are potential winners until proven they have lost to another candidate
        bool potential_winner = true;
        // loop through other candidates against the current one
        for (int j = 0; j < candidate_count; j++)
        {
            // if other candidate has won over this one, this candidate is no longer the potential winner
            if (locked[j][i])
            {
                potential_winner = false;
                break;
            }
        }
        // if the candidate is no longer potential winner, go to evaluate the next candidate
        if (potential_winner == false)
        {
            continue;
        }
        else
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

bool check_cycle(int winner, int loser)
{
    // if adding this pair to the locked graph will create a cycle, return true
    // else, return false

    // basically checking if the arrow goes from loser to winner

    // base case: already an arrow from loser to winner in locked graph
    // this doesn't happen since we don't create a pair with two tied candidates
    // but this case would happen in the recursive call
    if (locked[loser][winner])
    {
        //printf("Already an arrow from loser %i to winner %i, return true.\n", loser, winner);
        return true;
    }

    // loop through candidates to see if the loser has any secondary loser
    //printf("Looping through candidates to see if the loser %i has any secondary loser...\n", loser);
    for (int i = 0; i < candidate_count; i++)
    {
        //printf("Checking loser %i over candidate %i...\n", loser, i);
        // if yes, check if there's arrow from this secondary loser to the original winner
        if (locked[loser][i])
        {
            // use a recursive call to check
            current_cycle = check_cycle(winner, i);

            // if no cycle, continue to the next secondary loser
            if (current_cycle == false)
            {
                continue;
            }
            // if yes, return true and end the function
            else
            {
                return current_cycle;
            }
        }
    }
    // if this loser doesn't have any secondary loser
    //printf("This loser %i doesn't have any secondary loser. return false.\n", loser);
    return false;
}