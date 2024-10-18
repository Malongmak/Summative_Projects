#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    int id;
    char name[100];
    float grades[10];
    struct Student *left, *right, *parent;
    int color; // 0 for Red, 1 for Black
} Student;

Student *root = NULL;
Student *TNULL;

// functions prototypes
void initialize();
Student *createStudent(int id, char *name, float *grades);
void insertStudent(int id, char *name, float *grades);
void deleteStudent(int id);
void updateStudent(int id, char *name, float *grades);
Student *searchStudent(int id);
void inorderTraversal(Student *node);
void fixInsert(Student *k);
void rbTransplant(Student *u, Student *v);
void fixDelete(Student *x);
Student *minimum(Student *node);
void freeTree(Student *node);

// initialize the Red-Black Tree
void initialize() {
    TNULL = (Student *)malloc(sizeof(Student));
    TNULL->color = 1; // Black
    TNULL->left = NULL;
    TNULL->right = NULL;
    root = TNULL;
}

// create a new student node
Student *createStudent(int id, char *name, float *grades) {
    Student *newStudent = (Student *)malloc(sizeof(Student));
    newStudent->id = id;
    strcpy(newStudent->name, name);
    for (int i = 0; i < 10; i++) {
        newStudent->grades[i] = grades[i];
    }
    newStudent->left = newStudent->right = newStudent->parent = TNULL;
    newStudent->color = 0; // Red
    return newStudent;
}

// intert a new student into the Red-Black Tree
void insertStudent(int id, char *name, float *grades) {
    Student *newStudent = createStudent(id, name, grades);
    Student *y = NULL;
    Student *x = root;

    while (x != TNULL) {
        y = x;
        if (newStudent->id < x->id) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    newStudent->parent = y;
    if (y == NULL) {
        root = newStudent; // Tree was empty
    } else if (newStudent->id < y->id) {
        y->left = newStudent;
    } else {
        y->right = newStudent;
    }

    if (newStudent->parent == NULL) {
        newStudent->color = 1; // Black
        return;
    }

    fixInsert(newStudent);
}

// fix the tree after insertion
void fixInsert(Student *k) {
    Student *u;
    while (k->parent->color == 0) {
        if (k->parent == k->parent->parent->left) {
            u = k->parent->parent->right;
            if (u->color == 0) { // Case 1
                k->parent->color = 1;
                u->color = 1;
                k->parent->parent->color = 0;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) { // Case 2
                    k = k->parent;
                    // Left rotate
                    Student *temp = k->left;
                    k->left = temp->right;
                    if (temp->right != TNULL) temp->right->parent = k;
                    temp->parent = k->parent;
                    if (k->parent == NULL) {
                        root = temp;
                    } else if (k == k->parent->left) {
                        k->parent->left = temp;
                    } else {
                        k->parent->right = temp;
                    }
                    temp->right = k;
                    k->parent = temp;
                }
                // case 3
                k->parent->color = 1;
                k->parent->parent->color = 0;
                // right rotate
                Student *temp = k->parent->parent;
                k->parent->parent = k->parent->left;
                k->parent->left = temp;
            }
        } else {
            u = k->parent->parent->left;
            if (u->color == 0) { // Case 1
                k->parent->color = 1;
                u->color = 1;
                k->parent->parent->color = 0;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) { // Case 2
                    k = k->parent;
                    // right rotate
                    Student *temp = k->right;
                    k->right = temp->left;
                    if (temp->left != TNULL) temp->left->parent = k;
                    temp->parent = k->parent;
                    if (k->parent == NULL) {
                        root = temp;
                    } else if (k == k->parent->right) {
                        k->parent->right = temp;
                    } else {
                        k->parent->left = temp;
                    }
                    temp->left = k;
                    k->parent = temp;
                }
                // Case 3
                k->parent->color = 1;
                k->parent->parent->color = 0;
                // left rotate
                Student *temp = k->parent->parent;
                k->parent->parent = k->parent->right;
                k->parent->right = temp;
            }
        }
    }
    root->color = 1;
}

