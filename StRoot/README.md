
Modules Inside StRoot
=====================

- Modules that start with `Br` are pristine development and part of Bright-STAR framework. Other modules are collected from either STAR-CVS or other analyzers.

Description of inherited modules
------------------------------------
- BrFemtoMaker: Based on the DST maker for Breit-Wheeler process analysis source code.
- BrPicoMaker: Mini Tree maker out of Pico DST files. Modified based on another anallyzer's code.
- StFmsJetAnalysisMaker: Based on Zhanwen's FMS jet analysis maker.
- StFmsOFileMaker: Chris Dilks' FMS+RP DST maker
- StMuRpsUtil: After burner for RP. Not available in STAR-CVS. The alignment and hot-strips list are for run 15 only. This will work only from top level directory of Bright STAR. The vertex is set event by event and the value is currently set for Run 15. While running using scheduler, make sure to send the afterburner alignment and hotstrip files.




