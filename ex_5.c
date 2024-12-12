#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex_5.h"
/**
    * David goldstein
    * 331010835
    * ex_5
    **/
void main() {
    menu();
}
void menu() {

    Worker* workers[MAX_WORKERS];
    Project* projects[MAX_PROJECTS];
    int selection = 0;
    //workerCount,projectCount start from 0 becuase there no worker and project in the start
    int workerCount = 0;
    int projectCount = 0;
    do {
        printf("Welcome!\n"
            "Please select the desired option:\n"
            "1. Join the company\n"
            "2. Open a new project\n"
            "3. Join an existing project\n"
            "4. See what projects are in work\n"
            "5. See which people are in what projects\n"
            "6. Work on existing project\n"
            "7. Leave the company\n"
            "8. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &selection);
        switch (selection) {
        case ADD_WORKER: {
            joinCompany(workers, &workerCount);
            break;
        }
        case NEW_PROJECT: {
            openNewProject(workerCount, workers, projects, &projectCount);
            break;
        }
        case JOIN: {
            joinExistingProject(workers, workerCount, projects, projectCount);
            break;
        }
        case ALL_PROJECT: {
            printTheProjects(projects, projectCount);
            break;
        }
        case WHERE_WORK: {
            displayWorkers(workers, workerCount);
            break;
        }
        case WORKING_PROJECT: {
            addAndRemoveFeature(workers, workerCount, projects, projectCount);
            break;
        }
        case LEAV: {
            leaveCompany(workers, &workerCount, projects, &projectCount);
            break;
        }
        case EXIT: {
            freeAllAllocations(workers, workerCount, projects, projectCount);
            printf("Exiting...\n");
            return;
        }
        default:
        {
            printf("\nInvalid choice. Please try again.\n");
            break;
        }
        }
    } while (1);
}
char* getChar(void) {
    int lenghtOfS = 1;
    char* string = (char*)malloc(lenghtOfS * sizeof(char));

    if (string == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    //the index we work
    int index = 0;
    char charNow;
    scanf(" %c", &charNow);

    //the loop is until the user finish to put the name
    while (charNow != '\n' && charNow != '\r') {
        string[index] = charNow;
        if (++index == lenghtOfS) {
            lenghtOfS++;
            char* temp = (char*)realloc(string, lenghtOfS * sizeof(char));
            if (temp == NULL) {
                printf("Memory reallocation failed.\n");
                return string;
            }
            string = temp;
        }
        scanf("%c", &charNow);
    }

    // finish the string array
    string[index] = '\0';

    return string;
}

void joinCompany(Worker* workers[], int* workerCount) {
    char* string;
    //check if the worker is the only one with this name
    int check = 1;
    printf("Enter the name of the new worker: ");
    string = getChar();
    for (int i = 0; i < *workerCount; ++i) {
        if (strcmp(string, workers[i]->name) == 0) {
            printf("A worker with this name already exists\n"
                "There is not enough space in this company for both of us\n");
            check = 0;
            free(string);
            break;
        }
    }
    if (check) {
        Worker* worker = (Worker*)malloc(sizeof(Worker));
        Project** p = (Project**)malloc(sizeof(0));
        //check the mallocs fail
        if (worker == NULL || p == NULL) {
            exit(1);
        }
        worker->name = string;
        worker->projects = p;
        worker->projectCount = 0;
        workers[*workerCount] = worker;
        (*workerCount)++;
    }
}
void openNewProject(int workerCount, Worker* workers[], Project* projects[], int* projectCount) {
    //check if there worker
    if (workerCount > 0) {
        int choiceW;
        Worker** workerArray = (Worker**)malloc(MAX_WORKERS_IN_PROJECT * sizeof(Worker));
        if (workerArray == NULL)
        {
            exit(1);
        }
        char* projectName;
        //chekc if the project with this name already exists
        int check = 1;
        printf("Who are you? Choose a number:\n");
        showTheWorkers(workerCount, workers);
        printf("Enter your choice: ");
        scanf(" %d", &choiceW);
        if (choiceW < 1 || choiceW > workerCount)
        {
            printf("Invalid worker selection.\n");
        }
        else {
            choiceW--;
            printf("Enter the name of the new project: ");
            projectName = getChar();
            for (int i = 0; i < *projectCount; ++i) {
                if (strcmp(projects[i]->name, projectName) == 0)
                {
                    printf("A project with this name already exists.\n"
                        "Better be more creative\n");
                    check = 0;
                }
            }
            if (check) {
                //put the worker in the worker array in this project in the start(0)
                workerArray[0] = workers[choiceW];
                Project* project = (Project*)malloc(sizeof(Project));
                if (project == NULL) {
                    free(projectName);
                    free(workerArray);
                    exit(1);
                }
                project->name = projectName;
                project->workers = workerArray;
                project->workerCount = 1;
                projects[*projectCount] = project;
                Project** help = (Project*)realloc(workers[choiceW]->projects, (workers[choiceW]->projectCount + 1) * sizeof(Project));
                if (help == NULL) {
                    free(projectName);
                    free(workerArray);
                    free(project);
                    exit(1);
                }
                workers[choiceW]->projects = help;
                project->features = NULL;
                // Add the new project to the chosen worker's projects array
                workers[choiceW]->projects[workers[choiceW]->projectCount] = project;
                workers[choiceW]->projectCount++;
                (*projectCount)++;
            }
        }

    }
    else
        printf("There are no workers in the company yet, please join the company first.\n");
}


void joinExistingProject(Worker* workers[], int workerCount, Project* projects[], int projectCount) {
    //check if there are worker and project
    if (workerCount != 0 && projectCount != 0) {
        int choiceWorker, choiceProject = 1;
        printf("Select a worker to join a project:\n");
        showTheWorkers(workerCount, workers);
        printf("Enter the worker's number: ");
        scanf(" %d", &choiceWorker);
        printf("Select a project to join:\n");
        printProject(projectCount, projects);
        printf("Enter the project's number: ");
        scanf(" %d", &choiceProject);
        choiceWorker--;
        choiceProject--;
        //if the choos is not good
        if (choiceWorker < 0 || choiceWorker >= workerCount)
        {
            printf("\nInvalid worker selection.\n");

        }
        else if (choiceProject < 0 || choiceProject >= projectCount) {
            printf("\nInvalid project selection.\n");
        }
        //check if there a place for another worker
        else if (projects[choiceProject]->workerCount >= MAX_WORKERS_IN_PROJECT)
        {
            printf("Project is full.\n");
        }
        else if (!checkWorker(workers[choiceWorker], projects[choiceProject]))
        {
            workers[choiceWorker]->projects[workers[choiceWorker]->projectCount] = projects[choiceProject];
            workers[choiceWorker]->projectCount++;
            projects[choiceProject]->workers[projects[choiceProject]->workerCount] = workers[choiceWorker];
            projects[choiceProject]->workerCount++;
        }
    }

    else if (workerCount == 0)
    {
        printf("There are no workers in the company yet, please join the company first.\n");
    }
    else if (projectCount == 0) {
        printf("There are no projects in the company yet, please create one first.\n");
    }
}

int checkWorker(Worker* worker, Project* project) {
    for (int i = 0; i < project->workerCount; ++i) {
        if (worker == project->workers[i])
        {
            printf("This worker is already part of the project '%s'.\n", project->name);
            return 1;
        }
    }
    return 0;
}

void printTheProjects(Project* projects[], int projectCount) {
    //check if there project
    if (projectCount == 0)
    {
        printf("There are no projects to display.\n");
    }
    else {
        //print project and workers
        for (int i = 0; i < projectCount; ++i) {
            printf("Project: %s\n", projects[i]->name);
            printf("Workers in this project:\n");
            for (int j = 0; j < projects[i]->workerCount; ++j) {
                printf(" - %s\n", projects[i]->workers[j]->name);
            }
            printf("Features of the project:\n");
            //check if there features in the project
            if (projects[i]->features == NULL) {
                printf(" No features\n");
            }
            //take the features frome project 
            FeatureNode* featureP = projects[i]->features;
            //until the featureP is null,when we finish all the feature
            while (featureP != NULL) {
                printf(" - %s\n", featureP->feature);
                featureP = featureP->next;
            }
            printf("\n");
        }
    }
}

void displayWorkers(Worker* workers[], int workerCount) {
    //check if there worker
    if (workerCount == 0) {
        printf("There are no workers to display.\n");
    }
    else {
        //print the workers
        for (int i = 0; i < workerCount; ++i) {
            printf("Worker: %s\n", workers[i]->name);
            if (workers[i]->projectCount == 0) {
                printf(" Not involves in any projects.\n\n");
            }
            else {
                printf(" Projects this worker is involved in:\n");
                for (int j = 0; j < workers[i]->projectCount; ++j) {
                    printf("  - %s\n", workers[i]->projects[j]->name);
                }
                printf("\n");
            }
        }
    }
}

void addAndRemoveFeature(Worker* workers[], int workerCount, Project* projects[], int projectCount) {
    //check if there woeker and project to work on
    if (workerCount > 0 && projectCount > 0) {
        int workerChoice;
        printf("Select a worker:\n");
        showTheWorkers(workerCount, workers);
        printf("Enter the worker's number: ");
        scanf(" %d", &workerChoice);
        workerChoice--;
        //check the input from the user
        if (workerChoice < 0 || workerChoice >= workerCount)
        {
            printf("\nInvalid worker selection.\n");

        }
        else if (workers[workerChoice]->projectCount == 0)
        {
            printf("This worker is not involved in any projects.\n");
        }
        else {
            int projectChoice;
            printf("Select a project:\n");
            //print project
            for (int i = 0; i < projectCount; ++i) {
                printf("%d. %s\n", i + 1, projects[i]->name, projects[i]->workerCount);
            }
            printf("Enter the project number: ");
            scanf(" %d", &projectChoice);
            projectChoice--;
            if (projectChoice < 0 || projectChoice >= projectCount)
            {
                printf("\nInvalid project selection\n");
            }
            else {
                char* checkAddOrRemoveChoice;
                printf("Do you want to add or remove a feature? (add/remove): ");
                //take from the user the input getChar
                checkAddOrRemoveChoice = getChar();
                //check if it add
                if (strcmp(checkAddOrRemoveChoice, "add") == 0)
                {
                    printf("Enter the new feature: ");
                    addFeatureToProject(workers[workerChoice]->projects[projectChoice]);

                }
                //check if its remove
                else if (strcmp(checkAddOrRemoveChoice, "remove") == 0) {
                    if (workers[workerChoice]->projects[projectChoice]->features == NULL)
                    {
                        printf("There are no features to remove.\n");
                    }
                    else {
                        printf("Select a feature to remove:\n");
                        printFeatureNode(projects[projectChoice]->features);
                        printf("Enter your choice: ");
                        removeFeatureFromProject(workers[workerChoice]->projects[projectChoice]);
                    }
                }
                //if the input isnt add or remove the chois is not good
                else
                {
                    printf("\nInvalid choice\n");
                }
                free(checkAddOrRemoveChoice);
            }
        }
    }
    else if (workerCount == 0)
    {
        printf("There are no workers in the company yet, please join the company first.\n");
    }
    else if (projectCount == 0)
    {
        printf("There are no projects in the company yet, please create one first.");
    }
}


int addFeatureToProject(Project* project) {
    //take input from the user
    char* feature = getChar();
    FeatureNode* featureOnP = project->features, * newNode = (FeatureNode*)malloc(sizeof(FeatureNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    //take the feature
    newNode->feature = feature;
    newNode->next = NULL;
    if (featureOnP == NULL) {
        project->features = newNode;
    }
    else {
        //find the lest feature
        while (featureOnP->next != NULL) {
            if (strcmp(feature, featureOnP->feature) == 0)
            {
                printf("A feature with this name already exists.\n"
                    "Better be more creative\n");
                return 1;
            }
            //countiue to the next feature
            featureOnP = featureOnP->next;
        }
        //when we find we put the new feature
        featureOnP->next = newNode;
    }

    return 0;
}


int removeFeatureFromProject(Project* project) {
    int choice;
    //i is find the index that the user want to remove
    int i = 0;
    scanf(" %d", &choice);
    //check the input
    if (choice < 1)
    {
        printf("\nInvalid choice\n");
        return 1;
    }
    choice--;
    FeatureNode* featureOnP = project->features, * featureOnPBefore = featureOnP;
    //while we find the choice of the user or we finish the all feature
    while (!(featureOnP == NULL || i == choice)) {
        featureOnPBefore = featureOnP;
        featureOnP = featureOnP->next;
        i++;
        //check if the input is too bigger
        if (i < choice && featureOnP == NULL)
        {
            printf("\nInvalid choice\n");
            return 1;
        }
    }
    if (featureOnPBefore == featureOnP) {
        if (featureOnP != NULL)
        {
            //remove the first one
            project->features = featureOnP->next;
        }
    }
    else {
        if (featureOnP != NULL)
        {
            featureOnPBefore->next = featureOnP->next;
        }
    }
    return 0;
}

void leaveCompany(Worker* workers[], int* workerCount, Project* projects[], int* projectCount) {
    //check the worker count
    if (*workerCount == 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }

    int choice;
    printf("Select a worker to leave the company:\n");
    //print workers
    showTheWorkers(*workerCount, workers);
    printf("Enter the worker's number: ");
    scanf(" %d", &choice);
    //check input
    if (choice <= 0 || choice > *workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }
    choice--;
    for (int i = 0; i < workers[choice]->projectCount; ++i) {
        Project* project = workers[choice]->projects[i];
        // Remove worker from project
        for (int j = 0; j < project->workerCount; ++j) {
            if (project->workers[j] == workers[choice]) {
                for (int k = j; k < project->workerCount - 1; ++k) {
                    project->workers[k] = project->workers[k + 1];
                }
                project->workerCount--;
                break;
            }
        }
        //check if we finish with all workers
        if (project->workerCount == 0) {
            freeProject(project, projectCount);
            for (int j = i; j < *projectCount - 1; ++j) {
                projects[j] = projects[j + 1];
            }
            //remove 1 project
            (*projectCount)--;
            i--;
        }
    }
    free(workers[choice]->name);
    free(workers[choice]->projects);
    free(workers[choice]);
    for (int i = choice; i < *workerCount - 1; ++i) {
        workers[i] = workers[i + 1];
    }
    //(*workerCount)-- becuase 1 leave
    (*workerCount)--;

}
void freeAllAllocations(Worker* workers[], int workerCount, Project* projects[], int projectCount) {
    for (int i = 0; i < workerCount; ++i) {
        for (int j = 0; j < workers[i]->projectCount; ++j) {
            //free feature first
            freeFeatures(workers[i]->projects[j]->features);
        }
        free(workers[i]->projects);
        free(workers[i]->name);
        free(workers[i]);
    }
    for (int i = 0; i < projectCount; ++i) {
        freeFeatures(projects[i]->features);
        free(projects[i]->workers);
        free(projects[i]->name);
        free(projects[i]);
    }

}
void freeProject(Project* project, int* projectCount) {
    (*projectCount)--;
    free(project->workers);
    freeFeatures(project->features);
    free(project->name);
    project->workerCount--;
}
void freeFeatures(FeatureNode* head) {
    FeatureNode* nextOne;
    FeatureNode* current = head;
    while (current != NULL) {
        nextOne = current->next;
        free(current->feature);
        free(current);
        current = nextOne;
    }
}
void showTheWorkers(int workerCount, Worker* workers[]) {
    //prints the workers
    for (int i = 0; i < workerCount; ++i) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
}
void printProject(int projectCount, Project* projects[]) {
    //prints the projects
    for (int i = 0; i < projectCount; ++i) {
        printf("%d. %s (Workers: %d)\n", i + 1, projects[i]->name, projects[i]->workerCount);
    }
}
void showTheWorkersInP(int projectCount, Project* projects[]) {
    //prints the projects with the workers count
    for (int i = 0; i < projectCount; ++i) {
        printf("%d. %s (Workers: %d)\n", i + 1, projects[i]->name, projects[i]->workerCount);
    }
}
void printFeatureNode(FeatureNode* node) {
    int number = 1;
    FeatureNode* newFeatureNode = node;
    while (newFeatureNode != NULL) {
        printf("%d. %s\n", number, newFeatureNode->feature);
        newFeatureNode = newFeatureNode->next;
        number++;
    }
}