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
    - You can skip this step and use an existing dataset
        - 20k generated events/job * 800 jobs
        - OK = 731, Bad job host = 64, Run-time limit = 5
        - Stored in `/pnfs/e906/persistent/users/kenichi/sim_data/dy_acc_20201101_0[123]`
    - The generation condition (in `Fun4Sim.C`) has been modified
        - Drell-Yan process by E906LegacyGen
    	- Mass range
	- xF range
	- In-acceptance events are saved into DSTs
    - The procedure is explained below
1. Event analysis
    - Via `AnaSimDst`
    - The list of DSTs for analysis (i.e. `list_dst.txt`) by default points to the existing dataset mentioned above
    - The procedure is explained below

## Procedure for Event Generation

You better read `SimChainDev/README.md` beforehand.

```
cd SimChainDev
source /e906/app/software/script/setup-jobsub-spinquest.sh
kinit
./gridsub.sh dy_acc_20201101_01 1 100 20000
```

## Procedure for Event Analysis

You better read `AnaSimDst/README.md` beforehand.

1. Update the DST list only if you generate your own events
1. Compile the analysis module
    - As explained in `AnaSimDst/README.md`.
1. Run `Fun4SimDst.C`, `Fun4SimMicroDst.C` & `Fun4SimTree.C`
    - `DPTriggerAnalyzer` is registered in `Fun4SimMicroDst.C`.
      You can apply a new roadset for test here.
1. Look at a set of plots in `result/`.

