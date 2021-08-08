# Introduce To Dag Scheduler System

Octopus contains a parallel working system, cause many things can be running parallel, such as FPGA and ARM running each tasks at same time, or different ARM/PC side tasks running parallel. 

To manage tasks, ensure that each task is optimized in terms of strict order, and  calculate efficiency optimization, we provide this Dag Scheduler System.

Dag Scheduler System including task unit definitions,  task flow control, memory manage, and general matrix for tasks input/output transmission.



# 1. Task Unit

[^]: codes in task_define.h

Each task unit includes a funciont pointer, a general input, output, and running params. It looks like this below:

```
// ------- task defines ------- //
// _________________________
// |       task name       |
// |     function ptr      |      --[read input]-> {function} --[write output]->   
// |    input data ptr     |     <takeSemaphore>        ^            <giveSemaphore>
// |    output data ptr    |                            |
// |     calc param ptr    |          -- [calc param] __|
// |       node num        |
// |      pre node num     |
// -------------------------
```

A structure task_info is defined for a task unit.

```c
typedef struct _task_info{
   char func_name[16]; // "FIR" "FFT1D"
   void* func_ptr;     // execute function ptr during task flow runing
   void* input;        // normaly is preNode output; use as matrix recommanded
   void* output;       // alloc memory during task building; use as matrix recommanded
   void* params;       // use by task func_ptr
   int   node;
}task_info;,
```

In most conditions, function ptr will point to a function like  `int Foo(task_info* ti)`, and input/output will be a general matrix.  

> matrix definitions in ../blas/blas.h



We also provide some tasks ready in blas_tasks.h, you can directly use them,  Or as a reference for creating a task unit.  Our task creation style looks like this:

```c
int FIR(task_info* ti);
task_info createTaskFIR( /*filter params*/)
{
   ...
}
```



# 2. Task Flow Control

[^]: codes in captain.h

When  the task units are defined, we need to build a system to run them up, we call that task-flow. 

A taks-flow includes a task unit list, memory blocks, semaphores, and dependencies for each task. We also privide function methods to run, destroy, and print out a DAG of the task-flow.

If you have used labview, it's helpful for understanding task-flow logic. Infact the task-flow  is a Directed Acyclic Graph(DAG). We will also provide graphical tools for this DAG system, maybe in future. 

But now you have to call c language functions.

Below is the main function methods for a task-flow.

- Add Task Node 

  `addTaskNode(...)`

  Joining a task unit for task flow. Don't worry about the task running order here, but a task node number is required for dependency management.

- Set Task Dependency

  `setPreTask(...)`

  Setting task pre nodes if there are dependencies. Each task can have more than one pre-tasks, so the tasks looks like a graph.  

- Run Tasks

  Just `runTaskFlow()` and wait for results. A max-time-out can be set if there are unstable factors, usually hardware will have unstable factors, and fault tolerance is required.

- Destroy 

  If you need to run several task-flows, it's recommended that destroying the task-flow after it running, in order to free memories.



The codes for creating a task flow is like these below

```c

	printf("create vital sign task flow...\n");
	task_flow tf = createTaskFlow("vs");
	void* radarDataPtr = getRadarDataAddr(0);
	task_info taskFIR = createTaskFIR(radarDataPtr, 256, "Hamming",1,2);
	task_info taskFFT = createTask1DFFT(taskFIR.output);
	task_info taskVMD = createTaskVMD(0);
	task_info taskGetVital = createTaskGetRpmBpm(taskFFT.output);

	addTaskNode(1,&taskFIR, &tf);
	addTaskNode(2,&taskFFT, &tf);
	addTaskNode(3,&taskVMD, &tf);
	addTaskNode(4,&taskGetVital, &tf);
	setPreTask(2,1, &tf);
	setPreTask(3,2, &tf);
	setPreTask(4,3, &tf);

	printf("run vital sign tasks...\n");

	printTaskFlow(&tf);
	runTaskFlow(&tf, 10);
	destroyTaskFlow(&tf);

	printf("vital sign tasks done.\n");
```

Or you can find the complete example at $source/example/vital_sign.c .



# 4. Util Functions

During the definition of each task, or task flow, we provide several util functions and recommended to use them.

- `taskMemAlloc taskMemFree`

  Considering the platform portability, this memory alloc function is recommended to use instead of alloc/free or new/delete in c++. We Use a memory block list to manage every memory block.  

- Matrixs

  General matrix [defined in blas.h] is recommended to use for task input/outputs. You can use `createMat...`to create or copy a matrix.

  > reference:  Octopus BLAS user guild [ http://... ]

- Semaphore

  Semaphore is useful for task scheduling, we abstract different platforms for semaphore, see `task_sync.h`. Also you can use it in any other conditions. 

- If Connecting FPGA

  If a FPGA board is using, the FMC/FSMC addrs and pre-compiled macros are defined in fpga_define.h, and the I/O interrupts from FPGA will mostly post a semaphore for task flow controlling.

- 

