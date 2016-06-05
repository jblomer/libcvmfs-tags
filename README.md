Demo of how to use libcvmfs to access data of a specific repository tag.

Installation
============

Run `make`.  It will first clone and compile libcvmfs and then compile `libcvmfs_tags`.

Issues
======

  * Squid discovery (perhaps `http_proxy` points right, will be eventually solved by WPAD)
  * Cache quota management, currently only available in the fuse module
  * Static libcvmfs.a is big, perhaps a shared library might be better


