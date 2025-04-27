# qkcache

Small, serverless, NoSQL database engine for caching long-term data on the disk.

## Purpose

Used to cache data that needs to be stored for longer periods of time, rather than
just being stored during execution in random access memory.

qkcache does this by providing a simple API and minimal system that doesn't require
SQL or servers. It also facilitates a dynamic structure to store binary data in, with no
real restriction on how it is retrieved.

## Functionality

qkcache stores its data on the disk in a file, while an index file keeps record
of each entry and where they start and end. This allows for quick and fluid retrieval
of data.
