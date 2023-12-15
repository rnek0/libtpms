/********************************************************************************/
/*										*/
/*						*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*										*/
/*  Licenses and Notices							*/
/*										*/
/*  1. Copyright Licenses:							*/
/*										*/
/*  - Trusted Computing Group (TCG) grants to the user of the source code in	*/
/*    this specification (the "Source Code") a worldwide, irrevocable, 		*/
/*    nonexclusive, royalty free, copyright license to reproduce, create 	*/
/*    derivative works, distribute, display and perform the Source Code and	*/
/*    derivative works thereof, and to grant others the rights granted herein.	*/
/*										*/
/*  - The TCG grants to the user of the other parts of the specification 	*/
/*    (other than the Source Code) the rights to reproduce, distribute, 	*/
/*    display, and perform the specification solely for the purpose of 		*/
/*    developing products based on such documents.				*/
/*										*/
/*  2. Source Code Distribution Conditions:					*/
/*										*/
/*  - Redistributions of Source Code must retain the above copyright licenses, 	*/
/*    this list of conditions and the following disclaimers.			*/
/*										*/
/*  - Redistributions in binary form must reproduce the above copyright 	*/
/*    licenses, this list of conditions	and the following disclaimers in the 	*/
/*    documentation and/or other materials provided with the distribution.	*/
/*										*/
/*  3. Disclaimers:								*/
/*										*/
/*  - THE COPYRIGHT LICENSES SET FORTH ABOVE DO NOT REPRESENT ANY FORM OF	*/
/*  LICENSE OR WAIVER, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, WITH	*/
/*  RESPECT TO PATENT RIGHTS HELD BY TCG MEMBERS (OR OTHER THIRD PARTIES)	*/
/*  THAT MAY BE NECESSARY TO IMPLEMENT THIS SPECIFICATION OR OTHERWISE.		*/
/*  Contact TCG Administration (admin@trustedcomputinggroup.org) for 		*/
/*  information on specification licensing rights available through TCG 	*/
/*  membership agreements.							*/
/*										*/
/*  - THIS SPECIFICATION IS PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED 	*/
/*    WARRANTIES WHATSOEVER, INCLUDING ANY WARRANTY OF MERCHANTABILITY OR 	*/
/*    FITNESS FOR A PARTICULAR PURPOSE, ACCURACY, COMPLETENESS, OR 		*/
/*    NONINFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS, OR ANY WARRANTY 		*/
/*    OTHERWISE ARISING OUT OF ANY PROPOSAL, SPECIFICATION OR SAMPLE.		*/
/*										*/
/*  - Without limitation, TCG and its members and licensors disclaim all 	*/
/*    liability, including liability for infringement of any proprietary 	*/
/*    rights, relating to use of information in this specification and to the	*/
/*    implementation of this specification, and TCG disclaims all liability for	*/
/*    cost of procurement of substitute goods or services, lost profits, loss 	*/
/*    of use, loss of data or any incidental, consequential, direct, indirect, 	*/
/*    or special damages, whether under contract, tort, warranty or otherwise, 	*/
/*    arising in any way out of use or reliance upon this specification or any 	*/
/*    information herein.							*/
/*										*/
/*  (c) Copyright IBM Corp. and others, 2023				  	*/
/*										*/
/********************************************************************************/

//** Introduction
//
// This file contains the function prototypes for the functions that need to be
// present in the selected math library. For each function listed, there should
// be a small stub function. That stub provides the interface between the TPM
// code and the support library. In most cases, the stub function will only need
// to do a format conversion between the Crypt_* formats to the internal support
// library format.  Since the external library also provides the buffer macros
// for the underlying types, this is typically just a cast from the TPM type to
// the internal type.
//
// Arithmetic operations return a BOOL to indicate if the operation completed
// successfully or not.

#ifndef MATH_LIBRARY_INTERFACE_H
#define MATH_LIBRARY_INTERFACE_H

// Types
#include "MathLibraryInterfaceTypes.h"

// ***************************************************************************
// Library Level Functions
// ***************************************************************************

//** ExtMath_LibInit()
// This function is called by CryptInit() so that necessary initializations can be
// performed on the cryptographic library.
LIB_EXPORT int ExtMath_LibInit(void);

// ***************************************************************************
// Integer/Number Functions (non-ECC)
// ***************************************************************************

// #################
// type initializers
// #################

//** ExtMath_Initialize_Int()
// Initialize* functions tells the Crypt_Int types how large of a value it can
// contain which is a compile time constant
LIB_EXPORT Crypt_Int* ExtMath_Initialize_Int(Crypt_Int* buffer, NUMBYTES bits);

// #################
// Buffer Converters
// #################
// convert TPM2B byte datainto the private format. The Crypt_Int must already be
// initialized with it's maximum size. Byte-based Initializers must be MSB first
// (TPM external format).
LIB_EXPORT Crypt_Int* ExtMath_IntFromBytes(
					   Crypt_Int* buffer, const BYTE* input, NUMBYTES byteCount);