// search a student by id
Student *searchStudent(int id) {
    Student *current = root;
    while (current != TNULL) {
        if (id == current->id) {
            return current;
        } else if (id < current->id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

// update a student by id
void updateStudent(int id, char *name, float *grades) {
    Student *student = searchStudent(id);
    if (student != NULL) {
        strcpy(student->name, name);
        for (int i = 0; i < 10; i++) {
            student->grades[i] = grades[i];
        }
        printf("Student record updated successfully.\n");
    } else {
        printf("Student not found.\n");
    }
}

// in-order traversal of the Red-Black Tree
void inorderTraversal(Student *node) {
    if (node != TNULL) {
        inorderTraversal(node->left);
        printf("ID: %d, Name: %s\n", node->id, node->name);
        inorderTraversal(node->right);
    }
}

// delete a student by id
void deleteStudent(int id) {
    Student *student = searchStudent(id);
    if (student == NULL) {
        printf("Student not found.\n");
        return;
    }

    Student *y = student;
    Student *x;
    int yOriginalColor = y->color;

    if (student->left == TNULL) {
        x = student->right;
        rbTransplant(student, student->right);
    } else if (student->right == TNULL) {
        x = student->left;
        rbTransplant(student, student->left);
    } else {
        y = minimum(student->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == student) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = student->right;
            y->right->parent = y;
        }
        rbTransplant(student, y);
        y->left = student->left;
        y->left->parent = y;
        y->color = student->color;
    }
    free(student);
    if (yOriginalColor == 1) {
        fixDelete(x);
    }
}

// transplant nodes during deletion
void rbTransplant(Student *u, Student *v) {
    if (u->parent == NULL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// fix the tree after deletion of a node
void fixDelete(Student *x) {
    Student *s;
    while (x != root && x->color == 1) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == 0) {
                s->color = 1;
                x->parent->color = 0;
                // left rotate
                Student *temp = x->parent;
                x->parent = temp->right;
                temp->right = s;
            }
            if (s->left->color == 1 && s->right->color == 1) {
                s->color = 0;
                x = x->parent;
            } else {
                if (s->right->color == 1) {
                    s->left->color = 1;
                    s->color = 0;
                    // right rotate
                    Student *temp = s->left;
                    s->left = temp->right;
                    temp->parent = s->parent;
                    s->parent = temp;
                }
                s->color = x->parent->color;
                x->parent->color = 1;
                s->right->color = 1;
                // left rotate
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == 0) {
                s->color = 1;
                x->parent->color = 0;
                // right rotate
                Student *temp = x->parent;
                x->parent = temp->left;
                temp->left = s;
            }
            if (s->right->color == 1 && s->left->color == 1) {
                s->color = 0;
                x = x->parent;
            } else {
                if (s->left->color == 1) {
                    s->right->color = 1;
                    s->color = 0;
                    // left rotate
                    Student *temp = s->right;
                    s->right = temp->left;
                    temp->parent = s->parent;
                    s->parent = temp;
                }
                s->color = x->parent->color;
                x->parent->color = 1;
                s->left->color = 1;
                x = root;
            }
        }
    }
    x->color = 1;
}

// function to find the minimum value in the Red-Black Tree
Student *minimum(Student *node) {
    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

// free the memory allocated for the Red-Black Tree
void freeTree(Student *node) {
    if (node != TNULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

int main() {
    initialize();
    int choice, id;
    char name[100];
    float grades[10];

    while (1) {
        printf("\n1. Insert Student\n2. Delete Student\n3. Update Student\n4. Search Student\n5. Display All Students\n6. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; 
                printf("Enter Grades (10 values): ");
                for (int i = 0; i < 10; i++) {
                    scanf("%f", &grades[i]);
                }
                insertStudent(id, name, grades);
                break;
            case 2:
                printf("Enter ID to delete: ");
                scanf("%d", &id);
                deleteStudent(id);
                break;
            case 3:
                printf("Enter ID to update: ");
                scanf("%d", &id);
                getchar();
                printf("Enter New Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; 
                printf("Enter New Grades (10 values): ");
                for (int i = 0; i < 10; i++) {
                    scanf("%f", &grades[i]);
                }
                updateStudent(id, name, grades);
                break;
            case 4:
                printf("Enter ID to search: ");
                scanf("%d", &id);
                Student *found = searchStudent(id);
                if (found != NULL) {
                    printf("Found Student - ID: %d, Name: %s\n", found->id, found->name);
                } else {
                    printf("Student not found.\n");
                }
                break;
            case 5:
                printf("Students in ascending order:\n");
                inorderTraversal(root);
                break;
            case 6:
                freeTree(root);
                free(TNULL);
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
