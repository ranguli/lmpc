Convert to script or makefile:


The .l files (lex) must be built with:

```
flex udeml.l
cp udeml.yy.c udeml.c

```

The .y files (yacc/bison) must be built with:
```
bison -d -v udemy.y
cp udemy.tab.h udemy.h

```

Otherwise the build fails because it needs those source files 

** The udeml.l and udemy.y files are a working, living implementation of the DEM text format spec. They contain the complete language grammar documentation. **
