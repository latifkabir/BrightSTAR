

1. Run Part 1:

```
root4star 'RunEEmcTreeMakerPart1.C("/star/u/kabir/pwg/data/st_fms_16066050_raw_5000002.MuDst.root","EEmcTreeP1.root",1000,0)'
```


2. Run Part 2:

```
root4star 'RunEEmcTreeMakerPart2and3.C("EEmcTreeP1.root", "out.root", -1, 0, 100)'
```
