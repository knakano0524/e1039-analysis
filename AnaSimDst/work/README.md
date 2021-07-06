# e1039-analysis/AnaSimDst/work - Special Version for Geometric Acceptance

## Event generation

Use `e1039-analysis/SimChainDev` to generate two sets of events;
 - `4pi` = A set of events in 4 pi
 - `acc` = A set of events in acceptance

Here `acc` is made with `SQGeomAccLoose`.

## Processing `4pi` event set

```
mkdir  4pi
cd 4pi
../make-dst-list.sh ../../../SimChainDev/data/dy_4pi_20210703_0*
root -b ../Fun4SimDst.C
root -b ../Fun4SimMicroDst.C
root -b ../Fun4SimTree.C
```

## Processing `acc` event set

```
mkdir  acc
cd acc
../make-dst-list.sh ../../../SimChainDev/data/dy_acc_20210704_0*
root -b '../Fun4SimDst.C(true)'
root -b ../Fun4SimMicroDst.C
root -b ../Fun4SimTree.C
```

## Calculation of geometry acceptance

```
root -b draw_geom_acc.C
```
