#define MAX_WORKERS 50 
#define MAX_PROJECTS 100 
#define MAX_WORKERS_IN_PROJECT 4 
#define ADD_WORKER 1
#define NEW_PROJECT 2
#define JOIN 3
#define ALL_PROJECT 4 
#define WHERE_WORK 5 
#define WORKING_PROJECT 6
#define LEAV 7
#define EXIT 8 
typedef struct Project Project;
typedef struct Worker Worker;
typedef struct FeatureNode FeatureNode;
struct Worker
{
    char* name;
    Project** projects;
    int projectCount;
};

struct FeatureNode
{
    char* feature;
    struct FeatureNode* next;
};

struct Project
{
    char* name;
    Worker** workers;
    int workerCount;
    FeatureNode* features;
};
void menu();
void joinCompany(Worker* workers[], int* workerCount);
void openNewProject(int workerCount, Worker* workers[], Project* projects[], int* projectCount);
void joinExistingProject(Worker* workers[], int workerCount, Project* projects[], int projectCount);
void displayWorkers(Worker* workers[], int workerCount);
void addAndRemoveFeature(Worker* workers[], int workerCount, Project* projects[], int projectCount);
int addFeatureToProject(Project* project);
int removeFeatureFromProject(Project* project);
void printTheProjects(Project* projects[], int projectCount);
int checkWorker(Worker* w, Project* project);
void leaveCompany(Worker* workers[], int* workerCount, Project* projects[], int* countProject);
void showTheWorkers(int workerCount, Worker* workers[]);
void printProject(int projectCount, Project* project[]);
void printFeatureNode(FeatureNode* node);
void freeProject(Project* project, int* projectCount);
void freeFeatures(FeatureNode* head);
void freeAllAllocations(Worker* workers[], int workerCount, Project* projects[], int projectCount);
char* getChar();