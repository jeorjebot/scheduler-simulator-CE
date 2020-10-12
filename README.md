# scheduler-simulator-CE
A scheduler simulator project for Operative System course at UniGe - University of Genoa.


## Project Goal
The main goal was to simulate two scheduler, the first one preemptive and the second one not preemptive, test them on a (long) series of task and compare the output reports for each scheduler. <br>
We have to simulate instructions, tasks, processors, cores, clock and schedulers.

## IPC
We have to use and allocate:
- fork
- shared memory
- thread
- semaphores
- signals

## Usage
```sh
./simulator -op out_pre.csv -on out_not_pre.csv -i ./input/01_jobs.dat -q 1
```

## Parameters
- **-op | --output-preemption** : csv file used by the scheduler preemptive to write outputs.
- **-on | --output-no-preemption** : same as previous, for the scheduler not preemptive.
- **-i | --input** : the input file containing the tasks list.
- **-q** : time slot or quantum for the preemptive scheduling algorithm (round robin).
- **-h | --help** : usage helper.

