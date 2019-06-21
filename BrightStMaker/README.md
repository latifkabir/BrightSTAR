Bright STAR Maker
===================

- Maker modules are inside `StRoot/` directory and named after each detector or analysis. It start with `Br` to distinguish from `StRoot` similar modules.

- The Maker classes start with `TSt` and the corresponding root macro is inside `macros` directory. Each macro start with `Run` followed by corresponding maker class name (without `TSt`). The macros have extension `.C`.

- All required shared objects are loaded from `rootlogon.C` file on `root` startup and not from the maker macros.

- All batch farm job description files are inside `jobConfig/` and has the file name pattern `Sub` + Maker macro name (without Run).

- Before you submit the job, copy the desired job description file (xml file) to the top level directory of `BrightStMaker` and submit your job from there. Inside job description xmlfile, you need to call the macros with `macros/` prefix.

- Inside `StMaker`, make a symbolic link of the directory `libBrStar` if your maker class is uisng classes from that module. It's better not to use `libBrStar` classes for maker classes.

- Always compile with `cons` and run with `root4star`.

- Submit the job from the top level directory of `BrightStMaker`.
