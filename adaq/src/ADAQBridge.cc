///////////////////////////////////////////////////////////////////////////////
//
// name: ADAQBridge.cc
// date: 27 Feb 14
// auth: Zach Hartwig
//
// desc: The ADAQBridge class facilitates communication with the V1718
//       high voltage board with VME communications via the CAENComm
//       and CAENVME libraries. The purpose of ADAQBridge is to
//       obscure the nitty-gritty-details of interfacing with the
//       V1718 board and present the user with a relatively simple set
//       of methods and variables that can be easibly used in his/her
//       ADAQ projects by instantiating a single ADAQBridge "manager"
//       class. Technically, this class should probably be made into a
//       Meyer's singleton for completeness' sake, but the present
//       code should be sufficient for anticipated applications and
//       userbase.
//        
//       At present, the ADAQBridge class is compiled into two
//       shared object libraries: libADAQ.so (C++) and libPyADAQ.so
//       (Python). C++ and Python ADAQ projects can then link against
//       these libraries to obtain the provided functionality. The
//       purpose is to ensure that a single version of the ADAQ
//       libraries exist since they are now used in multiple C++
//       projects, multiple ROOT analysis projects, and new Python
//       projects for interfacing ADAQ with MDSplus data system.
//
///////////////////////////////////////////////////////////////////////////////

// C++
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
using namespace std;

// Boost
#include <boost/assign/std/vector.hpp>
using namespace boost::assign;

// CAEN
extern "C" {
#include "CAENComm.h"
#include "CAENVMElib.h"
}

// ADAQ
#include "ADAQBridge.hh"


ADAQBridge::ADAQBridge()
  :
  // The board handle
  BoardHandle(-1),

  // Bool to determine VME connection state to V6534
  LinkEstablished(false),

  // Bool to determine if output printed to cout
  Verbose(false)
{;}


ADAQBridge::~ADAQBridge()
{;}


int ADAQBridge::OpenLink(uint32_t BrdAddr)
{
  int Status = -42;
  /*
  // If the link is not currently valid then establish one!
  if(!LinkEstablished)
    Status = CAENComm_OpenDevice(CAENComm_USB, 0, 0, 0, &BoardHandle);
  else
    if(Verbose)
      cout << "\nADAQBridge: Error opening link! Link is already open!\n"
	   << endl;

  uint32_t data;

  
  // Set the LinkEstablished bool to indicate that a valid link nto
  // the V1718 has been established and output if Verbose set
  if(Status==0){
    LinkEstablished = true;
    if(Verbose)
      cout << "\nADAQBridge : Link successfully established!\n"
	   <<   "                  --> V1718 base address: 0x" 
	   << setw(8) << setfill('0') << hex << 0x00000000 << "\n"
	   <<   "                  --> V1718 handle: " << BoardHandle
	   << endl;
  }
  else
    if(Verbose and !LinkEstablished)
      cout << "\nADAQBridge : Error opening link! Error code: " << Status << "\n"
	   << endl;
  */
  // Return success/failure 
  return Status;
}


int ADAQBridge::CloseLink()
{
  // If the link is presently established then close it!
  int Status = -42;
  /*
  if(LinkEstablished)
    Status = CAENComm_CloseDevice(BoardHandle);
  else
    if(Verbose)
      cout << "\nADAQBridge : Error closing link! Link is already closed!\n"
		<< endl;
  
  if(Status==0){
    LinkEstablished = false;
    if(Verbose)
      cout << "\nADAQBridge : Link successfully closed!\n"
		<< endl;
  }
  else
    if(Verbose and LinkEstablished)
      cout << "\nADAQBridge : Error closing link! Error code: " << Status << "\n"
		<< endl;
  */
  return Status;
}


// Method to set a value to an individual register of the V6534
int ADAQBridge::SetRegisterValue(uint32_t addr32, uint32_t data32)
{
  // Ensure that each register proposed for writing is a register that
  // is valid for user writing to prevent screwing up V6534 firmware
  if(CheckRegisterForWriting(addr32))
    return CAENComm_Write32(BoardHandle, addr32, data32); 
  else
    return -1;
}


// Method to get a value stored at an individual register of the V1718
int ADAQBridge::GetRegisterValue(uint32_t addr32, uint32_t *data32)
{ return CAENComm_Read32(BoardHandle, addr32, data32); }


// Method to check validity of V6534 register for writing
bool ADAQBridge::CheckRegisterForWriting(uint32_t addr32)
{ return true; }