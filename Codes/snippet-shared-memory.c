/** Important functions to be used while creating a shared memory space
  * Most requires : <sys/shm.h>, <sys/ipc.h> and <sys/types.h>
  * 
  */

#define KEY 4567
#define PERMS 0660	// Define an rw- permission for user and group.

int	id;             // Id of the shared memory segment
int* shared_data;   // Pointer to the shared memory

// SHMGET - Allocate a shared memory segment
//
//      int shmget(key_t key, size_t size, int shmflg);
//
// Example: Allocate a new int sized shared memory segment
id = shmget(KEY, sizeof(int), IPC_CREAT | PERMS);

// SHMAT - Attaches the shared memory segment to the address space 
// of the calling process.
//
//      void* shmat(int shmid, const void* shmaddr, int shmflg);
//
// Example: Get the access to the memory allocated previously
shared_data = (int*) shmat(id, NULL, 0);

// SHMDT - Detaches the shared memory segment from the adress space
// of the calling process.
//
//      int shmdt(const void* shmaddr);
//

// Access to the shared data
*(shared_data) = 42;

// SHMCTL - Shared memory control. Perform the <ctl> control on memory.
//
//      int shmctl(int shmid, int cmd, struct shmid_ds* buf);
//
// Example: Mark the segment to be destroyed, Free the shared memory
shmctl(id, IPC_RMID, NULL);
