/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooNameSet.rdl,v 1.4 2001/09/17 18:48:15 verkerke Exp $
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/
#ifndef ROO_NAME_SET
#define ROO_NAME_SET

#include "TList.h"
#include "TString.h"
#include "TClass.h"
#include "RooFitCore/RooAbsArg.hh"
#include "RooFitCore/RooPrintable.hh"
class RooArgSet ;

class RooNameSet : public TObject, public RooPrintable {
public:

  // Constructors, assignment etc.
  RooNameSet();
  RooNameSet(const RooArgSet& argSet);
  RooNameSet(const RooNameSet& other) ;
  virtual TObject* Clone(const char* newname=0) const { return new RooNameSet(*this) ; }
  virtual ~RooNameSet() ;

  void refill(const RooArgSet& argSet) ;
  RooArgSet* select(const RooArgSet& list) const ;
  Bool_t operator==(const RooNameSet& other) ;  

  virtual void printToStream(ostream &os, PrintOption opt= Standard, TString indent= "") const;
  inline virtual void Print(Option_t *options= 0) const {
    printToStream(defaultStream(),parseOptions(options));
  }

  Int_t _len ;
  char* _nameList ; //[_len]

protected:

  ClassDef(RooNameSet,1) // A sterile version of RooArgSet, containing only the names of the contained RooAbsArgs
};

#endif
