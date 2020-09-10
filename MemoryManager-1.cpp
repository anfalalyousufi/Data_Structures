/*
	Name: Anfal AlYousufi 
	Date: 17/02/17 
	Description: MemoryManager.cpp 
*/

#include <cassert>
#include <iostream>
#include "dlUtils.h"
#include "MemoryManager.h"
#include "blockdata.h"
#define nullptr NULL

MemoryManager::MemoryManager(unsigned int memtotal): memsize(memtotal)
{
   baseptr = new unsigned char[memsize];
   blockdata dummyBlock(0,false,0);
   blockdata originalBlock(memsize,true,baseptr);
   header = new dlNode<blockdata>(dummyBlock,nullptr,nullptr);
   trailer = new dlNode<blockdata>(dummyBlock,nullptr,nullptr);
   header->next = new dlNode<blockdata>(originalBlock,header,trailer);
   trailer->prev = header->next;
   
}

MemoryManager::~MemoryManager()
{
  delete [] baseptr;
  clearList(header);
}

void MemoryManager::showBlockList() 
{
  printDlList(header,trailer,"->");
}

void MemoryManager::splitBlock(dlNode<blockdata> *p, unsigned int chunksize)
{
  // Complete the code for this method
   int newSize = p->info.blocksize - chunksize;

   blockdata C(newSize, true, baseptr+chunksize);
   insertAfter(trailer, p, C );
   
   
   p->info.free = false;
   p->info.blocksize = chunksize;
      
  
}

unsigned char * MemoryManager::malloc(unsigned int request)
{
  // Complete the code for this method
   dlNode<blockdata> *B = header;
   while (B!= NULL)
   {
       if (B->info.free == true && B->info.blocksize >= request)
       {
           splitBlock(B, request);
           return B->info.blockptr;
           break;
       }
       B=B->next;
   }
  
   return NULL;

}

void MemoryManager::mergeForward(dlNode<blockdata> *p)
{
  // Complete the code for this method
   int newSize = p->info.blocksize + p->next->info.blocksize;
   p->info.blocksize = newSize;
  
   dlNode<blockdata> *temp = p->next;
   if (temp->next == NULL)
       p->next = NULL;
   else
   {
       p->next = temp->next;
       p->next->prev = p;
      
   }

       delete temp;

}

void MemoryManager::mergeBackward(dlNode<blockdata> *p)
{ 
  // Complete the code for this method
   int newSize = p->prev->info.blocksize + p->info.blocksize;
   p->info.blocksize = newSize;
   p->info.blockptr = p->prev->info.blockptr;
   dlNode<blockdata> *temp = p->prev;
   if (temp->prev == NULL)
   {
       p->prev = NULL;
   trailer=p;
   }
   else
   {
       p->prev = temp->prev;
       p->prev->next = p;
   }
   delete temp;

}

void MemoryManager::free(unsigned char *ptr2block)
{
  // Complete the code for this method
    dlNode<blockdata> *curr=header;
   assert(curr != NULL);
  
   while (curr && curr->info.blockptr != ptr2block)
   {
      
       curr = curr->next;
   }
   assert(curr);
  


   if (curr->info.blockptr == ptr2block)
   {
       curr->info.free = true;

       if (curr->prev != NULL)
       {
           if (curr->prev->info.free == true && curr->info.free == true)
           {
               mergeBackward(curr);
           }
       }

       if (curr->next != NULL)
       {
           if (curr->info.free == true && curr->next->info.free == true)
           {
               mergeForward(curr);
           }

       };

   }
   else
       return;

} 

