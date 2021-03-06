#include "Task.h"


Task* CreateTask(int id, float duration, Station* station, int workersMin, int workersMax)
{
     Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->id = id;
    newTask->duration = duration;
    newTask->predecessors = (Task**)malloc(0);
    newTask->successors = (Task**)malloc(0);
    newTask->station = station;
    AddTask(station, newTask);
    newTask->workersMin = workersMin;
    newTask->workersMax = workersMax;
    newTask->nbWorkersToAssign = 0;

    newTask->assignedWorkers = NULL;
    ReinitTask(newTask);

    newTask->nbPredecessors = 0;
    newTask->nbSuccessors = 0;

    _nbTasks++;

   return newTask;
}
void ReinitTask(Task* task)
{
    task->availabilityTime = -1;
    free(task->assignedWorkers);
    task->assignedWorkers = (Worker**)malloc(0);
    task->beginningTime = -1;
    task->endingTime = -1;
    task->nbPredecessorsDone = 0;
	task->nbAssignedWorkers = 0;
}

void ClearTasks(Task*** tasks)
{
    int i;
    int nbTasks = _nbTasks;
    for (i = 0; i < nbTasks; i++)
    {
        DeleteTask((*tasks)[i]);
    }
    free(*tasks);
}

float CalculateCurrentDuration(Task* task)
{
    return CalculateDuration(task, task->nbWorkersToAssign);
}
float CalculateDuration(Task* task, int nbWorkers)
{
//    return task->duration / nbWorkers;
	float duree = task->duration;
	float maxWorker = task->workersMax;
	float minWorker = task->workersMin;

	return maxFloat(1, duree*(1 - ((nbWorkers - minWorker) / maxWorker)));
}

void ExerciseTask(Task* task, float beginningTime)
{
    int i = 0;

    task->beginningTime = beginningTime;
    task->endingTime = beginningTime + CalculateCurrentDuration(task);

    for (i = 0; i < task->nbAssignedWorkers; i++)
    {
        task->assignedWorkers[i]->availabilityTime = task->endingTime;
    }
    for (i = 0; i < task->nbSuccessors; i++)
    {
        task->successors[i]->availabilityTime = task->endingTime;
        task->successors[i]->nbPredecessorsDone++;
    }
}
void assignCriticakTaskPathWay(Task** tasks, int **TasksCriticalPathWayRecop, int nbWorkers)
{
	int i = 0;
	while ((*TasksCriticalPathWayRecop)[i] != -1)
	{
		tasks[(*TasksCriticalPathWayRecop)[i]]->nbWorkersToAssign = minInt(tasks[(*TasksCriticalPathWayRecop)[i]]->nbWorkersToAssign + 1, minInt(nbWorkers, tasks[(*TasksCriticalPathWayRecop)[i]]->workersMax));
		i++;
	}
}
void AddPredecessor(Task* task, Task* predecessor)
{
    task->predecessors = (Task**)realloc(task->predecessors, ++task->nbPredecessors * sizeof(Task*));
    task->predecessors[task->nbPredecessors - 1] = predecessor;

    predecessor->successors = (Task**)realloc(predecessor->successors, ++predecessor->nbSuccessors * sizeof(Task*));
    predecessor->successors[predecessor->nbSuccessors - 1] = task;
}
void AddSuccessor(Task* task, Task* successor)
{
    task->successors = (Task**)realloc(task->successors, ++task->nbSuccessors * sizeof(Task*));
    task->successors[task->nbSuccessors - 1] = successor;

    successor->predecessors = (Task**)realloc(successor->predecessors, ++successor->nbPredecessors * sizeof(Task*));
    successor->predecessors[successor->nbPredecessors - 1] = task;
}

void DeleteTask(Task* task)
{
    int i = 0;
    int j = 0;

    // delete from predecessors and successors
    for (i = 0; i < task->nbPredecessors; i++)
    {
        for (j = 0; j < task->predecessors[i]->nbSuccessors; j++)
        {
            if (task->predecessors[i]->successors[j]->id == task->id)
            {
                task->predecessors[i]->successors = (Task**)RemoveAt((void**)task->predecessors[i]->successors, task->predecessors[i]->nbSuccessors--, j);
                break;
            }
        }
    }
    free(task->predecessors);

    for (i = 0; i < task->nbSuccessors; i++)
    {
        for (j = 0; j < task->successors[i]->nbPredecessors; j++)
        {
            if (task->successors[i]->predecessors[j]->id == task->id)
            {
                task->successors[i]->predecessors = (Task**)RemoveAt((void**)task->successors[i]->predecessors, task->successors[i]->nbPredecessors--, j);
                break;
            }
        }
    }
    free(task->successors);

    free(task->assignedWorkers);
    free(task);

    _nbTasks--;
}
/*
void recopierTaches(Task * tache1, Task * tache2)
{
	tache1->id = tache2->id;
	tache1->duration = tache2->duration;
	tache1->predecessors = tache2->predecessors;
	tache1->successors = (Task**)malloc(0);
	tache1->station = station;
	AddTask(station, newTask);
	newTask->workersMin = workersMin;
	newTask->workersMax = workersMax;
	newTask->nbWorkersToAssign = 0;

	newTask->assignedWorkers = NULL;
	ReinitTask(newTask);

	newTask->nbPredecessors = 0;
	newTask->nbSuccessors = 0;


}
*/