/*
 * TLB handling
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: tlb.c,v 1.6 2004/04/16 10:54:29 ttakanen Exp $
 *
 */
#include "kernel/panic.h"
#include "kernel/assert.h"
#include "vm/tlb.h"
#include "vm/pagetable.h"
#include "kernel/thread.h"

//Definere en state som vi kan hente i exceptionsne
int tlb_index;
pagetable_t* pagetable;


void tlb_modified_exception(void)
{
//Da vi hader fejl, må vi hellere panikke.
  kprintf("it's all tlb_modified_exceptions fault");
    KERNEL_PANIC("Don't write if the dirty bit is 0, it's just not allowed, ok?");
}

void tlb_load_exception(void)
{
// kprintf("STOREAH");
  tlb_exception_state_t state;
  pagetable_t* pagetable;
//fylder en state på vores state.
 _tlb_get_exception_state(&state);
  
  pagetable = thread_get_current_thread_entry()->pagetable;

 _tlb_probe(pagetable->entries); 
 if(tlb_index < 0) { 
   KERNEL_PANIC("pagetable not found!");
 }

  for(uint32_t i = 0; i < pagetable->valid_count; i++) {
        _tlb_write_random((tlb_entry_t*) &pagetable->entries[i]);
  } 

 }
void tlb_store_exception(void)
{
// kprintf("STOREAH");
  tlb_exception_state_t state;
  pagetable_t* pagetable;
//fylder en state på vores state.
 _tlb_get_exception_state(&state);
  
  pagetable = thread_get_current_thread_entry()->pagetable;

  _tlb_probe(pagetable->entries); 
  if(tlb_index < 0) { 
   KERNEL_PANIC("pagetable not found!");
  }

  for(uint32_t i = 0; i < pagetable->valid_count; i++) {
        _tlb_write_random((tlb_entry_t*) &pagetable->entries[i]);
  } 



}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

void tlb_fill(pagetable_t *pagetable)
{
    if(pagetable == NULL)
	return;

    /* Check that the pagetable can fit into TLB. This is needed until
     we have proper VM system, because the whole pagetable must fit
     into TLB. */
    KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

    _tlb_write(pagetable->entries, 0, pagetable->valid_count);

    /* Set ASID field in Co-Processor 0 to match thread ID so that
       only entries with the ASID of the current thread will match in
       the TLB hardware. */
    _tlb_set_asid(pagetable->ASID);
}
