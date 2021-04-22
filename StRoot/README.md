
Modules Inside StRoot
=====================

- Modules that start with `Br` are pristine development and part of Bright-STAR framework. Other modules are collected from either STAR-CVS or other analyzers.

- The main DST maker module in this framework is the `BrNanoDstMaker`.

Description of externally inherited modules
------------------------------------
- BrFemtoMaker: Based on the DST maker for Breit-Wheeler process analysis source code.
- BrPicoMaker: Mini Tree maker out of Pico DST files. Modified based on another anallyzer's code.
- StFmsJetAnalysisMaker: Based on Zhanwen's FMS jet analysis maker.
- StFmsOFileMaker: Chris Dilks' FMS+RP DST maker
- StMuRpsUtil: After burner for RP. Not available in STAR-CVS. The alignment and hot-strips list are for run 15 only. The vertex is set event by event and the value is currently set for Run 15.  
- StSpinPool: Slightly modified version of StSpinPool for EM Jet Analysis.
- StJetMaker: Slightly modified version of StJetMaker for EM Jet Analysis.


