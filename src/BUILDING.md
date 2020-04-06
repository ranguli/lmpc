Convert to script or makefile:


The .l files (lex) must be built with:

```
flex udeml.l
cp lex.yy.c udeml.c

```

The .y files (yacc/bison) must be built with:
```
bison -d -v udemy.y
cp udemy.tab.h udemy.h

```

Otherwise the build fails because it needs those source files 

** The udeml.l and udemy.y files are a working, living implementation of the DEM text format spec. They contain the complete language grammar documentation. **



A note on future testing:

- Record a demo, take note of its sha256sum.
- Decompile it with `./lmpc --to-txt demofilehere.dem demofilehere.demtext`
- Convert it back into a binary with `./lmpc --to-bin demofilehere.demtext`
- If the hashes are the same, poggers
