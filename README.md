# Branch `ana_trig_acc` of Repository `knakano0524/e1039-analysis`

A modified version of `E1039-Collaboration/e1039-analysis` 
to conveniently analyze the trigger acceptance.

## Analysis Flow

1. Download this package and switch the branch
    ```
    git clone https://github.com/knakano0524/e1039-analysis.git
    cd e1039-analysis
    git checkout ana_trig_acc
    ```
1. Event generation
    - Via `SimChainDev`
    - You can skip this step and instead use an existing dataset under `/pnfs/e906/persistent/users/kenichi/sim_data/dy_acc_20201101_0[123]`
        - 20k generated events/job * 800 jobs
        - OK = 731, Bad job host = 64, Run-time limit = 5
        - 572,108 events saved on DSTs
    - The generation condition (in `Fun4Sim.C`) has been modified
        - Drell-Yan process via `SQPrimaryParticleGen`
    	- 1.5 < Mass < 5.0
        - -0.15 < xF < 0.95
        - Only in-acceptance events are saved (by `save_in_acc = true`)
1. Event analysis
    - Via `AnaSimDst`
    - The list of DSTs for analysis (i.e. `list_dst.txt`) by default points to the existing dataset mentioned above

## Procedure for Event Generation

Below is an example of overall commands.
Please take a look at [`SimChainDev/README.md`](SimChainDev/README.md) to understand the meaning of each command.

```
cd SimChainDev
source /e906/app/software/script/setup-jobsub-spinquest.sh
kinit
./gridsub.sh dy_acc_20201101_01 1 100 20000
```

## Procedure for Event Analysis

You might have to update the DST list _if_ you generate your own events.
Below is an example of overall commands.
Please take a look at [`AnaSimDst/README.md`](AnaSimDst/README.md) to understand the meaning of each command.

```
cd AnaSimDst
source setup.sh
cd build
cmake-ana-sim-dst
make install
cd ../work
root -b -q Fun4SimDst.C
root -b -q Fun4SimMicroDst.C
root -b -q Fun4SimTree.C
ls -l result
```

Remarks:
- `DPTriggerAnalyzer` is registered in `Fun4SimMicroDst.C`.
  You can apply a new roadset for test here, via `set_road_set_file_name()`.
- `Fun4SimTree.C` computes the trigger acceptance and outputs a set of plots under `result/`.
