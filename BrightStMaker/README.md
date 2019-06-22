Bright STAR Maker
===================

- Maker modules are inside `StRoot/` directory and named after each detector or analysis. It start with `Br` to distinguish from `StRoot` similar modules.

- The Maker classes start with `TSt` and the corresponding root macro is inside `macros` directory. Each macro start with `Run` followed by corresponding maker class name (without `TSt`). The macros have extension `.C`.

- All required shared objects are loaded from `rootlogon.C` file on `root` startup and not from the maker macros. Do not use symbolic link. Symbolic link is copied as symbolic link.

- All batch farm job description files are inside `jobConfig/` and has the file name pattern `Sub` + Maker macro name (without Run prefix).

- Current directory structure:

```
├── jobConfig
├── jobDir -> /star/u/kabir/pwg/jobDir
├── jobOutput -> /star/u/kabir/pwg/jobOutput
├── jobResults -> /star/u/kabir/pwg/jobResults
├── macros
├── README.md
├── rootlogon.C
├── RunList.txt
├── SchedulerSubmit.csh
├── Scheduler_template.xml
├── StRoot
├── SubDeltaPhiMaker.xml

```
