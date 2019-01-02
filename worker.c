



void worker(void *func(void*)){
    // argument pointer holder for alle datatyper
    // nødvendigt da man ikke kan cast fra void** til andre typer.
    void* arg;
    
    // hent data
    job_queue_pop(&job_queue, &arg);

    // brug data
    func(arg);

}