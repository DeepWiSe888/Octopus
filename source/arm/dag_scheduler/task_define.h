/*
 * Project Octopus
 *
 */


#ifndef INC_TASK_DEFINE_H_
#define INC_TASK_DEFINE_H_

#ifdef  __cplusplus
extern "C"
{
#endif
#pragma pack(1)

// ------- radar info ------- //
typedef struct _radar_info{
    void* data_ddr;
    int nTX;
    int nRX;
    int nRangeBin;
    int fps;
    int winsize;
    int nTxRxPairs; // max 8tx*8rx, min 1tx*1rx
    long* pairInfo; // tx_no <<16 | rx_no
                     // = NULL if nPairs=1
}radar_info;



// ------- task defines ------- //
// _________________________
// |       task name       |
// |     function ptr      |              --[read input]-> {function} --[write output]->  ... next task
// |    input data ptr     |          <takeSemaphore>           ^            <giveSemaphore>
// |    output data ptr    |                                    |
// |     calc param ptr    |                  -- [calc param] __|
// |       node num        |
// |      pre node num     |
// -------------------------

typedef struct _task_info{
	char func_name[16]; // "FIR" "FFT1D"
	void* func_ptr;     // execute function ptr during task flow runing
	void* input;        // normaly is preNode output; use as matrix recommanded
	void* output;       // alloc memory during task building; use as matrix recommanded
    void* params;       // use by task func_ptr
	int   node;
    int   runFlag;      // TASK_RUNNING_FLAGS
}task_info;

enum TASK_RUNNING_FLAGS
{
    TASK_FLAG_WAIT = 0,
    TASK_FLAG_RUNNING = 1,
    TASK_FLAG_FINISH = 2,
    TASK_FLAG_RUNERROR = 3,
};

typedef struct _task_list{      // maybe i'll use acyclic_graph or tree finnaly,
	task_info* taskInfo;        // but now list for quick performing.
	struct _task_list* next;
}task_list;

typedef struct _task_sem_info{
	char preTask;
	char nextTask;
	char semNo;
}task_sem_info;

typedef struct _task_sem_list{      // maybe i'll use acyclic_graph or tree finnaly,
	task_sem_info semInfo;        // but now list for quick performing.
	struct _task_sem_list* next;
}task_sem_list;

// task flow like this
// [radar data] -- [task 1] -- [task2] -- ... -- [output A]
//                     |
//                     |-- [task 3] -- [task4] -- ... -- [output B]

typedef struct _task_flow{
	char* radarInfoDescrib;
	radar_info* radarInfo;

	task_list taskList;
	void* memPool;
	int   memPoolSize;
	task_sem_list* semList;
	//something else
}task_flow;

#pragma pack() //pack(1)

#ifdef  __cplusplus
}
#endif

#endif /* INC_TASK_DEFINE_H_ */