// Convert Crypt_Int into external format as a byte array.
LIB_EXPORT BOOL ExtMath_IntToBytes(
				   const Crypt_Int* value, BYTE* output, NUMBYTES* pByteCount);

// #################
// Copy Functions
// #################

//*** ExtMath_Copy()
// Function to copy a bignum_t. If the output is NULL, then
// nothing happens. If the input is NULL, the output is set to zero.
LIB_EXPORT BOOL ExtMath_Copy(Crypt_Int* out, const Crypt_Int* in);

// ###############################
// Ordinary Arithmetic, writ large
// ###############################


//** ExtMath_Divide()
// This function divides two Crypt_Int* values. The function returns FALSE if there is
// an error in the operation. Quotient may be null, in which case this function returns
// only the remainder.
LIB_EXPORT BOOL ExtMath_Divide(Crypt_Int*       quotient,
			       Crypt_Int*       remainder,
			       const Crypt_Int* dividend,
			       const Crypt_Int* divisor);

//*** ExtMath_AddWord()
// This function adds a word value to a Crypt_Int*. This function always returns TRUE.
LIB_EXPORT BOOL ExtMath_AddWord(
				Crypt_Int* result, const Crypt_Int* op, crypt_uword_t word);

//*** ExtMath_SubtractWord()
// This function subtracts a word value from a Crypt_Int*. This function always
// returns TRUE.
LIB_EXPORT BOOL ExtMath_SubtractWord(
				     Crypt_Int* result, const Crypt_Int* op, crypt_uword_t word);

// ###############################
// Modular Arithmetic, writ large
// ###############################

//** ExtMath_ModMult()
// Compute result = (op1 * op2) mod modulus
LIB_EXPORT BOOL ExtMath_ModMult(Crypt_Int*       result,
				const Crypt_Int* op1,
				const Crypt_Int* op2,
				const Crypt_Int* modulus);
//** ExtMath_ModExp()
// Compute result = (number ^ exponent) mod modulus
// where ^ indicates exponentiation.
// This function is only needed when the TPM implements RSA.
LIB_EXPORT BOOL ExtMath_ModExp(Crypt_Int*       result,
			       const Crypt_Int* number,
			       const Crypt_Int* exponent,
			       const Crypt_Int* modulus);

//*** ExtMath_ModWord()
// compute numerator
// This function does modular division of a big number when the modulus is a
// word value.
LIB_EXPORT crypt_word_t ExtMath_ModWord(const Crypt_Int* numerator,
					crypt_word_t     modulus);

// ###############################
// Queries
// ###############################

//*** ExtMath_UnsignedCmp()
// This function performs a comparison of op1 to op2. The compare is approximately
// constant time if the size of the values used in the compare is consistent
// across calls (from the same line in the calling code).
//  Return Type: int
//      < 0             op1 is less than op2
//      0               op1 is equal to op2
//      > 0             op1 is greater than op2
LIB_EXPORT int ExtMath_UnsignedCmp(const Crypt_Int* op1, const Crypt_Int* op2);

//*** ExtMath_UnsignedCmpWord()
// Compare a Crypt_Int* to a crypt_uword_t.
//  Return Type: int
//      -1              op1 is less that word
//      0               op1 is equal to word
//      1               op1 is greater than word
LIB_EXPORT int ExtMath_UnsignedCmpWord(const Crypt_Int* op1, crypt_uword_t word);

//*** ExtMath_IsEqualWord()
// Compare a Crypt_Int* to a crypt_uword_t for equality
//  Return Type: BOOL
LIB_EXPORT BOOL ExtMath_IsEqualWord(const Crypt_Int* bn, crypt_uword_t word);

//*** ExtMath_GetLeastSignificant32bits()
//
// This function returns the least significant 32-bits of an integer value
// Return Type: uint32_t
LIB_EXPORT uint32_t ExtMath_GetLeastSignificant32bits(const Crypt_Int* bn);

//*** ExtMath_SizeInBits()
//
// This function returns the number of bits required to hold a number. It is one
// greater than the Msb.  This function is expected to be side channel safe, and
// may be O(size) or O(1) where 'size' is the allocated (not actual) size of the
// value.
LIB_EXPORT unsigned ExtMath_SizeInBits(const Crypt_Int* n);

// ###############################
// Bitwise Operations
// ###############################

//*** ExtMath_TestBit()
// This function is used to check to see if a bit is SET in a bignum_t. The 0th bit
// is the LSb of d[0].
//  Return Type: BOOL
//      TRUE(1)         the bit is set
//      FALSE(0)        the bit is not set or the number is out of range
LIB_EXPORT BOOL ExtMath_TestBit(Crypt_Int*   bn,     // IN: number to check
				unsigned int bitNum  // IN: bit to test
				);

//*** ExtMath_ShiftRight()
// This function will shift a Crypt_Int* to the right by the shiftAmount.
// This function always returns TRUE.
LIB_EXPORT BOOL ExtMath_ShiftRight(
				   Crypt_Int* result, const Crypt_Int* toShift, uint32_t shiftAmount);

#endif