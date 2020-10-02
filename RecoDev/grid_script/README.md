`gridrun_data.sh` and `gridsub_data.sh` :
are scripts to run the `RecoE1039Data.C`. Since `/data2` area is not acessible during job submission, this script assumes you copied the dst data files from `/data2` area to `/pnfs/e906/persistent/users/$USER/DstRun` area. Please refer to the [wiki page](https://github.com/E1039-Collaboration/e1039-wiki/wiki/data-file-on-grid) about setting the file location.
```
./gridsub_data.sh ouput_dir 1 runlist.txt -1 >test-grid.log
```
Here, the "-1" options lets you run all the available events in the run.

`splitDST.C`: 
splits the longer DST runs in specified number of spills. Useful for those longer runs which can't be finished within the 24 hours limit. For now, the default number of spills is set to 750. 

```
root -b -q splitDst.C\(-1\,run_number\)
```

The splitted dst files are outputed to `spill/run_number/`. After splitting, get the splitted run list (depends on step size of the split) and follow similar method as above. For this, you need to change the format of input and output as required. 

