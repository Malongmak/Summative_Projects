#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task {
    int id;
    char description[256];
    int completed; 
    struct Task *next;
} Task;

Task *head = NULL;
int taskCount = 0;

// function prototypes
void addTask(const char *description);
void removeTask(int id);
void updateTask(int id, const char *newDescription);
void displayTasks();
void markTaskCompleted(int id);
void freeTasks();

int main() {
    int choice, id;
    char description[256];

    while (1) {
        printf("\n1. Add Task\n2. Remove Task\n3. Update Task\n4. Display Tasks\n5. Mark Task Completed\n6. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter task description: ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0;
                addTask(description);
                break;
            case 2:
                printf("Enter task ID to remove: ");
                scanf("%d", &id);
                removeTask(id);
                break;
            case 3:
                printf("Enter task ID to update: ");
                scanf("%d", &id);
                printf("Enter new description: ");
                getchar();
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0;
                updateTask(id, description);
                break;
            case 4:
                displayTasks();
                break;
            case 5:
                printf("Enter task ID to mark as completed: ");
                scanf("%d", &id);
                markTaskCompleted(id);
                break;
            case 6:
                freeTasks();
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

// function to add a task
void addTask(const char *description) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->id = ++taskCount; 
    strcpy(newTask->description, description);
    newTask->completed = 0; 
    newTask->next = head; 
    head = newTask;
    printf("Task added with ID: %d\n", newTask->id);
}

// function to remove a task
void removeTask(int id) {
    Task *temp = head;
    Task *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Task with ID %d not found.\n", id);
        return;
    }

    if (prev == NULL) {
        head = temp->next; 
    } else {
        prev->next = temp->next; 
    }

    free(temp);
    printf("Task with ID %d removed.\n", id);
}

// function to update a task
void updateTask(int id, const char *newDescription) {
    Task *temp = head;

    while (temp != NULL) {
        if (temp->id == id) {
            strcpy(temp->description, newDescription);
            printf("Task with ID %d updated.\n", id);
            return;
        }
        temp = temp->next;
    }

    printf("Task with ID %d not found.\n", id);
}

// function to display all tasks
void displayTasks() {
    Task *temp = head;
    if (temp == NULL) {
        printf("No tasks available.\n");
        return;
    }

    printf("\nTo-Do List:\n");
    while (temp != NULL) {
        printf("ID: %d, Description: %s, Status: %s\n", temp->id, temp->description, temp->completed ? "Completed" : "Not Completed");
        temp = temp->next;
    }
}

// function to mark a task as completed
void markTaskCompleted(int id) {
    Task *temp = head;

    while (temp != NULL) {
        if (temp->id == id) {
            temp->completed = 1; 
            printf("Task with ID %d marked as completed.\n", id);
            return;
        }
        temp = temp->next;
    }

    printf("Task with ID %d not found.\n", id);
}

// function to free all tasks
void freeTasks() {
    Task *temp = head;
    while (temp != NULL) {
        Task *next = temp->next;
        free(temp);
        temp = next;
    }
    head = NULL;
}
