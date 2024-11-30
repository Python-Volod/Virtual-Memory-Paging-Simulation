# Virtual-Memory-Paging-Simulation

This program simulates a virtual memory paging system by managing a page table and main memory. It tracks memory references and calculates page faults based on access patterns.

The page table maps virtual memory pages to physical memory frames, and each access is tracked for hits and misses. When a page fault occurs (i.e., a page is accessed that is not currently in main memory), the program simulates the loading of that page into a free frame. If no free frames are available, the program will replace the least recently used frame with the new page.

Key Concepts:

Page Table: A data structure that stores the mapping between virtual memory pages and physical memory frames. It also tracks whether a page is currently valid in memory (using a validation bit).
Page Fault: Occurs when a page is accessed but not found in main memory. This leads to the loading of the page into a frame, causing the page fault ratio to increase.
Hits/Misses: The number of times a page is found in memory (hit) versus the number of times a page is not found and must be loaded (miss).
Main Memory: A physical memory with limited size, divided into frames. Pages from virtual memory are mapped into these frames.
Key Features:
Converts virtual memory addresses from binary to decimal for indexing.
Tracks page faults and calculates the page fault ratio.
Simulates the process of loading pages into memory and updating the page table.
Outputs detailed information for each memory access, page table status, and memory content.
The program accepts input describing the page size, main memory size, and memory references, and provides detailed output on memory accesses, hits, misses, and the current state of the page table and main memory.
