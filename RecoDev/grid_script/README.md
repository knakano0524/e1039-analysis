scripts to run the DST data. This script assumes you copied the dst data to `/pnfs/e906/persistent/users/$USER/DstRun` area. 

`splitDST.C`: splits the longer DST runs in specified number of spills. Useful for those longer runs which can't be finished within the 24 hours limit. For now, the default number of spills is set to 750. After splitting, you need to follow the same procedure as normal DST and run grid jobs. 

```
root -b -q splitDst.C\(-1\)
```

The splitted dst files are outputed to `spill/run_number/`

