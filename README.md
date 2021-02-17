# Scheduler Simulator
A scheduler simulator project for Operative Systems course at UniGe - University of Genoa.


## Project Goal
The main goal was to simulate two scheduler, the first one preemptive and the second one not preemptive, test them on a (long) series of task and compare the output reports for each scheduler. <br>
the project aims to **implement some IPC mechanisms**, listed in the next section.
We have to simulate instructions, tasks, processors, cores, clock and schedulers. <br>
The design specifications file is [there](docs/spec2017-2018.pdf) (italian).

## IPC mechanisms
We have to use and allocate:
- fork
- shared memory
- thread
- semaphores
- signals

## Usage
```bash
./simulator -op out_pre.csv -on out_not_pre.csv -i ./input/01_jobs.dat -q 1
```

## Parameters
- **-op | --output-preemption** : csv file used by the scheduler preemptive to write outputs.
- **-on | --output-no-preemption** : same as previous, for the scheduler not preemptive.
- **-i | --input** : the input file containing the tasks list.
- **-q** : time slot or quantum for the preemptive scheduling algorithm (round robin).
- **-h | --help** : usage helper.


## Authors
This project was proudly manteined by **Giorgio Rossi** and **[Camilla Bianchi](https://github.com/camillina03)** (FFlush Team)
