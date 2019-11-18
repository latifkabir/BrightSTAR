/*
 * example1.C
 * Created May 2012 by S. Gliske (sgliske@anl.gov)
 *
 * Example macro to read EEmcTrees without using the class dictionaries.
 *
 */

void example1( const Char_t *filelist_part1 = "treeList.Part1.test.short.txt",
               const Char_t *filelist_part2 = "treeList.Part2.test.short.txt",
               const Char_t *filelist_part3 = "treeList.Part3.test.short.txt" ){

   // Instead of just opening a TTree, we want to make a TChain.  A
   // TChain is just a concatonation of multiple TTrees, or rather the
   // same type of TTree spread over multiple files.  The argument of
   // the TChain constructor is the name of the TTree, in our case
   // just "tree", and later we will tell it in what files to look for
   // this tree.  Note, the TChain class is a child class of TTree,
   // and so all that a TTree can do, it can, plus more.

    TChain *chain1 = new TChain("tree");
    TChain *chain2 = new TChain("tree");
    TChain *chain3 = new TChain("tree");

   // We next need to open the file which contains all the root file
   // names.  Since we will need to do this three times, we will make
   // some arrays and do a single loop.
   TChain *allChains[3] = { chain1, chain2, chain3 };
   const Char_t *allNames[3] = { filelist_part1, filelist_part2, filelist_part3 };

   // ready to loop
   for( Int_t ipart = 0; ipart < 3; ++ipart ){
      // Construct an input file stream, call it fin and give it the
      // name of the file
      std::ifstream fin( allNames[ipart] );

      // make sure it opened OK
      if( !fin.is_open() ){
         cerr << "ERROR opening file '" << allNames[ipart] << "'" << endl;
         return;
      };

      // To store the filename and also the last file name.  Sometimes
      // if there is extra space, the end is not detected right away
      // and the last file could be added twice.
      std::string filename, lastFilename;

      // loop over contents of the input file
      while( !fin.eof() ){
         lastFilename = filename;
         filename.clear();
         fin >> filename;

         // if the filename isn't the same as on the last loop, add
         // the file to the list of files for the TChain.
         if( lastFilename != filename ){
            cout << "Adding file '" << filename << "'" << endl;
            allChains[ipart]->Add( filename.data(), -1 );
         };

         // check to see if there is anything left in the file
         fin >> ws;
         fin.peek();
      };
   };

   // lets make sure the chains are OK

   // here's a message to the user
   for( Int_t ipart = 0; ipart < 3; ++ipart )
      cout << "Part " << ipart+1 << " has " << allChains[ipart]->GetEntries() << " entries." << endl;

   // lets define the number of events as the number in the last
   // TChain, and make sure the other chains have at least as many.
   // (Really they should all be equal.)
   Int_t nevents = chain3->GetEntries();
   if( chain1->GetEntries() < nevents ){
      cerr << "ERROR: Part 1 has " << chain1->GetEntries() << " but Part 3 has " << nevents << endl;
      return;
   };
   if( chain2->GetEntries() < nevents ){
      cerr << "ERROR: Part 2 has " << chain2->GetEntries() << " but Part 3 has " << nevents << endl;
      return;
   };

   // Some versions of root do not like the array of strips in the
   // clusters.  Sometimes ROOT is just weird and unpredictable.
   // Here's a "hack" to get around it--we'll just turn off that
   // branch, since we're not using it for this example.
   chain2->SetBranchStatus("*strip*",0);
   chain2->SetBranchStatus("*tower*",0);

   // Lets see what each of the trees contain.  We'll have the 1st
   // event placed on the screen just for fun.
   cout << "This is Part 1 of the first event" << endl;
   chain1->Show(0);
   cout << endl;

   cout << "This is Part 2 of the first event" << endl;
   chain2->Show(0);
   cout << endl;

   cout << "This is Part 3 of the first event" << endl;
   chain3->Show(0);
   cout << endl;

   // Next, we make the trees "friends".  Whereas the TChain can be
   // though of putting TTrees together horizontally, making Trees
   // friends is like putting Trees together vertically.  The fields
   // of the "friend" trees are available to the other tree as if they
   // were there own.  For our case, we'll add part 1 and part 2 as
   // friends to part 3, and ever after can just use part 3.
   chain3->AddFriend( chain1 );
   chain3->AddFriend( chain2 );

   // Here are some drawing options.  You may prefer other options.
   gROOT->SetStyle("Plain");
   gStyle->SetTitleBorderSize(0);
   gStyle->SetOptStat(1110);   // stat box will show entries, mean and rms but not histo name

   // Next, we make a TCanvas to place our histograms
   TCanvas *can = new TCanvas( "can", "" );

   // You should compare what you see with the class header
   // definitions and make sure you understand the relations.

   // Lets make a histogram of the mass of the pi0 candiates.  Part 3
   // has a branch which is a TClonesArray of the class
   // EEmc2ParticleCandidate_t, which is container to hold information
   // about candidate particles which decay into two other particles,
   // such as pi0s.  The branch has been given the simple name "pi0".

   // A nice thing about ROOT is that the tree is "split", meaning
   // that the internal contents of the class is available.  For
   // example, the EEmc2ParticleCandidate_t class has a field called M
   // which is just a Float_t and stores the mass of the two particle
   // system.

   //  Additionally, ROOT allows you to ignore the fact that
   // the branch contains a TClonesArray and instead treat it as if it
   // were just a single copy of the class that was saved to the file.
   // The net result of all this is that one could make the histogram
   // by simply calling

   chain3->Draw("pi0.M");

   // And now your TCanvas has a histogram of the pi0 masses for all
   // pi0 candidates from all events.  The canvas will be overwritten
   // in a moment, so let's save this to an eps so you can look at it
   // later.

   can->Print("example1.histogram0.eps");

   // So you have a histogram, but it is not very pretty.  Lets tell
   // it to use 100 bins and only stay in a certain range, say 0 to
   // 0.6 GeV.  We also want to give the histogram a name, so we can
   // grab it and change it.  We do this by

   chain3->Draw("pi0.M>>hMass(100,0,0.6)");

   // Now lets grab the histogram pointer from the canvas.  We do this
   // by asking the current pad for an object with given name. Root
   // returns a pointer to a TObject, a parent of most of ROOT's
   // classes.  We need to cast the pointer to the type of a 1D
   // histogram, i.e. TH1.  

   TH1* hMass = (TH1*)can->GetPrimitive("hMass");

   // Now lets do a few things to make it look nicer.

   hMass->SetLineColor(kBlue+2); // set the line color blue
   hMass->SetTitle("Mass Spectrum with No Cuts;M_{#gamma#gamma} [GeV]; Counts per 6 MeV");

   // We need to update the TCanvas with the changes
   can->Update();

   // Again, lets save the TCanvas so you can see what it looks like.

   can->Print("example1.histogram1.eps");

   // Lets put a cut on the data.  This can be done via a second
   // argument to the TTree::Draw function.  We'd like to requre that
   // both photons (hits) in the pi0 candidate have greater than 3 GeV
   // of energy.  The index of the hits are stored in pi0.hitIdx1 and
   // pi0.hitIdx2.  Note, for now there is a one-to-one correspondance
   // between hits and photon candidates.  One could imagine being
   // smarter about making photon candidates, in which case this
   // correspondance would no longer be guarenteed.

   // The hit information itself is in part 2, but since part 2 is on
   // part 1's friend list, it can be accessed as if it were in part
   // 3.  Hits are in a TClonesArray in a branch called eemcHitArr.
   // Thus we can access the corresponding hits via eemcHitArr[
   // pi0.idx1 ] and eemcHitArr[ pi0.idx2 ].  We then want to cut on
   // the total amount of energy, for each hit, deposited in the
   // Towers.  Here's the full command:

   chain3->Draw("pi0.M>>hMass2(100,0,0.6)", "eemcHitArr[ pi0.hitIdx1 ].total.eTow > 3 && eemcHitArr[ pi0.hitIdx2 ].total.eTow > 3" );

   // lets grab the histogram, clean it up, and save it as before

   TH1* hMass2 = (TH1*)can->GetPrimitive("hMass2");
   hMass2->SetLineColor(kBlue+2);
   hMass2->SetTitle("Mass Spectrum, E_{#gamma,1},E_{#gamma,2} > 3 GeV;M_{#gamma#gamma} [GeV]; Counts per 6 MeV");
   can->Update();
   can->Print("example1.histogram2.eps");

   // Now lets place an additional cut on distance between the photon
   // candidates.  The x and y positions are in eemcHitArr.x and
   // eemcHitArr.y.  To simply things, lets define an "alias."  You
   // can think of an alias as an additional branch that you which you
   // had, but do not, that can be computed from the existing
   // branches.  Lets call the distance between the x positions of the
   // two photons dX and the distance between the y positions as dY.
   // The total distance we can call dTot, for total distance.

   chain3->SetAlias("dX",   "TMath::Abs(eemcHitArr[ pi0.hitIdx1 ].x - eemcHitArr[ pi0.hitIdx2 ].x)");
   chain3->SetAlias("dY",   "TMath::Abs(eemcHitArr[ pi0.hitIdx1 ].y - eemcHitArr[ pi0.hitIdx2 ].y)");
   chain3->SetAlias("dTot", "sqrt( dX*dX - dY*dY )");

   // Lets also define aliases for the hit energies used earlier

   chain3->SetAlias("E1", "eemcHitArr[ pi0.hitIdx1 ].total.eTow");
   chain3->SetAlias("E2", "eemcHitArr[ pi0.hitIdx2 ].total.eTow");

   // now we can make the histogram with the cut on dTot fairly easily.  Lets pick 4 cm as the threshold.

   chain3->Draw("pi0.M>>hMass3(100,0,0.6)", "E1 > 3 && E2 > 3 && dTot > 4" );

   // lets grab the histogram, and do our cleaning

   TH1* hMass3 = (TH1*)can->GetPrimitive("hMass3");
   hMass3->SetLineColor(kRed+1);
   hMass3->SetTitle("Mass Spectrum, E_{#gamma,1},E_{#gamma,2} > 3 GeV & d_{TOT} > 4;M_{#gamma#gamma} [GeV]; Counts per 6 MeV");

   // It'd be nice to compare this plot with the last, so lets redraw the last one, and then draw this one on top of it.

   hMass2->SetTitle("");  // this clears the title and the x and y titles
   hMass2->Draw();
   hMass3->Draw("SAME");

   // You should later learn how to make a legend and add one.  For now, lets just save it and move on.

   can->Print("example1.histogram3.eps");

   // One last trick.  By looking at the last histogram, you'll see
   // the low mass peak happens primarily when the distance dTot is
   // small.  Often, the distance dTot is entirely in the direction of
   // the u or v strips, i.e. the two hits share one of the u or v
   // clusters, but not both.  Lets cut that the distance between the
   // u clusters or the distance between the v clusters is zero.

   // First, lets make alias defining the position of the clusters for
   // both hits and for both u and v planes.  Note, we find the hits
   // from the pi0.hitIdx1 and pi0.hitIdx2, but then need to find the
   // clusters indices via uClusIdx and vClusIdx.  Here's the aliases

   chain3->SetAlias("Upos1", "eemcClusArr[ eemcHitArr[ pi0.hitIdx1 ].uClusIdx ].meanPos" );
   chain3->SetAlias("Vpos1", "eemcClusArr[ eemcHitArr[ pi0.hitIdx1 ].vClusIdx ].meanPos" );
   chain3->SetAlias("Upos2", "eemcClusArr[ eemcHitArr[ pi0.hitIdx2 ].uClusIdx ].meanPos" );
   chain3->SetAlias("Vpos2", "eemcClusArr[ eemcHitArr[ pi0.hitIdx2 ].vClusIdx ].meanPos" );

   // now lets define the differences
   chain3->SetAlias("dU", "TMath::Abs(Upos1 - Upos2)" );
   chain3->SetAlias("dV", "TMath::Abs(Vpos1 - Vpos2)" );

   // Now lets draw the mass with the cut on dU or dV equals zero

   chain3->Draw("pi0.M>>hMass4(100,0,0.6)", "E1 > 3 && E2 > 3 && ( dU == 0 || dV == 0 )" );

   // Again, lets put it on top of the plot with only energy cuts, clean it up, and write it to an eps file.

   TH1* hMass4 = (TH1*)can->GetPrimitive("hMass4");
   hMass4->SetLineColor(kRed+1);
   hMass4->SetTitle("Mass Spectrum, dU or dV is zero;M_{#gamma#gamma} [GeV]; Counts per 6 MeV");
   hMass2->SetTitle("");
   hMass2->Draw();
   hMass4->Draw("SAME");
   can->Print("example1.histogram4.eps");

   // Again, a legend should now be added.  What do you learn about the mass distribution based on looking at this last plot?

   // Note, in C++ one would need to be very careful about deleting
   // allocated memory.  ROOT in fact takes care of 90% of this for
   // you, and so in this example, one can just end without worrying
   // about this.
};
