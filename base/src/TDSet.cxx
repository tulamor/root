// @(#)root/base:$Name:  $:$Id: TDSet.cxx,v 1.1 2002/01/18 14:24:09 rdm Exp $
// Author: Fons Rademakers   11/01/02

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDSet                                                                //
//                                                                      //
// This class implements a data set to be used for PROOF processing.    //
// The TDSet defines the class of which objects will be processed,      //
// the directory in the file where the objects of that type can be      //
// found and the list of files to be processed. The files can be        //
// specified as logical file names (LFN's) or as physical file names    //
// (PFN's). In case of LFN's the resolution to PFN's will be done       //
// according to the currently active GRID interface.                    //
// Examples:                                                            //
//   TDSet treeset("TTree", "AOD");                                     //
//   treeset.Add("lfn:/alien.cern.ch/alice/prod2002/file1");            //
//   ...                                                                //
//   treeset.AddFriend(friendset);                                      //
//                                                                      //
// or                                                                   //
//                                                                      //
//   TDSet objset("MyEvent", "", "/events");                            //
//   objset.Add("root://cms.cern.ch/user/prod2002/hprod_1.root");       //
//   ...                                                                //
//   objset.Add(set2003);                                               //
//                                                                      //
// Validity of file names will only be checked at processing time       //
// (typically on the PROOF master server), not at creation time.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TDSet.h"
#include "TList.h"
#include "TROOT.h"
#include "TClass.h"


ClassImp(TDSetElement)
ClassImp(TDSet)

//______________________________________________________________________________
TDSetElement::TDSetElement(const char *file, const char *objname,
                           const char *dir)
{
   // Create a TDSet element.

   fFileName = file;

   if (objname)
      fObjName = objname;
   if (dir)
      fDirectory = dir;
}

//______________________________________________________________________________
TDSet::TDSet()
{
   // Default ctor.

   fElements = new TList;
   fElements->IsOwner();
   fIsTree = kFALSE;
}

//______________________________________________________________________________
TDSet::TDSet(const char *type, const char *objname, const char *dir)
{
   // Create a TDSet object. The "type" defines the class of which objects
   // will be processed. The optional "objname" argument specifies the
   // name of the objects of the specified class (the name is mandatory
   // if the type inherits from a TTree). If the "objname" is not given all
   // objects of the class found in the specified directory are processed.
   // The "dir" argument specifies in which directory the objects are
   // to be found, the top level directory ("/") is the default.
   // Directories can be specified using wildcards, e.g. "*" or "/*"
   // means to look in all top level directories, "/dir/*" in all
   // directories under "/dir", and "/*/*" to look in all directories
   // two levels deeps.

   fElements = new TList;
   fElements->IsOwner();

   if (!type || !*type) {
      Error("TDSet", "type name must be specified");
      return;
   }

   TClass *c;
   if (!(c = gROOT->GetClass(type)))
      Warning("TDSet", "type %s not yet known", type);

   fName   = type;
   fIsTree = kFALSE;

   if (c && c->InheritsFrom("TTree"))
      fIsTree = kTRUE;

   if (objname)
      fObjName = objname;

   if (dir)
      fTitle = dir;
}

//______________________________________________________________________________
TDSet::~TDSet()
{
   // Cleanup.

   delete fElements;
}

//______________________________________________________________________________
void TDSet::SetObjName(const char *objname)
{
   // Set/change object name.

   if (objname)
      fObjName = objname;
}

//______________________________________________________________________________
void TDSet::SetDirectory(const char *dir)
{
   // Set/change directory.

   if (dir)
      fTitle = dir;
}

//______________________________________________________________________________
void TDSet::Add(const char *file, const char *objname, const char *dir)
{
   // Add file to list of files to be analyzed. Optionally with the
   // objname and dir arguments the default, TDSet wide, objname and
   // dir can be overridden.

   if (!file || !*file) {
      Error("Add", "file name must be specified");
      return;
   }

   fElements->Add(new TDSetElement(file, objname, dir));
}

//______________________________________________________________________________
void TDSet::Add(TDSet *set)
{
   // Add specified data set to the this set.

   if (!set)
      return;

   TDSetElement *el;
   TIter next(set->fElements);
   while ((el = (TDSetElement*) next()))
      Add(el->GetFileName(), el->GetObjName(), el->GetDirectory());
}

//______________________________________________________________________________
void TDSet::AddFriend(TDSet *friendset)
{
   // Add friend dataset to this set. Only possible if the TDSet type is
   // a TTree or derived class.

   if (!friendset)
      return;

   if (!fIsTree) {
      Error("AddFriend", "a friend set can only be added to a TTree TDSet");
      return;
   }

   // to be implemented
}


