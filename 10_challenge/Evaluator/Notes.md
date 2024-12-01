## Testcases
- 256MB
- 1GB
- 4GB

## Flags
- O0
- O1
- O2
- O3
- Ofast
- Custom

## Script Procedure:
1. Compile for each flag.
2. Repeat 100x:
- For each testcase: Run + Sleep 5 second.
- Sleep 5 seconds.
3. Write all timing output to a file.


## Evalution:
1. Create test cases.
2. Move them to the directory `eval.sh` is in.
3. Run `eval.sh` for a given header.
