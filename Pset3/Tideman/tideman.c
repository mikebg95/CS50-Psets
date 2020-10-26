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

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

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

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // check to see if name matches with any candidate
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // set rank to that candidate
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // for each candidate except last one
    for (int i = 0, n = candidate_count - 1; i < n; i++)
    {
        // for each candidate coming after first candidate
        for (int j = i+1, m = candidate_count; j < m; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // get correct pair count
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        for (int j = 0, m = candidate_count; j < m; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // sort pairs through bubble sort
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            // get strength of first pair win
            int pref_winner1 = preferences[pairs[j].winner][pairs[j].loser];
            int pref_loser1 = preferences[pairs[j].loser][pairs[j].winner];
            int strength1 = pref_winner1 - pref_loser1;

            // get strength of second pair win
            int pref_winner2 = preferences[pairs[j + 1].winner][pairs[j + 1].loser];
            int pref_loser2 = preferences[pairs[j + 1].loser][pairs[j + 1].winner];
            int strength2 = pref_winner2 - pref_loser2;

            // bubble strongest pair win up to front
            if (strength1 < strength2)
            {
                pair temp = pairs[j + 1];
                pairs [j + 1] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO!!!

    for (int i = 0; i < pair_count - 1; i++)
    {
        int winner1 = pairs[i].winner;
        int loser1 = pairs[i].loser;
        int strength1 = preferences[winner1][loser1] - preferences[loser1][winner1];

        int winner2 = pairs[i + 1].winner;
        int loser2 = pairs[i + 1].loser;
        int strength2 = preferences[winner2][loser2] - preferences[loser2][winner2];

        if (strength1 > strength2)
        {
            locked[winner1][loser1] = true;
        }
        else {
            locked[winner1][loser1] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO!!!

    int winner = pairs[0].winner;
    printf("\n\nWinner is: %s\n\n", candidates[winner]);
    return;
}

