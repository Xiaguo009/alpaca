
#include <app/analysis/analysis.h>
#include <stdio.h>

static uint16_t max_task_id = 0;

static uint16_t run_time;
static uint32_t total_backup;

static uint16_t task_run_time[20] = {};
static uint32_t task_total_backup[20] = {};

void analysis_clear() {
    max_task_id = 0;
    total_backup = 0;
    run_time = 0;
    for (uint16_t i = 0; i < 20; ++i) {
        task_run_time[i] = 0;
        task_total_backup[i] = 0;
    }
}

void analysis_runtask(uint16_t task_id, uint16_t backup_size) {
    if (max_task_id < task_id)      max_task_id = task_id;
    task_run_time[task_id]++;
    run_time++;
    task_total_backup[task_id] += backup_size;
    total_backup += backup_size;
}

void analysis_printout() {
    printf("Max task id: %d\n", max_task_id);
    printf("Total tasks: %d\n", run_time);
    printf("Total bakcup size: %ld\n", total_backup * 2);

    printf("Task run time: ");
    for (uint16_t i = 0; i <= max_task_id; ++i)
        printf("%d ", task_run_time[i]);

    printf("\nTasked-based total backup size: ");
    for (uint16_t i = 0; i <= max_task_id; ++i)
        printf("%ld ", task_total_backup[i] * 2);
}


