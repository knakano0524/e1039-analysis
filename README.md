# Branch `ana_trig_acc` of Repository `knakano0524/e1039-analysis`

A modified version of `E1039-Collaboration/e1039-analysis` 
to conveniently analyze the trigger acceptance.

## Analysis Flow

1. Download this package

```
git clone https://github.com/knakano0524/e1039-analysis.git
cd e1039-analysis
git checkout ana_trig_acc
```

1. Event generation = `SimChainDev`
    - You can skip this step and use an existing dataset
        - 20k generated events/job $\times$ 800 jobs
        - OK = 731, Bad job host = 64, Run-time limit = 5
        - Stored in {\scriptsize\tt /pnfs/e906/persistent/users/kenichi/sim\_data/dy\_acc\_20201101\_0[123]}
    	- Listed in {\tt e1039-analysis/AnaSimDst/work/list\_dst.txt}
    - `Fun4Sim.C` has been modified
        - E906LegacyGen
    	- mass, xF

```
cd SimChainDev
source
source
./gridsub.sh 
```

1. Event analysis = `AnaSimDst`
    - Make a list of DSTs for analysis
        - The default list `list_dst.txt` contains the existing dataset mentioned above.
	- You can make your own list by following `AnaSimDst/README.md`.
    - Compile the analysis module
	- As explained in `AnaSimDst/README.md`.
    - Run ``Fun4SimDst.C'', ``Fun4SimMicroDst.C'' & ``Fun4SimTree.C''
        - These macros have been modified.
    - Various plots are outputted to `result/`.

```
root -b Fun4SimDst.C
root -b Fun4SimMicroDst.C
root -b Fun4SimTree.C
```



