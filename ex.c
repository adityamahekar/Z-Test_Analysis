#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATASET_SIZE 1000
#define MAX_VALUE 100000
#define NUM_TRIALS 100

// Function to compare integers for sorting
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Binary Search implementation
int binarySearch(int arr[], int low, int high, int key, int *comparisons) {
    while (low <= high) {
        (*comparisons)++;
        int mid = low + (high - low) / 2;
        if (arr[mid] == key)
            return mid;
        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Hash table structure for Hash Search
typedef struct Node {
    int key;
    struct Node *next;
} Node;

// Function to insert into the hash table
void insertHashTable(Node* hashTable[], int size, int key) {
    int index = key % size;
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Hash Search implementation
int hashSearch(Node* hashTable[], int size, int key, int *comparisons) {
    int index = key % size;
    Node *current = hashTable[index];
    while (current != NULL) {
        (*comparisons)++;
        if (current->key == key)
            return 1;
        current = current->next;
    }
    return -1;
}

// Free memory allocated for hash table
void freeHashTable(Node* hashTable[], int size) {
    for (int i = 0; i < size; i++) {
        Node *current = hashTable[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
}

// Main function
int main() {
    int arr[DATASET_SIZE];
    Node* hashTable[DATASET_SIZE];

    // Open file to write results
    FILE *file = fopen("search_comparisons.csv", "w");
    if (!file) {
        printf("Error: Could not create output file.\n");
        return 1;
    }

    fprintf(file, "BinarySearchComparisons,HashSearchComparisons\n");

    // Initialize random seed
    srand(time(NULL));

    // Run trials
    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        // Initialize hash table
        for (int i = 0; i < DATASET_SIZE; i++) {
            hashTable[i] = NULL;
            arr[i] = (rand() % MAX_VALUE) + 1;
            insertHashTable(hashTable, DATASET_SIZE, arr[i]);
        }

        // Sort array for Binary Search
        qsort(arr, DATASET_SIZE, sizeof(int), compare);

        // Select random key
        int key = arr[rand() % DATASET_SIZE];

        // Measure Binary Search
        int binaryComparisons = 0;
        binarySearch(arr, 0, DATASET_SIZE - 1, key, &binaryComparisons);

        // Measure Hash Search
        int hashComparisons = 0;
        hashSearch(hashTable, DATASET_SIZE, key, &hashComparisons);

        // Write results to file
        fprintf(file, "%d,%d\n", binaryComparisons, hashComparisons);

        // Free memory allocated for hash table
        freeHashTable(hashTable, DATASET_SIZE);
    }

    fclose(file);
    printf("Search comparisons written to 'search_comparisons.csv'.\n");
    return 0;
}