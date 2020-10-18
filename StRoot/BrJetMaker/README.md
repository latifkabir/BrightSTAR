Here I developed custom Jet event classes/dictionary in an attempt to save disk space by keeping only the branches needed. The custom classes have similar design as the original ones.
In most cases it might be worthy to just use original classes and rather fillter (say cut on eta) the events to save the disk space. Note the entire event is saved as StJetEvent object. So copying only desired events would be straight forward. See BrFmsSimTreeMaker as an example.



